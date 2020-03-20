#include "colorgradient.h"
#include "spline.h"

ColorGradient::ColorGradient(int nColors, std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb)
{
  Clamp<int> clampRgb{0,255};

  Spline redSpline(xs,yr);
  Spline greenSpline(xs,yg);
  Spline blueSpline(xs,yb);

  double x;
  for (int i = 0; i < nColors; ++i) {
    x = -0.05 + 1.1*i/nColors;
    this->push_back(qRgb( clampRgb(redSpline.get_value(x))  ,
                                 clampRgb(greenSpline.get_value(x)),
                                 clampRgb(blueSpline.get_value(x)) ));
  }
}

//int ColorGradient::clampRgb(int value){

//  return (value <= 0) ? 0 : ((value >= 255) ? 255 : value);
//}
