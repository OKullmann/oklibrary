// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TRIVIALVARIABLES_916TyVbx

#define TRIVIALVARIABLES_916TyVbx

#include <Transitional/Traits/concept_tag.hpp>
#include <Transitional/Traits/index_type.hpp>

#include <Transitional/Concepts/Variables.hpp>

namespace OKlib {

  namespace Variables {

    typedef signed char Variables_signed_char;
    typedef short int Variables_short_int;
    typedef int Variables_int;
    typedef long int Variables_long_int;

    typedef unsigned char Variables_unsigned_char;
    typedef unsigned short int Variables_unsigned_short_int;
    typedef unsigned int Variables_unsigned_int;
    typedef unsigned long int Variables_unsigned_long_int;
    
  }

  namespace Concepts {

    template <> struct concept_tag< ::OKlib::Variables::Variables_signed_char> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_signed_char> {
      typedef signed char type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_short_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_short_int> {
      typedef short int type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_int> {
      typedef int type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_long_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_long_int> {
      typedef long int type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_char> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_unsigned_char> {
      typedef unsigned char type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_short_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_unsigned_short_int> {
      typedef unsigned short int type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_unsigned_int> {
      typedef unsigned int type;
    };

    template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_long_int> {
      typedef VariablesAsIndex_tag type;
    };
    template <> struct index_type< ::OKlib::Variables::Variables_unsigned_long_int> {
      typedef unsigned long int type;
    };

  }

}

#endif
