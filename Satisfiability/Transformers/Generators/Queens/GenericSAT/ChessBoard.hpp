// Oliver Kullmann, 6.7.2018 (Swansea)

#ifndef CHESSBOARD_PjPxE
#define CHESSBOARD_PjPxE

#include <limits>
#include <type_traits>

#include <cstdint>

namespace ChessBoard {

  // The coordinates of the field, with valid values >= 1:
  typedef std::uint32_t coord_t;
  typedef std::int32_t scoord_t;
  constexpr coord_t max_coord = std::numeric_limits<coord_t>::max() / 2 - 1;

  /*
     Var{i,j} for i, j >= 1 refers to a valid field, while i=0 or j=0
     are "singular" variables.
  */
  struct Var {
    coord_t first;
    coord_t second;
  };
  static_assert(std::is_pod<Var>::value, "Var is not POD.");
  inline constexpr bool singular(const Var v) noexcept {
    return v.first == 0 or v.second == 0;
  }

  // Values related to encoding the 2-dimensional variables:
  typedef std::uint64_t Var_uint;
  typedef std::int64_t Var_int;

  // Counting solutions:
  typedef std::uint_fast64_t Count_t;

  /*
    The decomposition of the NxN field into diagonals (fields with equal
     difference) and antidiagonals (fields with equal sum), where each
     such line is specified by a value of class Diagonal:
  */
  struct Diagonal {
    Var s; // start field
    Var_uint l; // length
    Var_uint i;
    /* Field (variable) (x,y) has abstract diagonal-index x-y, which ranges
       from 1-N to N-1, and then we set i = (x-y) + (N-1) with
       0 <= i <= 2N-2.
    */
  };
  struct AntiDiagonal {
    Var s; // start field
    Var_uint l; // length
    Var_uint i;
    /* (x,y) has abstract antidiagonal-index x+y, which ranges from 1+1 to
       N+N, and then we set i = (x+y) - 2.
    */
  };
  static_assert(std::is_pod<Diagonal>::value, "Diagonal is not POD.");
  static_assert(std::is_pod<AntiDiagonal>::value, "AntiDiagonal is not POD.");

  // Returns diagonal starting field, length and index:
  inline constexpr Diagonal diagonal(const Var v, const coord_t N) noexcept {
    assert(v.first >= 1 and v.second >= 1);
    assert(v.first <= N and v.second <= N);
    const scoord_t c_diff = v.first - v.second;
    if (c_diff >= 0) {
      const coord_t cd = c_diff; return {{cd+1,1}, N-cd, N-cd-1};
    }
    else {
      const coord_t cd = -c_diff; return {{1,cd+1}, N-cd, N+cd-1};
    }
  }
  // Returns anti_diagonal starting field, length and index:
  inline constexpr AntiDiagonal anti_diagonal(const Var v, const coord_t N) noexcept {
    assert(v.first >= 1 and v.second >= 1);
    assert(v.first <= N and v.second <= N);
    const coord_t c_sum = v.first + v.second;
    if (c_sum <= N) return {{1,c_sum-1}, c_sum-1, c_sum-2};
    else return {{c_sum-N,N}, 2*N-(c_sum-1), c_sum-2};
  }

  // The number of open, placed and forbidden fields for any line, that is, any
  // row, column, diagonal or antidiagonal:
  struct Rank {
    Var_uint o;
    Var_uint p;
    Var_uint f;
  };
  static_assert(std::is_pod<Rank>::value, "Rank is not POD.");

  // The same numbers as with Rank, but now for the whole board:
  struct TotalRank {
    Var_uint o;
    Var_uint p;
    Var_uint f;
  };
  static_assert(std::is_pod<TotalRank>::value, "TotalRank is not POD.");

  enum class State { open=0, placed, forbidden };

}

#endif
