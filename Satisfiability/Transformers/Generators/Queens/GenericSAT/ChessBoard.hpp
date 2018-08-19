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

}

#endif
