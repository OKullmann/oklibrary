// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file AtomicConditions/eval.hpp
  \brief Base template for the evaluation function
*/

#ifndef EVAL_66ahahYt3e
#define EVAL_66ahahYt3e

namespace OKlib {
  namespace AtomicConditions {

    template <typename AC>
    inline bool eval(const AC&, const typename traits::value_type<AC>::type&);
  }
}

#endif
