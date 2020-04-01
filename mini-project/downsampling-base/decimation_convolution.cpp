// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation_convolution.cpp
//
// \brief     low level function for image decimation
//
// \legal     Copyright (c) 2020
//
// \author    Bernard
//
////////////////////////////////////////////////////////////////////////////////
// header-end


#include "./util.h"
#include "./decimation.h"

const int fir_coeff[7] = {-3544, 0, 11134, 17588, 11134, 0, -3544};


ConvolutionDecimation::ConvolutionDecimation(uint nbThreads) : Decimation{nbThreads}{}

void ConvolutionDecimation::horizontal_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {
  //
  uint y_start = thread_id*dst_height/nbThreads_;
  uint y_stop = (thread_id+1)*dst_height/nbThreads_;

  uint16_t *src_col0 = src_ptr+y_start*src_stride;
  uint16_t *dst_col0 = dst_ptr+y_start*dst_stride;

  int idx, k;
  int32_t sum, data;
  int16_t convoluted_val;
  int max_idx = 2*dst_width-1;
  for (uint y = y_start; y < y_stop; ++y) {
    // uint16_t *src_row = src_col0;
    // uint16_t *dst_row = dst_col0;

    for (uint x = 0; x < dst_width; ++x) {
      k = 2*x;
      sum = 2 << 14;
      for (int8_t i = -3; i <= 3; ++i) {
        idx = k + i;
        // clamp to image borders
        idx = (idx <= 0) ? 0 : ((idx >= max_idx) ? max_idx : idx);
        data = static_cast<int32_t>(src_col0[idx]);
        sum += data * fir_coeff[i+3];
      }
      convoluted_val = sum >> 15;
      convoluted_val = (convoluted_val > 1023) ?  1023 :
                       (convoluted_val < 0) ? 0 : convoluted_val;
      dst_col0[x] = convoluted_val;
      // *dst_row = *src_row;
      // dst_row++;
      // src_row +=2;
    }
    src_col0 += src_stride;
    dst_col0 += dst_stride;
  }
}

void ConvolutionDecimation::vertical_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {
    //
  uint x_start = thread_id*dst_width/nbThreads_;
  uint x_stop = (thread_id+1)*dst_width/nbThreads_;

  uint16_t *src_row0 = src_ptr+x_start;
  uint16_t *dst_row0 = dst_ptr+x_start;

  int k, idy;
  int32_t sum;
  int16_t convoluted_val;
  int max_idy = (2*dst_height-1)*src_stride;
  for (uint x = x_start; x < x_stop; ++x) {
    // uint16_t *src_col = src_row0;
    // uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
      k = 2*y;
      sum = 2 << 14;
      for (int i = -3; i <= 3; ++i) {
        idy = (k+i)*dst_stride;
        // clamp to image borders
        idy = (idy <= 0) ? 0 : ((idy >= max_idy) ? max_idy : idy);
        int32_t data = static_cast<int32_t>(src_row0[idy]);
        sum += data * fir_coeff[i+3];
      }
      convoluted_val = sum >> 15;
      convoluted_val = (convoluted_val > 1023) ?  1023 :
                       (convoluted_val < 0) ? 0 : convoluted_val;
      dst_row0[y*dst_stride] = convoluted_val;
      // *dst_col =convoluted_val;

      // *dst_col = *src_col;
      // dst_col += dst_stride;
      // src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }
}