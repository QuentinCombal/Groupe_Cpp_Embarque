#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H
#include <vector>
#include <QColor>
#include "elec4_util.h"

class ColorGradient: public std::vector<QRgb>
{
public:
  ColorGradient(int nColors, std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb);
};

#endif // COLORGRADIENT_H
