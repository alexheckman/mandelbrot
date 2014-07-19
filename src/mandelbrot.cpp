#include "mandelbrot.hpp"
#include "util.hpp"

#include <complex>
#include <utility>
#include <tuple>
#include <exception>
#include <cmath>

namespace mandelbrot
{

Mandelbrot::Mandelbrot(Range x, Range y, unsigned int width, unsigned int height, unsigned int max_iterations)
: x_(x), 
  y_(y), 
  width_(width), 
  height_(height),
  dx_((x_.second - x_.first) / static_cast<long double>(width)),
  dy_((y_.second - y_.first) / static_cast<long double>(height)),
  max_iterations_(max_iterations),
  conv_limit_(2),
  output_(height)
{ 
    //pre-allocate memory for each row
    for (unsigned i = 0; i < height_; i++) {
        output_[i].reserve(width);
    }
}

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height, unsigned int max_iterations)
: Mandelbrot(std::make_pair(-2.25, 1), std::make_pair(-1, 1), width, height, max_iterations)
{}

Mandelbrot::Mandelbrot(unsigned int max_iterations)
: Mandelbrot(std::make_pair(-2.25, 1), std::make_pair(-1, 1), 800, 600, max_iterations)
{}

RGB Mandelbrot::calc_pixel(unsigned x, unsigned y)
{
    if (x > height_ || y > width_)
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
        output_[x][y] = HSVtoRGB(0.95 + 10 * itsmooth, 0.6, 0.85);
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

int Mandelbrot::compute()
{
    //for each pixel compute a color
    for (unsigned x = 0; x < height_; x++) {
        for (unsigned y = 0; y < width_; y++) {
            calc_pixel(x, y);
        }
    }
}

}