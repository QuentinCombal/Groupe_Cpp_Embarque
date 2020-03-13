#include <string>
#include <vector>
#include <iostream>
#include "spline.hpp"

using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  vector<double> xs{   0., 0.16, 0.42, .6425, 0.8575};
  vector<double> ys{ 100., 183., 235.,   40.,    15.};
  //std::vector<double> ys{0.  , 32. , 237., 215.  , 0.    };
  Spline splineInt(xs, ys);
  for (double x = 0.0 ; x <= 1.0 ; x += 0.1) {
    std::cout << "Value for " << x
              << " is " << splineInt.get_value(x)
              << std::endl;
  }
}
