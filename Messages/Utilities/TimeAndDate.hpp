// Oliver Kullmann, 25.2.2006 (Swansea)

/*!
  \file Messages/Utilities/TimeAndDate.hpp
  \brief Message classes for time and date representation
*/

#ifndef TIMEANDDATE_jHgFrT45
#define TIMEANDDATE_jHgFrT45

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Messages/MessagesBase.hpp>

namespace OKlib {

  namespace Messages {

    namespace Utilities {

      /*!
        \class Messages::Utilities::TimeDateStandardImplementation
        \brief Message objects for the preferred date and time representations by the system

        For these messages, level and language specifications are ignored: The level is always
        that of the standard C-output, while the language is always given by the current locale.

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
