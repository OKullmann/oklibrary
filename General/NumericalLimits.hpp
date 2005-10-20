// Oliver Kullmann, 1.5.2004 (Swansea)

#ifndef NUMERICALLIMITSWAECHTER_hdhdt535ae

#define NUMERICALLIMITSWAECHTER_hdhdt535ae

#include <limits>

namespace NumericalLimits {

  // Int x can be safely negated iff -signed_max() <= x <= signed_max()  
  template <typename Int>
  Int signed_max() {
    const Int max = std::numeric_limits<Int>::max();
    const Int min = std::numeric_limits<Int>::min();
    return (min + max <= 0) ? max : - min;
  }
 
}

#endif
