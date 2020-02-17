#include <cstdlib>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

class Spline{
 private:
  std::vector<double> x_;
  std::vector<double> u_;
 public:
  Spline(const std::vector<double> x_values,
         const std::vector<double> y_values) {
    int size = x_values.size();
    x_ = x_values;
    Eigen::MatrixXd M(size, size);
    Eigen::VectorXd v(size);
    Eigen::Matrix3d bloc1(3, 3);
    Eigen::Matrix3d bloc2(3, 3);
    Eigen::Matrix3d bloc_of_zeroes(3, 3);

    bloc_of_zeroes << 0, 0, 0, 0, 0, 0, 0, 0, 0;

    int i, j, k;
    double delta_x;
    double delta_x_previous;
    for ( j = 0; j < 3*size; j+=3 ) {
      i = j/3;
      // Fill with zeroes before the important blocks
      for ( k = 0; k < 3*(i-1); k+=3)
        M.block<3, 3>(j, k) = bloc_of_zeroes;
      
      // Important blocks

      // Fill with zeroes before the important blocks
      for ( k = 3*(i+1); k < 3*(size-i-1); k+=3 )
        M.block<3, 3>(j, k) = bloc_of_zeroes;
    }
  }

  ~Spline();
};
