// Oliver Kullmann, 18.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/SystemSpecifics/Demangling.hpp
  \brief Translating type names in a readable form

  Users need to link with -liberty.
*/

#ifndef DEMANGLING_uHHalal0
#define DEMANGLING_uHHalal0

#include <functional>
#include <string>
#include <cassert>
#include <cstdlib>

#include <OKlib/Programming/SystemSpecifics/demangle.h>
// this was /usr/include/demangle.h

namespace OKlib {
  namespace SystemSpecifics {

    /*!
      \class Demangle
      \brief Functor, translating mangled type names into plain type names (both as C-strings).
    */

    struct Demangle : std::unary_function<const char*, const char*> {
      std::string operator ()(const char* const name) const {
        struct C_str {
          char* const s;
          C_str(char* const s) : s(s) { assert(s); }
          ~C_str() { std::free(s); }
        };
        const C_str c_str(cplus_demangle(name, DMGL_TYPES));
        std::string result(c_str.s);
        return result;
      }
    };
    
  }

}

#endif
