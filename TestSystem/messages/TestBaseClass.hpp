// Oliver Kullmann, 19.2.2006 (Swansea)

/*!
  \file TestSystem/messages/TestBaseClass.hpp
  \brief Messages for class OKlib::TestSystem::TestBase
*/

#ifndef TESTBASECLASSMESSAGES_jjdh6648
#define TESTBASECLASSMESSAGES_jjdh6648

#include <cassert>
#include <string>

#include <Transitional/SystemSpecifics/Demangling.hpp>
#include <Transitional/Messages/MessagesMain.hpp>

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

        \todo Use FileIdentification.
        \todo Use message-class for type-names.
        \todo While we use capitalise "Test function" etc., the test-level
        comes out as "test level".
      */

      struct BasicTestDescription : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        const std::string type_name;
        const char* const file_name;
        const ::OKlib::TestSystem::line_number_type line_number;
        const ::OKlib::TestSystem::depth_number_type depth;
        const ::OKlib::TestSystem::TestLevel& test_level;

        BasicTestDescription(
                             const char* const type_name_mangled,
                             const char* const file_name,
                             const ::OKlib::TestSystem::line_number_type line_number,
                             const ::OKlib::TestSystem::depth_number_type depth,
                             const ::OKlib::TestSystem::TestLevel& test_level) :
          type_name(::OKlib::SystemSpecifics::Demangle()(type_name_mangled)),
          file_name(file_name),
          line_number(line_number),
          depth(depth),
          test_level(test_level)
        {}

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "Test function = " << type_name.c_str() << "\n";
          out << "Test depth = " << depth << std::endl;
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "Test function = " << type_name.c_str() << "\n";
          out << "Test depth = " << depth << std::endl;
          out << "File name = " << file_name << "\n";
          out << "Line number = " << line_number << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          out << "Testfunktion = " << type_name.c_str() << "\n";
          out << "Testtiefe = " << depth << std::endl;
        }
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          out << "Testfunktion = " << type_name.c_str() << "\n";
          out << "Testtiefe = " << depth << std::endl;
          out << "Dateiname = " << file_name << "\n";
          out << "Zeilennummer = " << line_number << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

      };

      /*!
        \class LogDescription
        \brief Outputs a description of the circumstances of a log-message

        \todo How is ownership of the test-level object handled here?
        \todo Use FileIdentification, and also a messages-class for line-numbers etc.
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
