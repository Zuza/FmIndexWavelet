#ifndef _WAVELET_TREE_H_
#define _WAVELET_TREE_H_

#include "RankedBitmap.hpp"

#include <vector>
#include <divsufsort.h>

using namespace std;

typedef unsigned long long ullint;

class WaveletTree {
public:
  WaveletTree();
  WaveletTree(char* data, ullint data_sz, int alphabet_sz);

  // Expects data over an alphabet consisting of [0, 1, 2, ..., alphabet_sz-1]
  // Warning: data will become garbled
  void init(char* data, ullint data_sz, int alphabet_sz);

  void build_tree(int x, int a_lo, int a_hi, char* data, ullint data_sz);

  ullint get_rank(char c, ullint pos);

private:
  struct Node {
    RankedBitmap bitmap;
  };

  vector<Node> nodes;
  int alphabet_sz;
};

#endif // !_WAVELET_TREE_H_

