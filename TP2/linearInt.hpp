#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class linearInt {
private:
  std::vector<double> m_Xs;
  std::vector<double> m_As;
  std::vector<double> m_Bs;
public:
  linearInt(const std::vector<double> axe_x, const std::vector<double> axe_y) {
    int taille = axe_x.size();
    this -> m_Xs = axe_x;
    for(int N = 0; N < taille-1; ++N) {
      double x1 = axe_x[N];
      double x2 = axe_x[N+1];
      double y1 = axe_y[N];
      double y2 = axe_y[N+1];
      this-> m_As.push_back((y2-y1)/(x2-x1));
      this-> m_Bs.push_back(y1);
    };
  }

  float get_value(const double x) const {
    int indice = 0;
    while(x > m_Xs[indice+1] && indice < m_Xs.size()){
      std::cout << "indice: " << indice << std::endl;
      std::cout << "m_Xs[indice]: " << m_Xs[indice] << std::endl;
      ++indice;
    }
    std::cout << "m_As[indice]" << m_As[indice] << std::endl;
    return (m_As[indice]*x + m_Bs[indice]);
  }
};
