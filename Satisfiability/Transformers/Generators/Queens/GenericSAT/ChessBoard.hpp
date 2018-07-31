// Oliver Kullmann, 6.7.2018 (Swansea)

#include <utility>

#include <cstdint>

namespace ChessBoard {

  typedef std::uint32_t coord_t;

  struct Var {
    coord_t first;
    coord_t second;
  };
  static_assert(std::is_pod<Var>::value, "Var is not POD.");

  typedef std::uint64_t Var_uint;

  typedef std::uint_fast64_t Count_t;

}
