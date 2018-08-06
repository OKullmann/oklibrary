// Oliver Kullmann, 6.7.2018 (Swansea)

#include <utility>

#include <cstdint>

namespace ChessBoard {

  // The coordinates of the field, with valid values >= 1:
  typedef std::uint32_t coord_t;

  /*
     Var{i,j} for i, j >= 1 refers to a valid field, while i=0 or j=0
     are "singular" variables.
  */
  struct Var {
    coord_t first;
    coord_t second;
  };
  static_assert(std::is_pod<Var>::value, "Var is not POD.");

  // Values related to encoding the 2-dimensional variables:
  typedef std::uint64_t Var_uint;
  typedef std::int64_t Var_int;

  // Counting solutions:
  typedef std::uint_fast64_t Count_t;

}
