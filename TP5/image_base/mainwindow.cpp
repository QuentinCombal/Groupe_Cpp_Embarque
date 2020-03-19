
#include "mainwindow.h"
#include "splineimage.h"
#include "mandelbrot_fractal.h"
#include "commify.hpp"
#include "colorgradient.h"
#include <iostream>
#include <string>
#include <QtWidgets>
#include </usr/local/include/eigen/Eigen/Dense>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //
  // construct image_widget

  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(true);
  image_widget_->setMinimumSize(300, 300);

  create_actions();
  create_menus();

  setCentralWidget(image_widget_);
  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

}


void MainWindow::create_actions() {
  //
  action_checker_board_image_ = new QAction(tr("Checker Board Image"), this);
  connect(action_checker_board_image_, SIGNAL(triggered()), this, SLOT(slot_load_checker_board_image()));

  action_color_triangle_image_ = new QAction(tr("Color Triangle Image"), this);
  connect(action_color_triangle_image_, SIGNAL(triggered()), this, SLOT(slot_load_color_triangle_image()));

  action_spline_image_ = new QAction(tr("Spline Image"), this);
  connect(action_spline_image_, SIGNAL(triggered()), this, SLOT(slot_load_spline_image()));

  action_mandelbrot_fractal_ = new QAction(tr("Mandelbrot Fractal"), this);
  connect(action_mandelbrot_fractal_, SIGNAL(triggered()), this, SLOT(slot_load_mandelbrot_fractal()));

  action_test_gradient_ = new QAction(tr("Test gradient"), this);
  connect(action_test_gradient_, SIGNAL(triggered()), this, SLOT(slot_load_test_gradient()));

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT(slot_exit()));

  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT(slot_about()));
}

void MainWindow::slot_exit() {
  close();
}

void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The user can select several images to be displayed on the screen</p>"));
}


void MainWindow::create_menus() {
  menu_open_ = new QMenu(tr("&Open"), this);

  menu_open_->addAction(action_checker_board_image_);
  menu_open_->addAction(action_color_triangle_image_);
  menu_open_->addAction(action_spline_image_);
  menu_open_->addAction(action_mandelbrot_fractal_);
  menu_open_->addAction(action_test_gradient_);
  menu_open_->addAction(action_exit_);


  menu_help_ = new QMenu(tr("&Help"), this);
  menu_help_->addAction(action_about_);

  menuBar()->addMenu(menu_open_);
  menuBar()->addMenu(menu_help_);
}



//
// Checker Board Triangle
//
const int checker_board_width = 600;
const int checker_board_height = 600;

void MainWindow::slot_load_checker_board_image() {

  // The ~10 lines of code below must be changed with your code 
  // Create a QImage of required size
  // Draw a simple black/white checker board

  QImage checker_board_image(checker_board_width, checker_board_height, QImage::Format_RGB32);
  QPainter painter(&checker_board_image);
  painter.fillRect(checker_board_image.rect(), Qt::black);
  for (int y = 0; y < checker_board_height; ++y) {
    for (int x = 0; x < checker_board_width; ++x) {
      if ((x^y) & 0x20) {
        checker_board_image.setPixel(x, y, qRgb(255, 255, 255));
      }
    }
  }


  //
  // You probably don't want to change these lines 
  //

  image_widget_->setPixmap(QPixmap::fromImage(checker_board_image));
  image_widget_->setFixedSize(checker_board_width, checker_board_height);
  adjustSize();
}



//
// sRGB Color Triangle
//

const int frame_size = 600;

