// Oliver Kullmann, 5.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Variables/TrivialVariables.hpp
  \brief Typedefs for models of concept Variables given by signed and unsigned integral types
*/

#ifndef TRIVIALVARIABLES_916TyVbx
#define TRIVIALVARIABLES_916TyVbx

#include <Transitional/Concepts/traits/concept_tag.hpp>
#include <Transitional/Variables/traits/index_type.hpp>

#include <Transitional/Concepts/Variables.hpp>
#include <Transitional/Concepts/JoinedConceptTags.hpp>

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

    namespace traits {
    
      template <> struct index_type< ::OKlib::Variables::Variables_signed_char> {
        typedef signed char type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_short_int> {
        typedef short int type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_int> {
        typedef int type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_long_int> {
        typedef long int type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_unsigned_char> {
        typedef unsigned char type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_unsigned_short_int> {
        typedef unsigned short int type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_unsigned_int> {
        typedef unsigned int type;
      };
      template <> struct index_type< ::OKlib::Variables::Variables_unsigned_long_int> {
        typedef unsigned long int type;
      };
      
    }

  }

  namespace Concepts {
    namespace traits {

      template <> struct concept_tag< ::OKlib::Variables::Variables_signed_char> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_short_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_long_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_char> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_short_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      template <> struct concept_tag< ::OKlib::Variables::Variables_unsigned_long_int> {
        typedef OKlib::Concepts::VariablesLiterals_tag<OKlib::Concepts::VariablesAsIndex_tag,  OKlib::Concepts::Literals_tag> type;
      };
      
    }
  }

}

#endif
