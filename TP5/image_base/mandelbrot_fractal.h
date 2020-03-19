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
  double v_pixel2rect(int xpos);
  double h_pixel2rect(int ypos);
  void process_sub_image(int rank, int maxThreads, ColorGradient &gradient);
public:
  Mandelbrot_fractal(int width, int height, ColorGradient &gradient);
};

#endif // MANDELBROT_FRACTAL_H
