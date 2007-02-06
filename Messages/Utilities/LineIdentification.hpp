// Oliver Kullmann, 30.12.2006 (Swansea)

/*!
  \file Messages/Utilities/LineIdentification.hpp
  \brief Message class to provide source-code line-number information, plus supporting macro
*/

#ifndef LINEIDENTIFICATION_HHcaoryGb384
#define LINEIDENTIFICATION_HHcaoryGb384

#include <string>

#include <Transitional/Messages/MessagesMain.hpp>
#include <Transitional/Messages/LineHandling.hpp>

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
