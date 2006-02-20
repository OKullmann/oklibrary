// Oliver Kullmann, 19.2.2006 (Swansea)

/*!
  \file TestBaseClass_Messages.hpp
  \brief Messages for OKlib::TestSystem::TestBase
*/

#ifndef TESTBASECLASSMESSAGES_jjdh6648

#define TESTBASECLASSMESSAGES_jjdh6648

#include <cassert>
#include <string>

#include "Demangling.hpp"
#include "MessagesMain.hpp"

#include "BasicDeclarations.hpp"
#include "TestLevel_Explanations.hpp"

namespace OKlib {

  namespace TestSystem {

    namespace Documentation {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(BasicTestDescription) {
        OKLIB_MESSAGES_PRINT

        const std::string type_name;
        const char* const file_name;
        const ::OKlib::TestSystem::line_number_type line_number;
        const ::OKlib::TestSystem::depth_number_type depth;
        ::OKlib::TestSystem::TestLevel& test_level;

        BasicTestDescription(const char* const type_name_mangled, const char* const file_name, const ::OKlib::TestSystem::line_number_type line_number, const ::OKlib::TestSystem::depth_number_type depth, ::OKlib::TestSystem::TestLevel& test_level) : type_name(::OKlib::SystemSpecifics::Demangle()(type_name_mangled)), file_name(file_name), line_number(line_number), depth(depth), test_level(test_level) {}

        template <class Stream>
          void print(Stream& out, L<en_GB>, S<Basic>) const {
          out << "Test function = " << type_name.c_str() << "\n";
          out << "File name = " << file_name << "\n";
          out << "Line number = " << line_number << "\n";
          out << "Test level = " << ::OKlib::TestSystem::Documentation::TestLevelDescriptions(test_level) << "\n";
          out << "Test depth = " << depth << std::endl;
        }
      };

    }

  }

}

#endif
