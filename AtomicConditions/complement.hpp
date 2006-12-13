// Oliver Kullmann, 13.12.2006 (Swansea)

/*!
  \file AtomicConditions/complement.hpp
  \brief Primary template for the complementation function
*/

#ifndef COMPLEMENT_jJJggBop67
#define COMPLEMENT_jJJggBop67

namespace OKlib {
  namespace AtomicConditions {

    template <typename AC>
    inline void complement(AC& x) { x.complement(); }

  }
}

#endif
