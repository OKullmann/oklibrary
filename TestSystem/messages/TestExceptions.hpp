// Oliver Kullmann, 17.12.2006 (Swansea)

/*!
  \file TestSystem/messages/TestExceptions.hpp
  \brief Messages related to test-failure notification
*/

#ifndef TESTEXCEPTIONSMESSAGES_jJbv448865RtFd
#define TESTEXCEPTIONSMESSAGES_jJbv448865RtFd

#include <cassert>
#include <tr1/memory>

#include <Transitional/Messages/MessagesMain.hpp>
#include <Transitional/Messages/LineHandling.hpp>
#include <Transitional/Messages/Utilities/FileIdentification.hpp>
#include <Transitional/Messages/Utilities/LineIdentification.hpp>

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/messages/TestLevel.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace messages {

      /*!
        \class ErrorDescription
        \brief The unit to describe the point in call history leading to a test failure.

        A message class; an object contains shared pointers to message objects for the file name,
        the line number and the test-level (class ErrorDescription takes over ownership of these
        message-object-pointers), a C string with name of the type of the test class, and an integer
        for the nesting depth.

        \todo Provide different output-levels.
        \todo Provide different output-languages.
      */

      struct ErrorDescription : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

      private :
        
        typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;

        MessagePointer file;
        MessagePointer line;
        const char* type_test_class;
        MessagePointer level_description;
        ::OKlib::TestSystem::depth_number_type depth;

      public :

        ErrorDescription(
                         ::OKlib::Messages::Utilities::FileIdentification* const file_,
                         ::OKlib::Messages::Utilities::LineIdentification* const line_,
                         const char* const type_test_class_,
                         ::OKlib::TestSystem::messages::TestLevelDescriptions* const level_description_,
                         const ::OKlib::TestSystem::depth_number_type depth_
                         ) :
          file(file_),
          line(line_),
          type_test_class(type_test_class_),
          level_description(level_description_),
          depth(depth_)
        {
          assert(file.get());
          assert(line.get());
          assert(type_test_class);
          assert(level_description.get());

          file -> set_prefix(1);
          line -> set_prefix(1);
          level_description -> set_prefix(1);
        }

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          assert(file.get());
          assert(line.get());
          assert(type_test_class);
          assert(level_description.get());
          out << *file << "\n" << *line << "\n test type = " << type_test_class << "\n" << *level_description << "\n test depth = " << depth << "\n";
        }
      
      };
      
      
    }
  }
}

#endif
