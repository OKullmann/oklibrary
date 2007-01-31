// Oliver Kullmann, 17.12.2006 (Swansea)

/*!
  \file TestSystem/messages/TestExceptions.hpp
  \brief Messages related to test-failure notification
*/

#ifndef TESTEXCEPTIONSMESSAGES_jJbv448865RtFd
#define TESTEXCEPTIONSMESSAGES_jJbv448865RtFd

#include <string>
#include <cassert>
#include <tr1/memory>

#include <Transitional/SystemSpecifics/Demangling.hpp>
#include <Transitional/Messages/MessagesMain.hpp>
#include <Transitional/Messages/LineHandling.hpp>
#include <Transitional/Messages/Utilities/FileIdentification.hpp>
#include <Transitional/Messages/Utilities/LineIdentification.hpp>

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/messages/TestLevel.hpp>
#include <Transitional/TestSystem/messages/TestBaseClass.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace messages {

      /*!
        \class ErrorDescription
        \brief The unit to describe the point in call history leading to a test failure.

        A message class; an object contains shared pointers to message objects for
        the test class identification and the file name and line number for
        identifying the place where the exception was thrown (class ErrorDescription
        takes over ownership of these message-object-pointers).

        \todo Provide different output-languages.
      */

      struct ErrorDescription : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

      private :
        
        typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;

        MessagePointer test_description;
        MessagePointer file;
        MessagePointer line;
        size_type indent;

        struct ClassIdentificationHeading : ::OKlib::Messages::MessagesPrePost {
          OKLIB_MESSAGES_PRINT
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
          void print(std::ostream& out, L<en_GB>, S<Full>) const {
            l_start(out) << "Test class identification:"; l_end(out);
          }
        };
        struct ExceptionIdentificationHeading : ::OKlib::Messages::MessagesPrePost {
          OKLIB_MESSAGES_PRINT
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
          void print(std::ostream& out, L<en_GB>, S<Full>) const {
            l_start(out) << "Test exception identification:"; l_end(out);
          }
        };

      public :

        ErrorDescription(
                         const ::OKlib::TestSystem::messages::BasicTestDescription& test,
                         ::OKlib::Messages::Utilities::FileIdentification* const file_,
                         ::OKlib::Messages::Utilities::LineIdentification* const line_,
                         const size_type indent_ = 1
                         ) :
          test_description(new ::OKlib::TestSystem::messages::BasicTestDescription(test)),
          file(file_),
          line(line_),
          indent(indent_)
        {
          assert(test_description.get());
          assert(file.get());
          assert(line.get());
        }

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          assert(test_description.get());
          assert(line.get());
          ErrorDescription temp(*this);
          temp.prefix() += std::string(indent, ' ');
          out << ClassIdentificationHeading().cp_pp(temp);
          out << test_description -> cp_pp(temp);
          out << ExceptionIdentificationHeading().cp_pp(temp);
          out << line -> cp_pp(temp); l_end(out);
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          assert(test_description.get());
          assert(file.get());
          assert(line.get());
          ErrorDescription temp(*this);
          temp.prefix() += std::string(indent, ' ');
          out << ClassIdentificationHeading().cp_pp(temp);
          out << test_description -> cp_pp(temp);
          out << ExceptionIdentificationHeading().cp_pp(temp);
          out << file -> cp_pp(temp); l_end(out);
          out << line -> cp_pp(temp); l_end(out);
        }

      };
      
      
    }
  }
}

#endif
