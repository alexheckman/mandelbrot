#include "datasplit.hpp"
#include <iostream>
#include <queue>
#include <string>
#include <exception>
#include <algorithm>
#include <random>
#include <chrono>

namespace mandelbrot
{

namespace split
{

DataSplit::type DataSplit::next()
{
    if(position_ != limits_.end()) {
        DataSplit::type l(new DataSplit::type::element_type(*position_));
        ++position_;
        return l;
    }
    return nullptr;
}

void DataSplit::reset()
{
    position_ = limits_.begin();
}


SimpleDataSplit::SimpleDataSplit(unsigned w, unsigned h)
: DataSplit(w, h), percentage_(0.05), vertical_(false)
{
    doit();
}

void SimpleDataSplit::doit()
{
    unsigned a = 0, b = 0;
    unsigned& direction = vertical_ ? height_ : width_;
    unsigned step = static_cast<unsigned>(direction * percentage_);
    while (a < direction) {
        b += step;
        if (direction - b < step) b = direction;
        if (vertical_) limits_.push_back({0, width_, a, b});
        else limits_.push_back({a, b, 0, height_});
        a = b;
    }

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(limits_.begin(), limits_.end(), std::default_random_engine(seed));
}



PuzzleDataSplit::PuzzleDataSplit(unsigned w, unsigned h)
: DataSplit(w, h), percentage_(0.04)
{
    doit();
}

void PuzzleDataSplit::doit()
{
    unsigned wstep = static_cast<unsigned>(width_ * percentage_);
    unsigned hstep = static_cast<unsigned>(height_ * percentage_);

    unsigned l, r, u, b;
    l = r = u = b = 0;

    while (l < width_) {
        u = 0, b = 0;
        r += wstep;
        if (width_ - r < wstep) r = width_;
        while (u < height_) {
            b += hstep;
            if (height_ - b < hstep) b = height_;
            limits_.push_back({l, r, u, b});
            u = b;
        }
        l = r;
    }

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(limits_.begin(), limits_.end(), std::default_random_engine(seed));
}


} //namespace split

} //namespace mandelbrot