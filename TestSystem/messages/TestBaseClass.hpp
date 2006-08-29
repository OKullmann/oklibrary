// Oliver Kullmann, 19.2.2006 (Swansea)

/*!
  \file TestSystem/messages/TestBaseClass.hpp
  \brief Messages for OKlib::TestSystem::TestBase
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

      OKLIB_MESSAGES(BasicTestDescription) {
        OKLIB_MESSAGES_PRINT

        const std::string type_name;
        const char* const file_name;
        const ::OKlib::TestSystem::line_number_type line_number;
        const ::OKlib::TestSystem::depth_number_type depth;
        ::OKlib::TestSystem::TestLevel& test_level;

        BasicTestDescription(const char* const type_name_mangled, const char* const file_name, const ::OKlib::TestSystem::line_number_type line_number, const ::OKlib::TestSystem::depth_number_type depth, ::OKlib::TestSystem::TestLevel& test_level) : type_name(::OKlib::SystemSpecifics::Demangle()(type_name_mangled)), file_name(file_name), line_number(line_number), depth(depth), test_level(test_level) {}

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "Test function = " << type_name.c_str() << "\n";
          out << "Test depth = " << depth << std::endl;
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "Test function = " << type_name.c_str() << "\n";
          out << "Test depth = " << depth << std::endl;
          out << "File name = " << file_name << "\n";
          out << "Line number = " << line_number << "\n";
          out << "Test level = " << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
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
          out << "Testniveau = " << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

      };

    }

  }

}

#endif
