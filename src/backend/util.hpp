#pragma once

#include <cmath>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

namespace mandelbrot
{

struct Area
{
    unsigned left, right, top, bottom;
};

struct RGB
{
    RGB() {
        r = g = b = 0;
    }
    RGB(int v) {
        r = g = b = static_cast<int>(v);
    }
    RGB(double v) { // achromatic
        r = g = b = static_cast<int>(floor(v));
    }
    RGB(float v) {
        r = g = b = static_cast<int>(floor(v));
    }
    RGB(int x, int y, int z) : r(x), g(y), b(z) {}
    
    RGB(float x, float y, float z) {
        r = static_cast<int>(floor(x));
        g = static_cast<int>(floor(y));
        b = static_cast<int>(floor(z));
    }
    RGB(double x, double y, double z) {
        r = static_cast<int>(floor(x));
        g = static_cast<int>(floor(y));
        b = static_cast<int>(floor(z));
    }
    RGB(const RGB& other) {
        r = other.r; g = other.g; b = other.b;
    }
    RGB& operator=(const RGB& other) {
        r = other.r; g = other.g; b = other.b;
        return *this;
    }
    operator std::string() const {
        return boost::lexical_cast<std::string>(r) + ":" + boost::lexical_cast<std::string>(g) + ":" + boost::lexical_cast<std::string>(b);
    }
    int r, g, b;
};

ostringstream& operator<<(ostringstream& o, const RGB& rgb);
ostream& operator<<(ostream& o, const RGB& rgb);

struct HSV
{
    double h, s, v;
};

// original implementation in http://www.cs.rit.edu/~ncs/color/t_convert.html
RGB HSVtoRGB(double h, double s, double v);
RGB HSBtoRGB(float hue, float saturation, float brightness);

}