// Oliver Kullmann, 25.2.2006 (Swansea)

/*!
  \file RunTest_Messages.hpp
  \brief Messages for OKlib::TestSystem::RunTest
  \todo Create a new module Messages/DateAndTime for messages regarding date and time,
  and use them below.
*/

#ifndef RUNTESTMESSAGES_jJHHg5tr

#define RUNTESTMESSAGES_jJHHg5tr

#include <string>

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Messages/MessagesMain.hpp>

#include <Transitional/Testsystem/TestLevel_Explanations.hpp>
#include <Transitional/Testsystem/TestFondement.hpp>
#include <Transitional/Testsystem/RunTest_Declarations.hpp>

namespace OKlib {

  namespace TestSystem {

    namespace Documentation {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(RunTestOpening) {
        OKLIB_MESSAGES_PRINT

        ::OKlib::TestSystem::TestLevel& test_level;
        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type number_testobjects;
        const char banner_character;
        const unsigned int number_banner_characters;
        const char* const identification_string;

        RunTestOpening(::OKlib::TestSystem::TestLevel& level, const size_type number_tests) : test_level(level), number_testobjects(number_tests), banner_character('#'), number_banner_characters(40), identification_string("::OKlib::TestSystem::RunTest") {}

        void banner(std::ostream& out) const {
          out << std::string(number_banner_characters, banner_character);
        }
        void opening(std::ostream& out) const {
          out << "\n";
          banner(out);
          out << "\n" << identification_string << "\n\n";
        }

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          opening(out);
          out << TimeHandling::currentDateTime("%A, %B %e, %Y; %H:%M:%S%n");
          out << ::OKlib::TestSystem::Documentation::TestLevelDescriptions(test_level) << "\n";
          out << number_testobjects << " testobject";
          if (number_testobjects != 1) out << "s";
          out << ". \n" << std::endl;
        }

      };

    }

  }

}

#endif
