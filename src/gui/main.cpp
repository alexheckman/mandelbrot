#include "mandelbrotWidget.hpp"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MandelbrotWidget window;

    window.resize(1280, 710);
    window.setWindowTitle("Mandelbrot Set");
    window.show();

    return app.exec();
}
