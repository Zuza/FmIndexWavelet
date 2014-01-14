#ifndef _FM_INDEX_H_
#define _FM_INDEX_H_

#include "RankedBitmap.hpp"
#include "WaveletTree.hpp"

#include <unordered_map>

typedef unsigned long long ullint;

class FmIndex {
public:
  // Warning: garbles data
  FmIndex(char* data, ullint data_sz, const char* alphabet, int alphabet_sz);

  // counts the number of occurences of the query as a substring within the text
  ullint count_substrings(char* query, int query_len);

private:
  char alphabet_map[256]; 
  ullint data_sz;

  char first_char;
  ullint first_sa_idx;

  vector<ullint> prefix_sums;
  WaveletTree wavelet;

  unordered_map<ullint, ullint> sparse_sa;
};

#endif // !_FM_INDEX_H_
