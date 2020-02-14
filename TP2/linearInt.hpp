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
    double x1, x2;
    double y1, y2;
    for(int N = 0; N < taille-1; ++N) {
      x1 = axe_x[N];
      x2 = axe_x[N+1];
      y1 = axe_y[N];
      y2 = axe_y[N+1];
      this-> m_As.push_back((y2-y1)/(x2-x1));
      this-> m_Bs.push_back(y1);
    }
    this-> m_As.push_back((axe_y[0]-y2)/(1-x2));
    this-> m_Bs.push_back(y2);
  }

  float get_value(const double x) const {
    int indice = 0;
    while(x > m_Xs[indice+1] && indice < m_Xs.size()-1){
      ++indice;
      // std::cout << "indice: " << indice << std::endl;
      // std::cout << "m_Xs[indice]: " << m_Xs[indice];
    }
    // std::cout << std::endl << "m_As[indice]" << m_As[indice] << std::endl
    //                        << "m_Bs[indice]" << m_Bs[indice] << std::endl;
    return (m_As[indice]*(x-m_Xs[indice]) + m_Bs[indice]);
  }
};
