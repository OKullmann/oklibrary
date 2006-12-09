// Oliver Kullmann, 9.12.2006 (Swansea)

/*!
  \file Literals/TrivialLiterals.hpp
  \brief Typedefs for trivial models of concept Literals

  \todo Complete implementations.
*/

#ifndef TRIVIALLITERALS_jdjNNqpqoh7u5
#define TRIVIALLITERALS_jdjNNqpqoh7u5

#include <Transitional/Variables/TrivialVariables.hpp>
#include <Transitional/AtomicConditions/TrivialAtomicConditions.hpp>

#include <Transitional/Literals/var.hpp>
#include <Transitional/Literals/cond.hpp>


namespace OKlib {
  namespace Literals {

    typedef signed char Literals_signed_char;
    typedef short int Literals_short_int;
    typedef int Literals_int;
    typedef long int Literals_long_int;

  }

  namespace Concepts {
    namespace traits {

      // PROBLEM with global concept_tag !
      
//       template <>
//       struct concept_tag<OKlib::Literals::Literals_signed_char> {
//         typedef OKlib::Concepts::Literals_tag type;
//       };
//       template <>
//       struct concept_tag<OKlib::Literals::Literals_short_int> {
//         typedef OKlib::Concepts::Literals_tag type;
//       };
//       template <>
//       struct concept_tag<OKlib::Literals::Literals_int> {
//         typedef OKlib::Concepts::Literals_tag type;
//       };
//       template <>
//       struct concept_tag<OKlib::Literals::Literals_long_int> {
//         typedef OKlib::Concepts::Literals_tag type;
//       };

    }
  }
}

#endif
