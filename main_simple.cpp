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
  static char data[] = "AGTTCAGATCT"; int data_len = strlen(data);
  static char orig_data[123];

  strncpy(orig_data, data, data_len);
  orig_data[data_len] = 0;

  FmIndex fmindex(data, data_len, alpha, alpha_sz);

  static char query[] = "TCA";

  ullint cnt = fmindex.count_substrings(query, strlen(query));
  printf("cnt = %llu\n", cnt);

  return 0; 

}
