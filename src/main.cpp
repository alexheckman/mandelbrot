#include "mandelbrot.hpp"
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <vector>
#include <exception>

using namespace mandelbrot;

int run(int argc, char *argv[]) {
    namespace po = boost::program_options;

    unsigned w, h, max_iterations;
    w = h = max_iterations = 0;
    bool write_csv = false;
    std::vector<long double> x, y;
    po::options_description desc("Mandelbrot help");
    desc.add_options()
        ("help,h", "Display help message")
        ("width,w", po::value<unsigned>(&w)->default_value(1280), "width of produced image in pixels")
        ("height,h", po::value<unsigned>(&h)->default_value(720), "height of produced image in pixels")
        ("max-iterations,i", po::value<unsigned>(&max_iterations)->default_value(256), "maximum number of iterations to test a point")
        ("x,x", po::value<std::vector<long double>>(&x)->multitoken(), "range pair along x-axis")
        ("y,y", po::value<std::vector<long double>>(&y)->multitoken(), "range pair along y-axis")
        ("write-csv", po::value<bool>(&write_csv)->default_value(true), "write pixel matrix of shape [w,h,rgb] as CSV file. Enabled by default.");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << " HELP! " << std::endl;
        return 1;
    }

    if (w > 1920 && h > 1080)
        throw std::out_of_range("Maximum resolution is 1920x1080.");


    mandelbrot::Mandelbrot mandelbrot(x, y, w, h, max_iterations);
    mandelbrot.compute();

    if (write_csv) {
        mandelbrot.writeCSV();
    }

    //mandelbrot::Mandelbrot mandelbrot({0.001643721966,0.001643721976}, {0.822467633293,0.822467633303}, 1024, 768, 512);
    //mandelbrot::Mandelbrot mandelbrot({-1.76839782, -1.76839772}, {0.00553854 ,0.00553864}, 1067, 600, 512); //16:9
    //mandelbrot::Mandelbrot mandelbrot({-1.749759145135, -1.749759145125}, {0.000000003680, 0.000000003690}, 1067, 600, 512); //16:9
    return 0;
}

int main(int argc, char* argv[])
{
    try {
        return run(argc, argv);
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
