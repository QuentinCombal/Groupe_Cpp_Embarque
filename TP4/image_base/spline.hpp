#ifndef SPLINE_HPP
#define SPLINE_HPP
// Copyright [2020] <Quentin Combal>
#include <Eigen/Dense>
#include <cstdlib>
#include <iostream>
#include <vector>

class Spline{
 private:
  int size_;
  std::vector<double> x_;
  std::vector<double> y_;
  Eigen::VectorXd u_;

 public:
  Spline(const std::vector<double> x_values,
         const std::vector<double> y_values):
         size_(x_values.size()), x_(x_values), y_(y_values) {
    Eigen::MatrixXd M(3*size_, 3*size_);
    Eigen::VectorXd v(3*size_);
    Eigen::Matrix3d temp_bloc(3, 3);

    M.setZero();
    v.setZero();

    int i, j;
    double delta_x;

    for ( i = 0; i < size_; ++i ) {
      j = 3*i;

      // Important blocks
      if (i != size_-1)
        delta_x = x_values[i + 1] - x_values[i];
      else
        delta_x = 1 - x_values[i];

      // First important bloc
      if ( i < size_-1 ) {
        temp_bloc <<  delta_x*delta_x*delta_x, delta_x*delta_x,  delta_x,
                            3*delta_x*delta_x,       2*delta_x,        1,
                                    6*delta_x,               2,        0;
      } else {
        temp_bloc <<  delta_x*delta_x*delta_x, delta_x*delta_x,  delta_x,
                                            0,               0,        0,
                            3*delta_x*delta_x,       2*delta_x,        1;
      }
      M.block<3, 3>(j, j)  = temp_bloc;

      // Second important bloc
      // Add it unless we're on the last row
      if ( i < size_-1 ) {
        temp_bloc << 0,  0,  0,
                     0,  0, -1,
                     0, -2,  0;
        M.block<3, 3>(j, j + 3) = temp_bloc;
      }

      if (i != size_ - 1)
        v(j) = y_values[i+1] - y_values[i];
      else
        v(j) = y_values[0] - y_values[i];
    }

    // Solve u_
    u_ = M.fullPivHouseholderQr().solve(v);

    // std::cout << M << "\n";
    // std::cout << v << "\n";
  }

  float get_value(double x) const {

    if (x <= x_[0])
      x = x_[0];
    else if (x >= 1.0)
      x = 1.0;

    int i = 0;
    while (x > x_[i+1] && i < size_-1) ++i;


    double delta_x;
    double val;

    delta_x = x - x_[i];
    val = u_[3*i]*delta_x*delta_x*delta_x;
    val += u_[3*i+1]*delta_x*delta_x;
    val += u_[3*i+2]*delta_x;
    val += y_[i];
    val = ((u_[3*i]*delta_x + u_[3*i+1])*delta_x +  u_[3*i+2])*delta_x + y_[i];
    return val;
  }
  ~Spline() = default;
};

#endif // SPLINE_HPP
