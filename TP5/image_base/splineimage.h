#ifndef SPLINEIMAGE_H
#define SPLINEIMAGE_H

#include <QObject>
#include <QImage>

class splineImage : public QImage
{
private:
  std::vector<double> xs;
  std::vector<double> yr;
  std::vector<double> yg;
  std::vector<double> yb;
  static inline double xPixelToCoord(int xpos);
  static inline int xCoordToPixel(double x);
  static inline double yPixelToCoord(int ypos);
  static inline int yCoordToPixel(double y);

public:
  splineImage(std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb);
};

#endif // SPLINEIMAGE_H
