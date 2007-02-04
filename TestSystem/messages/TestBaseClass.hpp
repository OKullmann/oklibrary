// Oliver Kullmann, 19.2.2006 (Swansea)

/*!
  \file TestSystem/messages/TestBaseClass.hpp
  \brief Messages for class OKlib::TestSystem::TestBase
*/

#ifndef TESTBASECLASSMESSAGES_jjdh6648
#define TESTBASECLASSMESSAGES_jjdh6648

#include <cassert>
#include <string>
#include <tr1/memory>

#include <Transitional/SystemSpecifics/Demangling.hpp>
#include <Transitional/Messages/MessagesMain.hpp>
#include <Transitional/Messages/LineHandling.hpp>
#include <Transitional/Messages/Utilities/FileIdentification.hpp>
#include <Transitional/Messages/Utilities/LineIdentification.hpp>

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/TestFondement.hpp>
#include <Transitional/TestSystem/messages/TestLevel.hpp>

namespace OKlib {

  namespace TestSystem {
    namespace messages {

      OKLIB_USING_MESSAGES

      /*!
        \class BasicTestDescription
        \brief Outputs a description of the test (an identification of the
        test function, and nesting depth and test level).

        \todo While we use capitalised "Test function" etc., the test-level
        comes out as "test level" etc..
      */

      struct BasicTestDescription : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

      private :

        typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;

        std::string type_name;
        MessagePointer file;
        MessagePointer line;
        ::OKlib::TestSystem::depth_number_type depth;
        MessagePointer test_level_description;

      public :

        BasicTestDescription(
                             const char* const type_name_mangled,
                             ::OKlib::Messages::Utilities::FileIdentification* const file_name,
                             ::OKlib::Messages::Utilities::LineIdentification* const line_number,
                             const ::OKlib::TestSystem::depth_number_type test_depth,
                             const ::OKlib::TestSystem::TestLevel& test_level) :
          type_name(::OKlib::SystemSpecifics::Demangle()(type_name_mangled)),
          file(file_name),
          line(line_number),
          depth(test_depth),
          test_level_description(new ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level))
        {
          assert(file.get());
          assert(line.get());
          assert(test_level_description.get());
        }
        BasicTestDescription(
                             const char* const type_name_mangled,
                             const MessagePointer file_name,
                             const MessagePointer line_number,
                             const ::OKlib::TestSystem::depth_number_type test_depth,
                             const ::OKlib::TestSystem::TestLevel& test_level) :
          type_name(::OKlib::SystemSpecifics::Demangle()(type_name_mangled)),
          file(file_name),
          line(line_number),
          depth(test_depth),
          test_level_description(new ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level))
        {
          assert(file.get());
          assert(line.get());
          assert(test_level_description.get());
}

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          l_start(out) << "Test class = " << type_name.c_str(); l_end(out);
          l_start(out) << "Test depth = " << depth; l_end(out);
          out.flush();
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          assert(file.get());
          assert(line.get());
          assert(test_level_description.get());
          print(out, L<en_GB>(), S<Basic>());
          out << file -> cp_pp(*this); l_end(out);
          out << line -> cp_pp(*this); l_end(out);
          out << test_level_description -> cp_pp(*this); l_end(out);
          out.flush();
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          l_start(out) << "Testklasse = " << type_name.c_str(); l_end(out);
          l_start(out) << "Testtiefe = " << depth; l_end(out);
          out.flush();
        }
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          assert(file.get());
          assert(line.get());
          assert(test_level_description.get());
          print(out, L<de_DE>(), S<Basic>());
          out << *file;
          out << *line;
          out << *test_level_description;
          out.flush();
        }

      };

      // #############################################

      /*!
        \class LogDescription
        \brief Outputs a description of the circumstances of a log-message
      */

      struct LogDescription : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        const char* const file_name;
        const ::OKlib::TestSystem::line_number_type line_number;
        const ::OKlib::TestSystem::depth_number_type depth;
        const ::OKlib::TestSystem::TestLevel* const test_level;

        LogDescription(
                       const char* const file_name,
                       const ::OKlib::TestSystem::line_number_type line_number,
                       const ::OKlib::TestSystem::depth_number_type depth,
                       const ::OKlib::TestSystem::TestLevel* test_level) :
          file_name(file_name),
          line_number(line_number),
          depth(depth),
          test_level(test_level)
        {}
        
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "Log message at line " << line_number << " in file " << file_name << ":\n";
        }
        void print(std::ostream& out, L<en_GB>, S<Extensive>) const {
          out << "Log message at line " << line_number << " in file " << file_name << ",\n";
          out << "at test depth = " << depth << " and " << ::OKlib::TestSystem::messages::TestLevelDescriptions(*test_level) << ":\n";
        }
        
        void print(std::ostream& out, L<de_DE>, S<Basic>) const {}
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          out << "Log-Meldung in Zeile " << line_number << " und Datei " << file_name << ":\n";
        }
        void print(std::ostream& out, L<de_DE>, S<Extensive>) const {
          out << "Log-Meldung in Zeile " << line_number << " und Datei " << file_name << ",\n";
          out << "wobei Test-Schachtelungstiefe = " << depth << " und " << ::OKlib::TestSystem::messages::TestLevelDescriptions(*test_level) << ":\n";
        }

      };

    }

  }

}

#endif
