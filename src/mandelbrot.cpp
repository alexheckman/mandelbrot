#include "mandelbrot.hpp"
#include "util.hpp"

#include <complex>
#include <utility>
#include <tuple>
#include <exception>
#include <cmath>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/format.hpp>

namespace mandelbrot
{

Mandelbrot::Bounds Mandelbrot::bounds;
    
Mandelbrot::Mandelbrot(Range x, Range y, unsigned int width, unsigned int height, unsigned int max_iterations)
: x_(x), 
  y_(y), 
  width_(width), 
  height_(height),
  dx_((x_.second - x_.first) / static_cast<long double>(width)),
  dy_((y_.second - y_.first) / static_cast<long double>(height)),
  max_iterations_(max_iterations),
  conv_limit_(2),
  output_(width_)
{ 
    //pre-allocate memory for each row
    for (unsigned i = 0; i < width_; i++) {
        output_[i].reserve(height_);
    }
}

Mandelbrot::Mandelbrot(const std::vector<long double>& vx, const std::vector<long double>& vy, unsigned int width, unsigned int height, unsigned int max_iterations)
: Mandelbrot(validate_range(vx, X), validate_range(vy, Y), width, height, max_iterations)
{}


Mandelbrot::Mandelbrot(unsigned int width, unsigned int height, unsigned int max_iterations)
: Mandelbrot(bounds.x, bounds.y, width, height, max_iterations)
{}

Mandelbrot::Mandelbrot(unsigned int max_iterations)
: Mandelbrot(800, 600, max_iterations)
{}

RGB
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

int
Mandelbrot::compute()
{
    //for each pixel compute a color
    for (unsigned x = 0; x < width_; x++) {
        for (unsigned y = 0; y < height_; y++) {
            calc_pixel(x, y);
        }
    }
}

Mandelbrot::Range
Mandelbrot::validate_range(const std::vector<long double>& v, Axis ax)
{
    std::string msg = "";
    std::unique_ptr<Mandelbrot::Range> default_range;
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
    
    if (v.size() > 0) {
        if (v.size() != 2)
            throw std::runtime_error(msg + " represent a range, therefore only 2 values are allowed.");
        if (v[0] > v[1])
            throw std::runtime_error("First value must be lower than the second.");
    } else {
        return *default_range;
    }
    
    //check if it's inside bounds
    if (v[0] < default_range->first && v[1] > default_range->second)
        throw std::out_of_range((boost::format("One of %1% values is out of range. Must be between [%2%, %3%]") 
                                    % msg % default_range->first % default_range->second).str());
        
    return Mandelbrot::Range(v[0], v[1]);
}

void
Mandelbrot::writeCSV(std::string suffix)
{
    FILE *f = fopen((csv_filename_ + suffix).c_str(), "w");
    if (!f) {
        throw std::runtime_error("Could not open file for writing");
    }
    std::ostringstream oss;
    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++)
        {
            oss << output_[j][i] << ((j < static_cast<int>((width_ - 1))) ? "," : "");
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