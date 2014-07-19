#pragma once

#include "util.hpp"

#include <utility>
#include <tuple>
#include <vector>
#include <complex>

namespace mandelbrot
{

class Mandelbrot
{
    typedef std::pair<long double, long double> Range;
    typedef std::complex<long double> CPoint;

public:
    Mandelbrot(unsigned int max_iterations = 255);
    Mandelbrot(unsigned width, unsigned height, unsigned int  max_iterations = 255);
    Mandelbrot(Range x, Range y,
                unsigned width, unsigned height, unsigned int max_iterations = 255);
    int compute();
    void write_CSV();

private:
    struct PointResult
    {
        int iterations;
        CPoint zn;
    };

    PointResult calc_point(CPoint&& c);
    RGB calc_pixel(unsigned x, unsigned y);

    Range x_, y_; //x-domain, y-codomain of mandelbrot set
    const unsigned width_, height_; //screen measurements
    const long double dx_, dy_; //increment ratios
    const unsigned max_iterations_;
    const long double conv_limit_;
    //TODO color palette ?
    std::vector< std::vector<RGB> > output_;

    const std::string csv_filename = "mandelbrot_pixel_matrix.csv";
    const RGB black = { 0, 0, 0 };
};

}