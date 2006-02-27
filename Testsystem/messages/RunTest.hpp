// Oliver Kullmann, 25.2.2006 (Swansea)

/*!
  \file Testsystem/messages/RunTest.hpp
  \brief Messages for OKlib::TestSystem::RunTest
*/

#ifndef RUNTESTMESSAGES_jJHHg5tr

#define RUNTESTMESSAGES_jJHHg5tr

#include <string>

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Messages/MessagesMain.hpp>
#include <Transitional/Messages/Utilities/TimeAndDate.hpp>

#include <Transitional/Testsystem/messages/TestLevel.hpp>
#include <Transitional/Testsystem/TestFondement.hpp>
#include <Transitional/Testsystem/RunTest_Declarations.hpp>

namespace OKlib {

  namespace TestSystem {

    namespace messages {

      struct Banner {
        const char banner_character;
        const unsigned int number_banner_characters;
        Banner() :  banner_character('X'), number_banner_characters(80) {}
        Banner(const char banner_character, const unsigned int number_banner_characters) : banner_character(banner_character), number_banner_characters(number_banner_characters) {}
        void operator()(std::ostream& out) const {
          out << std::string(number_banner_characters, banner_character);
        }
      };

      struct IdentificationTestsystem {
        const char* const identification_string;
        IdentificationTestsystem() : identification_string("::OKlib::TestSystem::RunTest") {}
        IdentificationTestsystem(const char* const identification_string) : identification_string(identification_string) {}
        void operator()(std::ostream& out) const {
          out << identification_string;
        }
      };

      // ##################################################

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(RunTestOpening) {
        OKLIB_MESSAGES_PRINT

        ::OKlib::TestSystem::TestLevel& test_level;
        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type number_testobjects;

        RunTestOpening(::OKlib::TestSystem::TestLevel& level, const size_type number_tests) : test_level(level), number_testobjects(number_tests) {}

        void opening(std::ostream& out) const {
          out << "\n";
          Banner()(out);
          out << "\n";
          IdentificationTestsystem()(out);
          out << "\n\n";
          out << ::OKlib::Messages::Utilities::TimeDateStandardImplementation() << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          opening(out);
          out << number_testobjects << " testobject";
          if (number_testobjects != 1) out << "s";
          out << ". \n" << std::endl;
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          opening(out);
          out << number_testobjects << " Testobjekt";
          if (number_testobjects != 1) out << "e";
          out << ". \n" << std::endl;
        }

      };

      // ##################################################

      OKLIB_MESSAGES(RunTestClosing) {
        OKLIB_MESSAGES_PRINT

        ::OKlib::TestSystem::TestLevel& test_level;
        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type number_errors;
        const size_type number_testobjects;
        const double system_time;
        const double total_time;

        RunTestClosing(::OKlib::TestSystem::TestLevel& level, const size_type number_errors, const size_type number_tests, const double system_time, const double total_time) : test_level(level), number_errors(number_errors), number_testobjects(number_tests), system_time(system_time), total_time(total_time) {}

        void closing(std::ostream& out) const {
          out << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
          out << ::OKlib::Messages::Utilities::TimeDateStandardImplementation() << "\n";
          out << "\n";
          IdentificationTestsystem()(out);
          out << "\n";
          Banner()(out);
          out << std::endl;
        }

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          out << "\nTEST END\n";
          out << number_errors << " error";
          if (number_errors != 1) out << "s";
          out << ",\n";
          out << number_testobjects << " testobject";
          if (number_testobjects != 1) out << "s";
          out << ". \n";
          out << "\nElapsed system time: " << system_time << "s\n";
          out << "Elapsed total time: " << total_time << "s\n";
          closing(out);
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          out << "\nTESTENDE\n";
          out << number_errors << " Fehler,\n";
          out << number_testobjects << " Testobjekt";
          if (number_testobjects != 1) out << "e";
          out << ". \n";
          out << "\nVerbrauchte Systemzeit: " << system_time << "s\n";
          out << "Verbrauchte Gesamtzeit: " << total_time << "s\n";
          closing(out);
        }

      };

    }

  }

}

#endif
