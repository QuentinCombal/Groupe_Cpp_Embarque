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
  static int clipRgb(int value);
  static float clipSpline(float value);
  static inline float xPixelToCoord(int xpos);
  static inline int xCoordToPixel(float x);
  static inline float yPixelToCoord(int ypos);
  static inline int yCoordToPixel(float y);

public:
  splineImage(std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb);
};

#endif // SPLINEIMAGE_H
