// Oliver Kullmann, 30.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Utilities/LineIdentification.hpp
  \brief Message class to provide source-code line-number information, plus supporting macro
*/

#ifndef LINEIDENTIFICATION_HHcaoryGb384
#define LINEIDENTIFICATION_HHcaoryGb384

#include <string>

#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>

namespace OKlib {
  namespace Messages {
    namespace Utilities {

      /*!
        \class LineIdentification
        \brief Line number indentification (for source-code files)
      */

      OKLIB_USING_MESSAGES

      struct LineIdentification : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT
        LineIdentification(std::string const line) : line(line) {}
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          l_start(out) << "line number = " << line;
        }
        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          l_start(out) << "Zeilennummer = " << line;
        }
      private :
        std::string line;
      };

    /*!
      \def OKLIB_LINE
      \brief Macro to create a message object with the current line number

      By OKLIB_LINE a pointer to a LineIdentification-object of dynamic storage creation is created.
    */

# define OKLIB_NUMBER(N) # N
    //! putting quotes around the line number
# define OKLIB_INTERMEDIATE_LINE(X) OKLIB_NUMBER(X)
# define OKLIB_LINE new ::OKlib::Messages::Utilities::LineIdentification(OKLIB_INTERMEDIATE_LINE(__LINE__))

    }
  }
}

#endif
