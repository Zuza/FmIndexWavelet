#include "RankedBitmap.hpp"

#include <algorithm>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>

using namespace std;
 
typedef unsigned long long ullint;

const int kBitmapSuperbucketPow = 16; // shoule be <= 16, because of unsigned short int
const int kBitmapSuperbucketValM1 = (1 << kBitmapSuperbucketPow) - 1;

const int kBitmapBucketPow = 4;
const int kBitmapBucketValM1 = (1 << kBitmapBucketPow) - 1;

RankedBitmap::RankedBitmap() {}

RankedBitmap::RankedBitmap(const char* data, ullint data_len, ullint n_elements, int zero_lo, int one_lo, int one_hi) {
  init(data, data_len, n_elements, zero_lo, one_lo, one_hi);
}

void RankedBitmap::init(const char* data, ullint data_len, ullint n_elements, int zero_lo, int one_lo, int one_hi) {
  this->n_elements = n_elements;
  bitmask_sz = (n_elements + 7) >> 3; // ceil(n/8)
  bitmask.resize(bitmask_sz, 0);

  ullint pos = 0;
  ullint last_superbucket_0rank = 0;
  ullint zero_rank = 0;

  superbuckets.resize((n_elements + kBitmapSuperbucketValM1) >> kBitmapSuperbucketPow);
  buckets.resize((n_elements + kBitmapBucketValM1) >> kBitmapBucketPow);

  ullint superbucket_idx = 0;
  ullint bucket_idx = 0;

  for (ullint data_i = 0; data_i < data_len; ++data_i) {
    if (zero_lo <= data[data_i] && data[data_i] < one_hi) {

      if ((pos&kBitmapSuperbucketValM1) == 0) { // next superbucket
        last_superbucket_0rank = zero_rank;
        superbuckets[superbucket_idx++] = last_superbucket_0rank;
      }

      if ((pos&kBitmapBucketValM1) == 0) { // next bucket
        unsigned short val = (zero_rank - last_superbucket_0rank);
        assert((ullint)val == (zero_rank - last_superbucket_0rank));
        buckets[bucket_idx++] = val;
      }

      if (zero_lo <= data[data_i] && data[data_i] < one_lo) { // 0
        ++zero_rank;
      } else { // 1
        bitmask[pos >> 3] |= 1<<(pos&7);
      }

      ++pos;
    }
  }

  assert(pos == n_elements);
  assert(superbucket_idx == (ullint)superbuckets.size());
  assert(bucket_idx == (ullint)buckets.size());
}

// get bit in position pos, pos should be in [0, n_elements)
bool RankedBitmap::get_bit(ullint pos) {
  return (bitmask[pos >> 3] >> (pos&7)) & 1;
}

// if bit == 0: how many 0's in [0, 1, 2, ..., pos)
// else         how many 1's in [0, 1, 2, ..., pos)
ullint RankedBitmap::get_rank(bool bit, ullint pos) {
  ullint rank = 
    + superbuckets[pos >> kBitmapSuperbucketPow]
    + buckets[pos >> kBitmapBucketPow];
  for (ullint i_pos = pos&(~kBitmapBucketValM1); i_pos < pos; ++i_pos) { // TODO: switch to popcount!!!
    rank += (get_bit(i_pos) == 0);
  }
  return (bit == 0) ? (rank) : (pos-rank);
}
