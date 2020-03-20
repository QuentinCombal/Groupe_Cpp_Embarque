#ifndef MANDELBROT_FRACTAL_H
#define MANDELBROT_FRACTAL_H

#include "colorgradient.h"
#include <QObject>
#include <QImage>
#include <QtWidgets>
#include <complex>

class Mandelbrot_fractal : public QImage
{
private:
  int width_, height_;
  double v_pixel2rect(int xpos, float xc, float d);
  double h_pixel2rect(int ypos, float yc, float d);
  void process_sub_image(int rank, int maxThreads, ColorGradient &gradient, float xc, float yc, float d, bool julia);
public:
  Mandelbrot_fractal(int width, int height, double xc, double yc, double d, bool julia, ColorGradient &gradient);
};

#endif // MANDELBROT_FRACTAL_H
