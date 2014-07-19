#include "mandelbrot.hpp"

int main(int argc, char* argv[])
{
    //mandelbrot::Mandelbrot mandelbrot(1920, 1080);
    mandelbrot::Mandelbrot mandelbrot;

    mandelbrot.compute();

    return 0;
}
