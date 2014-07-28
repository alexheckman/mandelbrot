#include "util.hpp"

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

namespace mandelbrot
{

ostringstream& operator<<(ostringstream& o, const RGB& rgb)
{
    o << boost::lexical_cast<std::string>(rgb.r) << ":"
      << boost::lexical_cast<std::string>(rgb.g) << ":"
      << boost::lexical_cast<std::string>(rgb.b);

    return o;
}

ostream& operator<<(ostream& o, const RGB& rgb)
{
    o << boost::lexical_cast<std::string>(rgb.r) << ":"
      << boost::lexical_cast<std::string>(rgb.g) << ":"
      << boost::lexical_cast<std::string>(rgb.b);

    return o;
}

RGB HSVtoRGB(double h, double s, double v)
{
    double r, g, b; 
    int i;
    double f, p, q, t;
    if( s == 0 ) {
        return RGB(v); // achromatic (grey)
    }
    h /= 60;            // sector 0 to 5
    i = floor( h );
    f = h - i;          // factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    switch( i ) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:        // case 5:
            r = v;
            g = p;
            b = q;
            break;
    }

    return RGB(r, g, b);
}

/*
RGB HSBtoRGB(float hue, float saturation, float brightness) {
    int r = 0, g = 0, b = 0;
    if (saturation == 0) {
        r = g = b = static_cast<int>((brightness * 255.0f + 0.5f));
    } else {
        float h = (hue - static_cast<float>(std::floor(hue))) * 6.0f;
        float f = h - static_cast<float>(std::floor(h));
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        switch (static_cast<int>(h)) {
        case 0:
            r = static_cast<int>((brightness * 255.0f + 0.5f));
            g = static_cast<int>((t * 255.0f + 0.5f));
            b = static_cast<int>((p * 255.0f + 0.5f));
            break;
        case 1:
            r = static_cast<int>((q * 255.0f + 0.5f));
            g = static_cast<int>((brightness * 255.0f + 0.5f));
            b = static_cast<int>((p * 255.0f + 0.5f));
            break;
        case 2:
            r = static_cast<int>((p * 255.0f + 0.5f));
            g = static_cast<int>((brightness * 255.0f + 0.5f));
            b = static_cast<int>((t * 255.0f + 0.5f));
            break;
        case 3:
            r = static_cast<int>((p * 255.0f + 0.5f));
            g = static_cast<int>((q * 255.0f + 0.5f));
            b = static_cast<int>((brightness * 255.0f + 0.5f));
            break;
        case 4:
            r = static_cast<int>((t * 255.0f + 0.5f));
            g = static_cast<int>((p * 255.0f + 0.5f));
            b = static_cast<int>((brightness * 255.0f + 0.5f));
            break;
        case 5:
            r = static_cast<int>((brightness * 255.0f + 0.5f));
            g = static_cast<int>((p * 255.0f + 0.5f));
            b = static_cast<int>((q * 255.0f + 0.5f));
            break;
        }
    }

    return RGB(r, g, b);
}
*/

RGB HSBtoRGB(float hue, float saturation, float brightness) {
    int r = 0, g = 0, b = 0;
    if (saturation == 0) {
        r = g = b = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
    } else {
        float h = (hue - static_cast<float>(std::floor(hue))) * 6.0f;
        float f = h - static_cast<float>(std::floor(h));
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        switch (static_cast<int>(floor(h))) {
        case 0:
            r = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            g = static_cast<int>(floor((t * 255.0f + 0.5f)));
            b = static_cast<int>(floor((p * 255.0f + 0.5f)));
            break;
        case 1:
            r = static_cast<int>(floor((q * 255.0f + 0.5f)));
            g = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            b = static_cast<int>(floor((p * 255.0f + 0.5f)));
            break;
        case 2:
            r = static_cast<int>(floor((p * 255.0f + 0.5f)));
            g = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            b = static_cast<int>(floor((t * 255.0f + 0.5f)));
            break;
        case 3:
            r = static_cast<int>(floor((p * 255.0f + 0.5f)));
            g = static_cast<int>(floor((q * 255.0f + 0.5f)));
            b = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            break;
        case 4:
            r = static_cast<int>(floor((t * 255.0f + 0.5f)));
            g = static_cast<int>(floor((p * 255.0f + 0.5f)));
            b = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            break;
        case 5:
            r = static_cast<int>(floor((brightness * 255.0f + 0.5f)));
            g = static_cast<int>(floor((p * 255.0f + 0.5f)));
            b = static_cast<int>(floor((q * 255.0f + 0.5f)));
            break;
        }
    }

    return RGB(r, g, b);
}

}