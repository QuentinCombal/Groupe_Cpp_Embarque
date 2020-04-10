// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation_vectorized.cpp
//
// \brief     low level function for image decimation using intrinsics
//
// \legal     Copyright (c) 2020
//
// \author    Bernard
//
////////////////////////////////////////////////////////////////////////////////
// header-end


#include <tmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>


#include "./util.h"
#include "./decimation.h"


#if defined(__GNUC__)
#define ALIGN32(X) X __attribute__((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN32(X)  __declspec(align(32)) X
#else
#error "Error: not alignment directive"
#endif

const int16_t ALIGN32(mem_fir_coeff[8]) = {-3544, 0, 11134, 17588, 11134, 0, -3544, 0};

VectorizedDecimation::VectorizedDecimation(uint nbThread) : Decimation{nbThread}{}

void VectorizedDecimation::horizontal_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {
  //
  __m128i r128_a, r128_b, r128_r;
  
  uint y_start = thread_id*dst_height/nbThreads_;
  uint y_stop = (thread_id+1)*dst_height/nbThreads_;

  uint16_t *src_col0 = src_ptr+y_start*src_stride;
  uint16_t *dst_col0 = dst_ptr+y_start*dst_stride;

  uint16_t data[8];
  int32_t sum;
  int16_t convoluted_val;
  uint k;
  int idx, max_idx;
  
  max_idx = 2*dst_width-1;
  
  r128_a = _mm_load_si128((__m128i*)mem_fir_coeff);
  // r128_b = _mm_set1_epi32(0);
  data[8] = 0;

  for (uint y = y_start; y < y_stop; ++y) {
    for (uint x = 0; x < dst_width; ++x) {
      k = 2*x;
      for (int8_t i = -3; i <= 3; ++i) {
        idx = k + i;
        // clamp to image borders
        idx = (idx <= 0) ? 0 : ((idx >= max_idx) ? max_idx : idx);
        data[i+3] = src_col0[idx];
      }
      r128_b = _mm_loadu_si128((__m128i*)data);
      // r128_b = _mm_loadu_si128((__m128i*) (src_col0+k-3) );
      r128_r = _mm_madd_epi16(r128_a, r128_b);
      r128_r = _mm_hadd_epi32(r128_r, _mm_set1_epi32(0));
      sum = _mm_extract_epi32(r128_r, 1) + _mm_extract_epi32(r128_r, 0);
      sum += 2 << 14;

      convoluted_val = sum >> 15;
      convoluted_val = (convoluted_val > 1023) ?  1023 :
                       (convoluted_val < 0) ? 0 : convoluted_val;
      dst_col0[x] = convoluted_val;
    }
    src_col0 += src_stride;
    dst_col0 += dst_stride;
  }
}

void VectorizedDecimation::vertical_decimation
    (uint16_t *dst_ptr, uint16_t *src_ptr,
     uint dst_width, uint dst_height,
     uint dst_stride, uint src_stride,
     uint thread_id) {

  __m128i r128_a, r128_b, r128_r;

  uint x_start = thread_id*dst_width/nbThreads_;
  uint x_stop = (thread_id+1)*dst_width/nbThreads_;

  uint16_t *src_row0 = src_ptr+x_start;
  uint16_t *dst_row0 = dst_ptr+x_start;

  uint16_t data[8];
  int32_t sum;
  int16_t convoluted_val;
  uint k;
  int idy, max_idy;

  max_idy = (2*dst_height-1)*src_stride;

  r128_a = _mm_load_si128((__m128i*)mem_fir_coeff);
  data[8] = 0;

  for (uint x = x_start; x < x_stop; ++x) {
    for (uint y = 0; y < dst_height; ++y) {
      k = 2*y;
      for (int i = -3; i <= 3; ++i) {
        idy = (k+i)*dst_stride;
        // clamp to image borders
        idy = (idy <= 0) ? 0 : ((idy >= max_idy) ? max_idy : idy);
        data[i+3] = src_row0[idy];
      }
      r128_b = _mm_loadu_si128((__m128i*)data);
      r128_r = _mm_madd_epi16(r128_a, r128_b);
      r128_r = _mm_hadd_epi32(r128_r, _mm_set1_epi32(0));
      sum = _mm_extract_epi32(r128_r, 1) + _mm_extract_epi32(r128_r, 0);
      sum += 2 << 14;
      convoluted_val = sum >> 15;
      convoluted_val = (convoluted_val > 1023) ?  1023 :
                       (convoluted_val < 0) ? 0 : convoluted_val;
      dst_row0[y*dst_stride] = convoluted_val;
    }
    src_row0++;
    dst_row0++;
  }
}