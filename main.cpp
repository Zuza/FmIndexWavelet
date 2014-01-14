#include "RankedBitmap.hpp"
#include "WaveletTree.hpp"
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

#include <unordered_map>

using namespace std;

typedef unsigned long long ullint;

int main() {
  const char alpha[] = "ACGT"; int alpha_sz = strlen(alpha);
  const int data_len = 123;
  static char data[data_len+123];
  static char orig_data[data_len+123];

  mt19937 mt;
  for (int i = 0; i < data_len; ++i)
    data[i] = alpha[mt() % alpha_sz];
  data[data_len] = 0;
  strncpy(orig_data, data, data_len);
  orig_data[data_len] = 0;

  FmIndex fmindex(data, data_len, alpha, alpha_sz);

  auto slow_count_substrings = [&orig_data, data_len](char* query, int query_len) {
    ullint ret = 0;
    for (int i = 0; i < data_len; ++i)
      ret += (strncmp(orig_data+i, query, query_len) == 0);
    return ret;
  };

  const int numQueries = 1e3;
  const int query_len = 3;

  for (int i_q = 0; i_q < numQueries; ++i_q) {
    static char query[query_len + 123];

    for (int i = 0; i < query_len; ++i)
      query[i] = alpha[mt() % alpha_sz];
    query[query_len] = 0;
    
    ullint cnt = fmindex.count_substrings(query, query_len);
    ullint cnt_brute = slow_count_substrings(query, query_len);

    printf("cnt = %llu\n", cnt);
    assert(cnt == cnt_brute);
  }

  return 0; 
}
