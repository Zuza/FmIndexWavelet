#include "FmIndex.hpp"

#include <divsufsort.h>

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

const int kFmSparseSaValM1 = 7;

FmIndex::FmIndex(char* data, ullint data_sz, const char* alphabet, int alphabet_sz) {
  this->data_sz = data_sz;

  memset(alphabet_map, -1, sizeof alphabet_map);
  for (int i = 0; i < alphabet_sz; ++i) {
    alphabet_map[(int)alphabet[i]] = i;
  }

  prefix_sums.resize(alphabet_sz+1, 0);
  for (ullint data_i = 0; data_i < data_sz; ++data_i) {
    int val = alphabet_map[(int)data[data_i]];
    assert(val != -1);
    data[data_i] = val;
    ++prefix_sums[val+1];
  }

  for (int i = 0; i < alphabet_sz; ++i)
    prefix_sums[i+1] += prefix_sums[i];

  // ONLY SUPPORTS 32bit sizes!!!
  vector<int> SA(data_sz);
  assert(sizeof(saidx_t) == sizeof(SA[0]));
  int ret_val = divsufsort((sauchar_t*)data, (saidx_t*)SA.data(), data_sz);
  assert(ret_val == 0);

  // construct BWT
  vector<char> bwt(data_sz);
  for (ullint i = 0; i < data_sz; ++i) { // TODO: optimize for cache (if needed!)
    int idx = SA[i] - 1;
    char val;
    if (idx == -1) {
      val = data[data_sz - 1];
      first_char = data[0];
      first_sa_idx = i;
    } else {
      val = data[idx];
    }
    bwt[i] = val;      
  }

  wavelet.init(bwt.data(), data_sz, alphabet_sz);
}

// counts the number of occurences of the query as a substring within the text
ullint FmIndex::count_substrings(char* query, int query_len) {
  int last_val = alphabet_map[(int)query[query_len - 1]];
  ullint lo = prefix_sums[last_val], hi = prefix_sums[last_val + 1];

  for (int iter = query_len - 2; iter >= 0; --iter) {
    int val = alphabet_map[(int)query[iter]];
    lo = prefix_sums[val] + wavelet.get_rank(val, lo) + (val == first_char && lo <= first_sa_idx);
    hi = prefix_sums[val] + wavelet.get_rank(val, hi) + (val == first_char && hi <= first_sa_idx);
  }
  return hi - lo;
}

void get_substring_pos(vector<llint> &results, char* query, int query_len) {
  assert(false);
}
