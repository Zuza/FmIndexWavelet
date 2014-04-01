#include "DnaIndex.hpp"

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
  const int num_genes = 3;
  static char gene[][123] = {
    "ACGTAG",
    "AGCAC",
    "GCAATCA",
  };

  DnaIndex dna_index(1e6);

  // for num_genes?
  for (int i = 0; i < num_genes; ++i) {
    dna_index.insert_gene(gene[i], strlen(gene[i]));
  }
  dna_index.create_index();

  const char query[] = "CA"; const int query_len = strlen(query);
  vector<pair<ullint, ullint> > results;
  dna_index.get_substring_pos(results, query, query_len);

  for (int i = 0; i < (int)results.size(); ++i) {
    printf("gene = %llu position in gene = %llu\n", results[i].first, results[i].second);
  }

  return 0; 

}
