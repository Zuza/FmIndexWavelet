#include <algorithm>
#include <functional>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define VAR(name, val) __typeof(val) name=(val)
#define FORC(it, V) for(VAR(it, (V).begin()); it != (V).end(); ++it)
#define REP(i, n) for (int i = 0; i < (n); ++i)
#define TRACE(x) cout << #x << " = " << (x) << endl

typedef long long llint;
typedef unsigned long long hash_t; // 2^64 -> 2^31

const hash_t P = 12347;

struct Index {
  int len; // watch out!
  int full_len;

  vector<hash_t> prefix;
  vector<int> sa;

  vector<int> occ[4];
  int before[5];

  char *S;

  // ptr should be double
  Index(char* ptr, int len) {
    this->len = len;
    this->full_len = 2 * len + 1;

    S = (char*)malloc(full_len * sizeof(char));
    assert(S);

    for (int i = 0; i < len; ++i) {
      S[i] = ptr[i];
      S[i+len] = ptr[i];
    }
    S[len] = -1;

    prefix.resize(full_len + 1);
    prefix[0] = 0;
    for (int i = 0; i < full_len; ++i) {
      prefix[i+1] = prefix[i]*P + S[i];
    }

    build_sa();
  }

  struct IndexCmpf {
    int len;
    int full_len;
    vector<hash_t>& prefix;
    const char* S;

    vector<hash_t> powers;

    IndexCmpf(int _len, int _full_len, vector<hash_t>& _prefix, const char* _S) : prefix(_prefix), S(_S){
      len = _len;
      full_len = _full_len;
      powers.resize(full_len);
      powers[0] = 1;
      for (int i = 1; i < full_len; ++i)
        powers[i] = powers[i-1] * P;
    }

    hash_t get_hash(int a, int b) const { // [a, b>
      return prefix[b] - prefix[a] * powers[b-a];
    }

    int get_eq_len(int a, int b) const {
      int l = 0, r = len;
      while (l < r) {
        int mid = (l + r + 1) / 2;
        if (get_hash(a, a+mid) == get_hash(b, b+mid))
          l = mid;
        else
          r = mid - 1;
      }
      return l;
    }

    bool operator () (int a, int b) const {
      int L = get_eq_len(a, b);
      return S[a + L] < S[b + L];
    }
  };

  void build_sa() {
    sa.resize(len + 1);
    for (int i = 0; i <= len; ++i) {
      sa[i] = i;
    }
    sort(sa.begin(), sa.end(), IndexCmpf(len, full_len, prefix, S));
    build_occ();
  }

  void build_occ() {
    for (int i = 0; i < 4; ++i) {
      occ[i].resize(len + 2, 0);
    }
    for (int i = 0; i <= 4; ++i)
      before[i] = 0;   

    for (int i = 0; i <= len; ++i) {
      char bwt = S[sa[i] + len];

      before[((int)bwt)+1]++;
      for (int j = 0; j < 4; ++j)
        occ[j][i+1] = occ[j][i] + (bwt == j);
    }

    for (int i = 0; i < 4; ++i) {
      before[i+1] += before[i];
    }

    // for (int i = 0; i < 4; ++i) {
    //   TRACE(before[i]);
    //   for (int j = 0; j <= len+1; ++j)
    //     printf("%d ", occ[i][j]);
    //   putchar('\n');
    // }
    // TRACE(before[4]);

    free(S);
  }

  int count(char *ptr, int L) {
    int first = (int)ptr[L - 1];
    int a = before[first], b = before[first+1];

    for (int i = L - 2; i >= 0; --i) {
      int x = ptr[i];
      a = before[x] + occ[x][a];
      b = before[x] + occ[x][b];
    }

    return b - a;
  }
};

int main(void)
{
  srand(time(0));

  const int len = 6;
  char* ptr = (char*)malloc(sizeof(len));
  for (int i = 0; i < len; ++i) {
    ptr[i] = rand() % 4;
  }
  
  Index index(ptr, len);
  char niz[2] = {0, 0};
  TRACE(index.count(niz, 2));

  return 0;
}
