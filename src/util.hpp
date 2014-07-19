#pragma once

#include <cmath>

using namespace std;

namespace mandelbrot
{

struct RGB
{
    RGB(double v) // achromatic
    {
        r = g = b = static_cast<unsigned char>(floor(v));
    }
    RGB(double a, double b, double c)
    {
        r = static_cast<unsigned char>(floor(a));
        g = static_cast<unsigned char>(floor(b));
        b = static_cast<unsigned char>(floor(c));
    }
    unsigned char r, g, b;
};

struct HSV
{
    double h, s, v;
};

// original implementation in http://www.cs.rit.edu/~ncs/color/t_convert.html
RGB HSVtoRGB(double h, double s, double v);

}