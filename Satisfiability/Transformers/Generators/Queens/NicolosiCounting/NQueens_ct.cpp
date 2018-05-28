// Oliver Kullmann 27.5.2018 (Swansea)

/*
  Computes the N-Queens count for N given as macro, e.g. for N=16:
> make SETN=-DN=16
> ./qcount_ct

*/


#include <iostream>
#include <cassert>
#include <bitset>

namespace {

typedef std::size_t size_t;
#ifndef N
# error "N must be defined."
#endif
constexpr size_t n=N;

typedef std::uint_fast64_t count_t; // counting solutions
typedef std::bitset<n> queen_t;

queen_t setbits(const size_t m) {
  assert(m <= n);
  queen_t res;
  for (size_t i = 0; i < m; ++i) res[i] = true;
  return res;
}

count_t count=0, nodes=0;

inline void backtracking(const queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  size_t size) noexcept {
  assert(avail.any());
  ++nodes;
  const size_t sp1 = size+1;
  assert(sp1 < n);
  if (sp1+1 == n)
    for (size_t i = 0; i < n; ++i) {
      if (not avail[i]) continue;
      queen_t forb(columns);
      forb[i] = true;
      {queen_t ndiag(fdiag); ndiag[i]=true; ndiag>>=1; forb |= ndiag;}
      {queen_t nantid(fantid); nantid[i]=true; nantid<<=1; forb |= nantid;}
      if (not forb.all()) ++count;
    }
  else
    for (size_t i = 0; i < n; ++i) {
      if (not avail[i]) continue;
      queen_t newcolumns(columns); newcolumns[i] = true;
      queen_t ndiag(fdiag); ndiag[i] = true; ndiag >>= 1;
      queen_t nantid(fantid); nantid[i] = true; nantid <<= 1;
      const queen_t newavail(~(newcolumns | ndiag | nantid));
      if (newavail.any()) backtracking(newavail,newcolumns,ndiag,nantid,sp1);
    }
}

}

int main() {
  if (n % 2 == 0) {
    backtracking(setbits(n/2), 0, 0, 0, 0);
    std::cout << 2*count << " " << nodes << "\n";
  }
  else {
    backtracking(setbits(n/2), 0, 0, 0, 0);
    const count_t half = count; count = 0;
    backtracking(queen_t().set(n/2), 0, 0, 0, 0);
    std::cout << 2*half + count << " " << nodes << "\n";
  }
}