void MainWindow::slot_load_color_triangle_image() {

  QImage color_triangle(frame_size, frame_size, QImage::Format_RGB32);
  QPainter painter(&color_triangle);
  // New pen with greenish color
  QPen pen(QColor(51,204,102));
  painter.setPen(pen);
  painter.fillRect(color_triangle.rect(), Qt::black);

  /*
   *
   * Draw square frame and dashed grid
   *
   */

  // Draw the square frame (solid lines)
  painter.drawRect(0,0,frame_size-1,frame_size-1);

  // Draw the dashed grid
  int grid_step = frame_size / 10;

  //Create and set the dash pattern
  QVector<qreal> dashes;
  dashes << 2 << 2;
  pen.setDashPattern(dashes);
  painter.setPen(pen);

  // Draw vertical dashed lines
  for (int x = grid_step; x < frame_size; x+=grid_step) {
    painter.drawLine(x,0,x,frame_size-1);
  }
  // Draw horizontal dashed lines
  for (int y = grid_step; y < frame_size; y+=grid_step) {
    painter.drawLine(0,y,frame_size-1,y);
  }


  /*
   *
   *  Draw triangle
   *
   */
  const float x_red   = 0.64;
  const float y_red   = 0.33;
  const float x_green = 0.3;
  const float y_green = 0.6;
  const float x_blue  = 0.15;
  const float y_blue  = 0.06;
//  const float x_white  = 0.3127;
//  const float y_white  = 0.3290;

  const QVector<QPoint> triangle_points = {
    QPoint(x_red*frame_size , (1-y_red)*frame_size),
    QPoint(x_green*frame_size, (1-y_green)*frame_size),
    QPoint(x_blue*frame_size, (1-y_blue)*frame_size)
  };
  QPolygon triangle(triangle_points);

  // coordinates of the smallest rectangle that contains the triangle
  const int start_x = 0.15*frame_size;
  const int start_y = (1- 0.6)*frame_size;
  const int stop_x = 0.64*frame_size;
  const int stop_y = (1-0.06)*frame_size;

  // Variables for barycentric coordinates calculation
  const float denominator = (y_green-y_blue)*(x_red-x_blue) + (x_blue-x_green)*(y_red-y_blue);
  float alpha, beta, gamma, x_std, y_std, red_value, green_value, blue_value;

  // Iterate over a rectangle containing the triangle
  for (int x = start_x; x < stop_x; ++x) {
    for (int y = start_y; y < stop_y; ++y) {
      // Check if (x,y) is in the triangle
      if(triangle.containsPoint(QPoint(x,y),Qt::OddEvenFill)){
        // Convert x and y to standard coordinates
        x_std = x/(float)frame_size;
        y_std = 1 - y/(float)frame_size;

        // Compute alpha, beta, and gamma according to the formula :
        // https://en.wikipedia.org/wiki/Barycentric_coordinate_system?oldformat=true#Conversion_between_barycentric_and_Cartesian_coordinates
        alpha = ((y_green-y_blue)*(x_std-x_blue) + (x_blue-x_green)*(y_std-y_blue))/denominator;
        beta  = ((y_blue-y_red)*(x_std-x_blue) + (x_red-x_blue)*(y_std-y_blue))/denominator;
        gamma = 1 - alpha - beta;

        // Determine values
        red_value = alpha*255;
        green_value = beta*255;
        blue_value = gamma*255;
        //white_correction =( 1 - sqrt((x-x_white)*(x-x_white) + (y-y_white)*(y-y_white)) *255);
        // Draw the pixel
        color_triangle.setPixel(x, y, qRgb(red_value, green_value, blue_value));
      }
    }
  }
  //color_triangle.setPixel(0.3127*frame_size, (1-0.32903)*frame_size, qRgb(255, 255, 255));

  //
  // You probably don't want to change these lines
  //

  image_widget_->setPixmap(QPixmap::fromImage(color_triangle));
  image_widget_->setFixedSize(frame_size, frame_size);
  adjustSize();
}

const int image_height = 512;
const int image_width = 1024;

void MainWindow::slot_load_spline_image(){
  std::vector<double> xs{0.  , 0.16, 0.42, 0.6425, 0.8575};
  std::vector<double> yr{0.  , 32. , 237., 215.  , 0.    };
  std::vector<double> yg{7.  , 107., 255., 170.  , 10.   };
  std::vector<double> yb{100., 183., 235., 40.   , 15.   };
  splineImage spline(xs,yr,yg,yb);
  image_widget_->setPixmap(QPixmap::fromImage(spline));
  image_widget_->setFixedSize(image_width, image_height);
  adjustSize();
}


void MainWindow::slot_load_mandelbrot_fractal(){
  const int fractal_width = 600;
  const int fractal_height = 400;
  //Color for the color gradient
  std::vector<double> xs{0.  , 0.16, 0.42, 0.6425, 0.8575};
  std::vector<double> yr{0.  , 32. , 237., 215.  , 0.    };
  std::vector<double> yg{7.  , 107., 255., 170.  , 10.   };
  std::vector<double> yb{100., 183., 235., 40.   , 15.   };
  //Construct the gradient
  ColorGradient gradient(2048,xs,yr,yg,yb);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::high_resolution_clock::now();
  Mandelbrot_fractal fractal(fractal_width,fractal_height, gradient);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_time = end - start;
  std::cout << "Info: image calculated in " << Commify(elapsed_time.count()*1E6)<< " µs" <<std::endl;

  image_widget_->setPixmap(QPixmap::fromImage(fractal));
  image_widget_->setFixedSize(fractal_width, fractal_height);
  adjustSize();
}

void MainWindow::slot_load_test_gradient(){
  //Color for the color gradient
  std::vector<double> xs{0.  , 0.16, 0.42, 0.6425, 0.8575};
  std::vector<double> yr{0.  , 32. , 237., 215.  , 0.    };
  std::vector<double> yg{7.  , 107., 255., 170.  , 10.   };
  std::vector<double> yb{100., 183., 235., 40.   , 15.   };
  //Construct the gradient
  ColorGradient gradient(2048,xs,yr,yg,yb);

  QImage test(1024,600, QImage::Format_RGB32);
  QPainter painter(&test);
  for (int xpos = 0; xpos < 1024; ++xpos) {
    painter.setPen(gradient[xpos*2]);
    painter.drawLine(xpos,0,xpos,599);
  }

  std::cout << gradient[0] << std::endl;
  std::cout << gradient[2047] << std::endl;

  image_widget_->setPixmap(QPixmap::fromImage(test));
  image_widget_->setFixedSize(1024, 600);
  adjustSize();
}
