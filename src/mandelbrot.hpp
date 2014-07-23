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
public:
    enum Axis { X, Y };
    typedef std::pair<long double, long double> Range;
    typedef std::complex<long double> CPoint;
    struct Bounds {
        Range x, y;
        Bounds() : x(std::make_pair(-2.25, 1)), y(std::make_pair(-1, 1)) {}
    };
    
    struct PointResult
    {
        int iterations;
        CPoint zn;
    };
    
    struct Action
    {
        Action(Mandelbrot& m, std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) : mandelbrot(m), x(a), y(b) {}
        void operator()();

        Mandelbrot& mandelbrot;
        std::pair<unsigned, unsigned> x, y;
    };

    static Bounds bounds;

    Mandelbrot(Range x, Range y, unsigned width, unsigned height, unsigned int max_iterations = 255);
    Mandelbrot(const std::vector<long double> &vx, const std::vector<long double> &vy, 
                unsigned width, unsigned height, unsigned int max_iterations = 255);
    Mandelbrot(unsigned width, unsigned height, unsigned int  max_iterations = 255);
    Mandelbrot(unsigned int max_iterations = 255);

    void compute(unsigned threads = 1);
    void writeCSV(std::string suffix = "");

private:
    PointResult calc_point(CPoint&& c);
    void calc_pixel(unsigned x, unsigned y);
    Range validate_range(const std::vector<long double>& v, Axis ax);

    Range x_, y_; //x-domain, y-codomain of mandelbrot set
    const unsigned width_, height_; //screen measurements
    const long double dx_, dy_; //increment ratios
    const unsigned max_iterations_;
    const long double conv_limit_;
    //TODO color palette ?
    std::vector< std::vector<RGB> > output_;

    const std::string csv_filename_ = "mandelbrot_pixel_matrix.csv";
    const RGB black = { 0, 0, 0 };
};

}