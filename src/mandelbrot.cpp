#include "mandelbrot.hpp"
#include "util.hpp"
#include "datasplit.hpp"

#include <complex>
#include <utility>
#include <tuple>
#include <exception>
#include <cmath>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <boost/format.hpp>

namespace mandelbrot
{

Mandelbrot::Bounds Mandelbrot::bounds;

Mandelbrot::Mandelbrot(Range x, Range y, unsigned int width, unsigned int height, split::DataSplitType type)
: x_(validate_range(x, X)),
  y_(validate_range(y, Y)),
  width_(width),
  height_(height),
  dx_((x_.second - x_.first) / static_cast<long double>(width_)),
  dy_((y_.second - y_.first) / static_cast<long double>(height_)),
  conv_limit_(2),
  max_iterations_(256),
  parallel_(1),
  data_pool_(split::DataSplitFactory::create(type, width_, height_)),
  output_(width_)
{
    //pre-allocate memory for each row
    for (unsigned i = 0; i < width_; i++) {
        output_[i].reserve(height_);
    }
}


Mandelbrot::Mandelbrot(unsigned int width, unsigned int height, split::DataSplitType type)
: Mandelbrot(bounds.x, bounds.y, width, height, type)
{}

Mandelbrot::Mandelbrot(split::DataSplitType type)
: Mandelbrot(800, 600, type)
{}

void Mandelbrot::set_area(Mandelbrot::Range x, Mandelbrot::Range y)
{
    x_ = validate_range(x, X);
    y_ = validate_range(y, Y);
}

void Mandelbrot::set_area(Area area)
{
    if (area.left > area.right || area.top > area.bottom)
        throw std::out_of_range("Not a valid area.");

    set_area({x_.first + dx_ * area.left, x_.second + dx_ * area.right},
                {y_.first + dy_ * area.top, y_.second = dy_ * area.bottom});
}

void
Mandelbrot::calc_pixel(unsigned x, unsigned y)
{
    if (x > width_ || y > height_)
        throw std::out_of_range("Point is outside coordinates!");

    //convert graphical coordinate to mandelbrot plane
    long double
        xx = x_.first + dx_ * x,
        yy = y_.first + dy_ * y;

    auto point = calc_point(CPoint(xx, yy));

    if (point.iterations > 0) {
        // as described in http://stackoverflow.com/questions/369438/smooth-spectrum-for-mandelbrot-set-rendering
        auto itsmooth = point.iterations + 1 - std::log(std::log(std::abs(point.zn)))/std::log(2.0);
        itsmooth /= max_iterations_; //get a number between 0..1
        output_[x][y] = HSBtoRGB(0.85f + 10 * itsmooth, 0.6f, 0.9f);
        //output_[x][y] = HSBtoRGB(0.65f + 10 * itsmooth, 0.8f, 0.8f);
    } else {
        output_[x][y] = black;
    }
}

Mandelbrot::PointResult
Mandelbrot::calc_point(CPoint&& c)
{
    unsigned it = 0;
    CPoint z(0,0);

    do {
        z = z*z + c;
        it++;
    } while(it < max_iterations_ && std::abs(z) < conv_limit_);

    return (it < max_iterations_) ? PointResult({static_cast<int>(it), z}) : PointResult({-1, z});
}

void Mandelbrot::Action::operator()()
{
    for(;;) {
        static std::mutex  mtx; //one time only init
        split::DataSplit::type limit;
        {
            std::lock_guard<std::mutex> lock(mtx);
            limit = mandelbrot.data_pool_->next();
            if(!limit.get())
                return;
        }

        //for each pixel compute a color
        for (unsigned i = limit->left; i < limit->right; i++) { //left -> right
            for (unsigned j = limit->top; j < limit->bottom; j++) { //top -> bottom
                mandelbrot.calc_pixel(i, j);
            }
        }
    }
}

void
Mandelbrot::compute()
{
    dx_ = (x_.second - x_.first) / static_cast<long double>(width_);
    dy_ = (y_.second - y_.first) / static_cast<long double>(height_);
    data_pool_->reset();

    if (parallel_ <= 1) {
        //for each pixel compute a color
        for (unsigned x = 0; x < width_; x++) {
            for (unsigned y = 0; y < height_; y++) {
                calc_pixel(x, y);
            }
        }
    } else {
        std::vector<std::thread> workers;
        for(unsigned i = 0; i < parallel_; i++)
            workers.emplace_back(Action(*this));

        for( auto& thread : workers)
            thread.join();
    }
}

Mandelbrot::Range
Mandelbrot::validate_range(Range r, Axis ax)
{
    std::string msg = "";
    std::unique_ptr<Range> default_range;
    switch(ax) {
        case X:
            msg = "'x'";
            default_range.reset(new Mandelbrot::Range(Mandelbrot::bounds.x));
            break;
        case Y:
            msg = "'y'";
            default_range.reset(new Mandelbrot::Range(Mandelbrot::bounds.y));
            break;
    }

    if (r.first > r.second)
        throw std::runtime_error("First value must be lower than the second.");

    //check if it's inside bounds
    if (r.first < default_range->first && r.second > default_range->second)
        throw std::out_of_range((boost::format("One of %1% values is out of range. Must be between [%2%, %3%]")
                                    % msg % default_range->first % default_range->second).str());

    return r;
}


void
Mandelbrot::writeCSV(std::string suffix)
{
    FILE *f = fopen((csv_filename_ + suffix).c_str(), "w");
    if (!f) {
        throw std::runtime_error("Could not open file for writing");
    }
    std::ostringstream oss;
    for (unsigned i = 0; i < height_; i++) {
        for (unsigned j = 0; j < width_; j++)
        {
            oss << output_[j][i] << ((j < (width_ - 1)) ? "," : "");
        }
        oss << std::endl;
    }

    {
        std::string output = oss.str();
        fwrite(output.c_str(), output.size(), 1, f);
    }
    fclose(f);
}


} //namespace mandelbrot