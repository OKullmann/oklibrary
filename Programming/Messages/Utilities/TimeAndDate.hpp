// Oliver Kullmann, 25.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Utilities/TimeAndDate.hpp
  \brief Message classes for time and date representation
*/

#ifndef TIMEANDDATE_jHgFrT45
#define TIMEANDDATE_jHgFrT45

#include <OKlib/General/TimeHandling.hpp>

#include <OKlib/Programming/Messages/MessagesBase.hpp>

namespace OKlib {
  namespace Messages {
    namespace Utilities {

      /*!
        \class TimeDateStandardImplementation
        \brief Message objects for the preferred date and time representation
        by the system

        For these messages, level and language specifications are ignored:
        <ol>
         <li> The level is always that of the standard C-output, </li>
         <li> while the language is always given by the current locale. </li>
        </ol>

      */

      struct TimeDateStandardImplementation : ::OKlib::Messages::MessagesBase {
        std::ostream& print(std::ostream& out) const {
          return ::TimeHandling::currentDateTime("%c", out);
        }
      };

    }

  }

}

#endif
