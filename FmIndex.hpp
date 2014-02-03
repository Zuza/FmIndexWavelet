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

  // read index from (binary) file
  FmIndex(FILE* in);

  // counts the number of occurences of the query as a substring within the text
  ullint count_substrings(char* query, int query_len);

  // returns the positions where query occurs in the text (at most limit results are returned)
  // the results do NOT have to be sorted
  void get_substring_pos(vector<ullint> &results, char* query, int query_len, int limit = 100);

  void serialize(FILE* out) const;
  void deserialize(FILE* in);

private:

  // classical LF mapping
  ullint lf_mapping(ullint pos);

  pair<ullint, ullint> get_substring_idx_bounds(char* query, int query_len);

  char alphabet_map[256]; 
  ullint data_sz;

  vector<ullint> prefix_sums;
  WaveletTree wavelet;

  unordered_map<ullint, ullint> sparse_sa;
};

#endif // !_FM_INDEX_H_
