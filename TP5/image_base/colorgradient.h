#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H
#include <vector>
#include <QColor>

class ColorGradient
{
private:
  std::vector<QRgb> colorVector_;
public:
  ColorGradient(int nColors, std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb);
  QRgb operator[](int i);
  int size();
  int clipRgb(int value);
};

#endif // COLORGRADIENT_H
