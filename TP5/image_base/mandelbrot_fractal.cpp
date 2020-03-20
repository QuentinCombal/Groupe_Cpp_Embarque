#include "mandelbrot_fractal.h"

const int NMAX = 512;
const int ZMAX = 256;
const int nThreads = 8;



Mandelbrot_fractal::Mandelbrot_fractal(int width,int height, const double xc, const double yc, const double d, const bool julia, ColorGradient &gradient):
  QImage{width, height, QImage::Format_RGB32}, width_{width}, height_{height}
{
  //Create painter and fill image with a black background
  QPainter painter(this);
  painter.fillRect(this->rect(), Qt::black);

  std::vector<std::thread> threads;
  // Create threads for data parallelism
  for (int rank = 0; rank < nThreads; ++rank) {
      threads.emplace_back([=, &gradient]() {
        process_sub_image(rank, nThreads, gradient, xc, yc, d, julia);
      });
    }

  //Join threads
  for (auto &thread: threads) {
    thread.join();
  }
}

void Mandelbrot_fractal::process_sub_image(int rank, int maxThreads, ColorGradient &gradient, float xc, float yc, float d, bool julia){

  double x0,y0, xz, yz, xz_temp, v, x, y;
  double xz2, yz2;
  int n, i, threshold;

  // Start and stop indexes for data parallelism
  int xpos_start = rank*(width_)/maxThreads;
  int xpos_stop  = (rank+1)*(width_)/maxThreads;

  for (int xpos = xpos_start; xpos < xpos_stop; ++xpos) {
    x = h_pixel2rect(xpos, xc, d);
    for (int ypos = 0; ypos < height_; ++ypos) {
      y = v_pixel2rect(ypos, yc, d);

      // Determine if (x0 + i*y0) is in the Mandelbrot set
      xz = julia ? x : 0;
      yz = julia ? y : 0;
      x0 = julia ? -0.4 : x;
      y0 = julia ? 0.6  : y;
      xz2 = xz*xz;
      yz2 = yz*yz;
      n = 0;
      threshold = ZMAX*ZMAX;
      while ((xz2+yz2<threshold) && (++n<NMAX)) {
          // Compute z = z*z +c0
          xz_temp = xz2 - yz2 + x0;
          yz = 2*xz*yz + y0;
          xz = xz_temp;
          // Compute squares
          xz2 = xz*xz;
          yz2 = yz*yz;
      }
      if (n < NMAX){
        v = log2(log2(xz2+yz2));
        i = 1024*sqrt(5+n-v);
        if (i >= 2048)
          i = i%gradient.size();
        this->setPixel(xpos,ypos,gradient[i]);
      }
    }
  }
}

double Mandelbrot_fractal::h_pixel2rect(int xpos, float xc, float d){
  return xpos*3.0*d/(width_-1) + xc - 1.5*d;
}

double Mandelbrot_fractal::v_pixel2rect(int ypos, float yc, float d){
  return (1-ypos/(height_-1.0))*2.0*d + yc-d;
}
