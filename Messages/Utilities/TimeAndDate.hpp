// Oliver Kullmann, 25.2.2006 (Swansea)

/*!
  \file Messages/Utilities/TimeAndDate.hpp
  \brief Message classes for time and date representation
*/

#ifndef TIMEANDDATE_jHgFrT45
#define TIMEANDDATE_jHgFrT45

#include <Transitional/General/TimeHandling.hpp>

namespace OKlib {

  namespace Messages {

    namespace Utilities {

      /*!
        \class Messages::Utilities::TimeDateStandardImplementation
        \brief Message objects for the preferred date and time representations by the system

        For these messages, level and language specifications are ignored: The level is always
        that of the standard C-output, while the language is always given by the current locale.

        \todo On the one hand it would be nice if we could specify different levels (say, the
        implementation below as "full", one without timezone, seconds and day as "basic", and
        one with full names for day and month (possibly also with day-number and week-number)
        as "extensive"), but since this is not provided by the standard, we had to define it
        all ourselves (that is, the order of the components). So perhaps the implementation
        below is a reasonable compromise. (But the best thing would be if the format string
        could be determined, and then one would simply replace the abbreviated forms by
        their full forms, and potentially add something.)
      */

      OKLIB_MESSAGES(TimeDateStandardImplementation) {

        std::ostream& print(std::ostream& out) const {
          return ::TimeHandling::currentDateTime("%c", out);
        }
      };

    }

  }

}

#endif
