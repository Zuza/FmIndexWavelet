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
  const int data_len = 123123123;
  static char data[data_len+123];

  mt19937 mt;
  for (int i = 0; i < data_len; ++i)
    data[i] = alpha[mt() % alpha_sz];

  FmIndex fmindex(data, data_len, alpha, alpha_sz);

  return 0; 
}
