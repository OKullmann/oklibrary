// Oliver Kullmann, 23.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/InputOutput.hpp
  \brief Concepts concerning general input and output.
*/

#ifndef INPUTOUTPUT_88uUHhbg
#define INPUTOUTPUT_88uUHhbg

#include <ostream>
//#include <istream>

#include <OKlib/Concepts/LibraryBasics.hpp>

namespace OKlib {
  namespace Concepts {

    struct OutputStreamable_tag : ConceptsBase_tag {};

    template <class X>
    struct OutputStreamable {
      void constraints() {
        static_cast<std::ostream&>(out << x_c);
      }
      std::ostream out;
      const X x_c;
    };

    class OutputStreamable_Archetype {
      typedef OutputStreamable_tag concept_tag;
      OutputStreamable_Archetype();
      ~OutputStreamable_Archetype();
      OutputStreamable_Archetype(const OutputStreamable_Archetype&);
      OutputStreamable_Archetype& operator =(const OutputStreamable_Archetype&);
      friend std::ostream& operator <<(std::ostream& out, const OutputStreamable_Archetype&) { return out; }
    };

  }

}

#endif
