#include "colorgradient.h"
#include "spline.hpp"

ColorGradient::ColorGradient(int nColors, std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb)
{
  Spline redSpline(xs,yr);
  Spline greenSpline(xs,yg);
  Spline blueSpline(xs,yb);

  double x;
  for (int i = 0; i < nColors; ++i) {
    x = -0.05 + 1.1*i/nColors;
    colorVector_.push_back(qRgb( clipRgb(redSpline.get_value(x))  ,
                                 clipRgb(greenSpline.get_value(x)),
                                 clipRgb(blueSpline.get_value(x)) ));
  }
}

QRgb ColorGradient::operator[](int i){
  return colorVector_[i];
}

int ColorGradient::size(){
  return colorVector_.size();
};

int ColorGradient::clipRgb(int value){
  if (value < 0)
    return 0;
  if (value > 255)
    return 255;
  return value;
}
