// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation.h
//
// \brief     low level function for image decimation
//
// \legal     Copyright (c) 2020
//
// \author    Quentin Combal 
//
////////////////////////////////////////////////////////////////////////////////
// header-end
#ifndef DECIMATION_H
#define DECIMATION_H

#include <thread>
#include <vector>

class Decimation{
 protected:
  uint nbThreads_;
 public:
  Decimation(uint nbThreads) : nbThreads_{nbThreads}{}
  ~Decimation() = default;

//! \brief
//!  This function performs the horizontal decimation by 2 of a
//!  source component (16 bits per pixels) into a destination.
//!
//!       source          destination
//!       2*width           width
//!     +--------------+   +------+
//!     |              |   |      |
//!     |              |   |      |
//!     +--------------+   +------+
//!
virtual void
horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                       uint dst_width, uint dst_height,
                       uint dst_stride, uint src_stride,
                       uint thread_id) = 0;

//! \brief
//!  This function performs the vertical decimation by 2 of a source component (16 bits per pixels)
//!  into a destination.
//!
//!       source          destination
//!        width           width
//!       +------+       +------+
//!       |      |       |      |   height
//!       |      |       |      |
//!       |      |       +------+
//!       |      |
//!       |      |  2*height
//!       +------+
//!
virtual void
vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride,
                    uint thread_id) = 0;


  void 
  do_horizontal(uint16_t *dst_ptr, uint16_t *src_ptr,
                            uint dst_width, uint dst_height,
                            uint dst_stride, uint src_stride){
    //
    std::vector<std::thread> threads;
    
    for (uint id = 0; id < nbThreads_; ++id) {
      threads.emplace_back([=]() {
        horizontal_decimation(
          dst_ptr, src_ptr,
          dst_width, dst_height,
          dst_stride, src_stride,
          id);
      });
    }
  
    for (auto &thread_elem : threads) {
      thread_elem.join();
    }
  }

  void 
  do_vertical(uint16_t *dst_ptr, uint16_t *src_ptr,
                            uint dst_width, uint dst_height,
                            uint dst_stride, uint src_stride){
    //
    std::vector<std::thread> threads;
    
    for (uint id = 0; id < nbThreads_; ++id) {
      threads.emplace_back([=]() {
        vertical_decimation(
          dst_ptr, src_ptr,
          dst_width, dst_height,
          dst_stride, src_stride,
          id);
      });
    }
  
    for (auto &thread_elem : threads) {
      thread_elem.join();
    }
  }
};


//////////////////////////////////////////////////////////////////////////////

//
//
// Derived classes definition
//
//



/////////// SimpleDecimation ///////////

class SimpleDecimation : public Decimation {
public:
  SimpleDecimation(uint nbThreads);
  ~SimpleDecimation();

  void
  horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                         uint dst_width, uint dst_height,
                         uint dst_stride, uint src_stride,
                         uint thread_id) override;

  void
  vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                      uint dst_width, uint dst_height,
                      uint dst_stride, uint src_stride,
                      uint thread_id) override;
};


/////////// ConvolutionDecimation ///////////

class ConvolutionDecimation : public Decimation {
public:
  ConvolutionDecimation(uint nbThreads);
  ~ConvolutionDecimation();

  void
  horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                         uint dst_width, uint dst_height,
                         uint dst_stride, uint src_stride,
                         uint thread_id) override;

  void
  vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                      uint dst_width, uint dst_height,
                      uint dst_stride, uint src_stride,
                      uint thread_id) override;
};



/////////// VectorizedDecimation ///////////

class VectorizedDecimation : public Decimation {
public:
  VectorizedDecimation(uint nbThreads);
  ~VectorizedDecimation();

  void
  horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                         uint dst_width, uint dst_height,
                         uint dst_stride, uint src_stride,
                         uint thread_id) override;

  void
  vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                      uint dst_width, uint dst_height,
                      uint dst_stride, uint src_stride,
                      uint thread_id) override;
};



#endif // DECIMATION_H