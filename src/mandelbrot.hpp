#pragma once

#include "util.hpp"
#include "datasplit.hpp"

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
        Action(Mandelbrot& m) : mandelbrot(m) {}
        void operator()();

        Mandelbrot& mandelbrot;
    };

    static Bounds bounds;

    Mandelbrot(Range x, Range y, unsigned width, unsigned height, split::DataSplitType type = split::DataSplitType::Simple);
    Mandelbrot(const std::vector<long double> &vx, const std::vector<long double> &vy, 
                unsigned width, unsigned height, split::DataSplitType type = split::DataSplitType::Simple);
    Mandelbrot(unsigned width, unsigned height, split::DataSplitType type = split::DataSplitType::Simple);
    Mandelbrot(split::DataSplitType type = split::DataSplitType::Simple);

    void set_iterations(unsigned i) { max_iterations_ = i; }
    void set_parallel(unsigned p) { parallel_ = p; }

    void compute();
    void writeCSV(std::string suffix = "");

private:
    PointResult calc_point(CPoint&& c);
    void calc_pixel(unsigned x, unsigned y);
    Range validate_range(const std::vector<long double>& v, Axis ax);

    Range x_, y_; //x-domain, y-codomain of mandelbrot set
    const unsigned width_, height_; //screen measurements
    const long double dx_, dy_; //increment ratios
    const long double conv_limit_;
    unsigned max_iterations_, parallel_;

    std::unique_ptr<split::IDataSplit> data_pool_;
    std::vector< std::vector<RGB> > output_;

    //TODO color palette ?
    const std::string csv_filename_ = "mandelbrot_pixel_matrix.csv";
    const RGB black = { 0, 0, 0 };
};

}