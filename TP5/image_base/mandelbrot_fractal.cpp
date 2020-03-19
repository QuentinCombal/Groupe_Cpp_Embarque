#include "mandelbrot_fractal.h"

const int NMAX = 512;
const float xc = -0.5;
//const float xc = -1.7685736562992577;
const float yc = 0;
//const float yc = -0.000964296850972570525;
const float d_ = 1;
//const float d_ = 0.0002;
const int nThreads = 8;



Mandelbrot_fractal::Mandelbrot_fractal(int width,int height, ColorGradient &gradient):
  QImage{width, height, QImage::Format_RGB32}, width_{width}, height_{height}
{
  //Create painter
  //QPainter painter(this);
  std::vector<std::thread> threads;

  // Create threads for data parallelism
  for (int rank = 0; rank < nThreads; ++rank) {
      threads.emplace_back([=, &gradient]() {
        process_sub_image(rank, nThreads, gradient);
      });
    }

  //Join threads
  for (auto &thread: threads) {
    thread.join();
  }
}

void Mandelbrot_fractal::process_sub_image(int rank, int maxThreads, ColorGradient &gradient){
  double x,y, v;
  std::complex<double> z;
  std::complex<double> c;
  int n, i;
  int xpos_start = rank*(width_)/maxThreads;
  int xpos_stop  = (rank+1)*(width_)/maxThreads;

  for (int xpos = xpos_start; xpos < xpos_stop; ++xpos) {
    x = h_pixel2rect(xpos);
    for (int ypos = 0; ypos < height_; ++ypos) {
      y = v_pixel2rect(ypos);
      //Set c to be x+iy
      c.real(x); c.imag(y);
      //Set z to 0;
      z.real(0); z.imag(0);

      //Determine if c is in the Mandelbrot set
      n = 0;
      int threshold = 256*256;
      while ((norm(z)<threshold) && (++n<NMAX)) {
          z =z*z+c;
      }
      if (n < NMAX){
        v = log2(log2(norm(z)));
        i = 1024*sqrt(5+n-v);
        if (i >= 2048)
          i = i%gradient.size();
        this->setPixel(xpos,ypos,gradient[i]);
      }
    }
  }
}

double Mandelbrot_fractal::h_pixel2rect(int xpos){
  return xpos*3.0*d_/(width_-1) + xc - 1.5*d_;
}

double Mandelbrot_fractal::v_pixel2rect(int ypos){
  return (1-ypos/(height_-1.0))*2.0*d_ + yc-d_;
}
