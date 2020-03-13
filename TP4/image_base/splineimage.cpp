#include "splineimage.h"
#include "spline.hpp"
#include <QtWidgets>

const int image_height = 512;
const int image_width = 1024;
const double xmax = 1.05;
const double xmin = -0.05;
const double ymax = 265.0;
const double ymin = -10.0;

splineImage::splineImage(std::vector<double> xs, std::vector<double> yr, std::vector<double> yg, std::vector<double> yb):
  QImage{image_width, image_height, QImage::Format_RGB32}
{
  //Create painter
  QPainter painter(this);

  //Generate a spline for each color
  Spline redSpline(xs,yr);
  Spline greenSpline(xs,yg);
  Spline blueSpline(xs,yb);

  //Plot the splines and color the background
  float red,green,blue,x;
  int redPos, greenPos, bluePos, xpos, ypos;

  for (xpos = 0; xpos < image_width; ++xpos) {
    //Get color values from the spline interpolations
    x = xPixelToCoord(xpos);
    red = redSpline.get_value(x);
    green = greenSpline.get_value(x);
    blue = blueSpline.get_value(x);

    //Draw a vertical line with the corresponding color
    painter.setPen(qRgb(clipRgb(red),
                        clipRgb(green),
                        clipRgb(blue)));
    painter.drawLine(xpos,0,xpos,image_height-1);

    //Plot the splines
    redPos   = yCoordToPixel(clipSpline(red)  );
    greenPos = yCoordToPixel(clipSpline(green));
    bluePos  = yCoordToPixel(clipSpline(blue));
    this->setPixel(xpos,redPos,qRgb(255, 0, 0));
    this->setPixel(xpos,greenPos,qRgb(0, 255, 0));
    this->setPixel(xpos,bluePos,qRgb(0, 0, 255));
  }

  //Draw white vertical line at xs values
  painter.setPen(qRgb(255,255,255));
  for (double x : xs) {
    xpos = xCoordToPixel(x);
    painter.drawLine(xpos,0,xpos,image_height-1);
  }
  xpos = xCoordToPixel(1.0);
  painter.drawLine(xpos,0,xpos,image_height-1);

  //Draw white horizontal dashed lines at y=0 and y=255
  QPen pen(QColor(255,255,255));
  QVector<qreal> dashes;
  dashes << 2 << 2;
  pen.setDashPattern(dashes);
  painter.setPen(pen);

  ypos = yCoordToPixel(0);
  painter.drawLine(0,ypos,image_width-1,ypos);
  ypos = yCoordToPixel(255);
  painter.drawLine(0,ypos,image_width-1,ypos);
}

inline float splineImage::xPixelToCoord(int xpos){
  return (xmax - xmin)*xpos/(image_width-1) + xmin;
}

inline int splineImage::xCoordToPixel(float x){
  return (x-xmin)*(image_width-1)/(xmax-xmin);
}

inline float splineImage::yPixelToCoord(int ypos){
  return (ymax -ymin)*(1 - ypos/(image_height-1)) + ymin;
}

inline int splineImage::yCoordToPixel(float y){
  return (1 - (y-ymin)/(ymax-ymin))*(image_height-1);
}

int splineImage::clipRgb(int value){
  if (value < 0)
    return 0;
  if (value > 255)
    return 255;
  return value;
}

float splineImage::clipSpline(float value){
  if (value < ymin)
    return ymin;
  if (value > ymax)
    return ymax;
  return value;
}
