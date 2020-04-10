// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation.cpp
//
// \brief     low level function for image decimation
//
// \legal     Copyright (c) 2020
//
// \author    Quentin 
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#include "./util.h"
#include "./decimation.h"
#include <iostream>


SimpleDecimation::SimpleDecimation(uint nbThreads) : Decimation{nbThreads}{}


void SimpleDecimation::horizontal_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {
  //
  uint y_start = thread_id*dst_height/nbThreads_;
  uint y_stop = (thread_id+1)*dst_height/nbThreads_;

  uint16_t *src_col0 = src_ptr+y_start*src_stride;
  uint16_t *dst_col0 = dst_ptr+y_start*dst_stride;

  for (uint y = y_start; y < y_stop; ++y) {
    uint16_t *src_row = src_col0;
    uint16_t *dst_row = dst_col0;

    for (uint x = 0; x < dst_width; ++x) {
      *dst_row = *src_row;
      dst_row++;
      src_row +=2;
    }
    src_col0 += src_stride;
    dst_col0 += dst_stride;
  }
}

void SimpleDecimation::vertical_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {
  //
  uint x_start = thread_id*dst_width/nbThreads_;
  uint x_stop = (thread_id+1)*dst_width/nbThreads_;
  
  uint16_t *src_row0 = src_ptr+x_start;
  uint16_t *dst_row0 = dst_ptr+x_start;

  for (uint x = x_start; x < x_stop; ++x) {
    uint16_t *src_col = src_row0;
    uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
      *dst_col = *src_col;
      dst_col += dst_stride;
      src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }
}
