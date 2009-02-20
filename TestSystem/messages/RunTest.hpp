// Oliver Kullmann, 25.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/messages/RunTest.hpp
  \brief Messages for OKlib::TestSystem::RunTest
*/

#ifndef RUNTESTMESSAGES_jJHHg5tr
#define RUNTESTMESSAGES_jJHHg5tr

#include <string>

#include <OKlib/General/TimeHandling.hpp>

#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/Utilities/TimeAndDate.hpp>

#include <OKlib/TestSystem/messages/TestLevel.hpp>
#include <OKlib/TestSystem/TestFondement.hpp>
#include <OKlib/TestSystem/RunTest_Declarations.hpp>

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

      struct IdentificationTestSystem {
        const char* const identification_string;
        IdentificationTestSystem() : identification_string("::OKlib::TestSystem::RunTest") {}
        IdentificationTestSystem(const char* const identification_string) : identification_string(identification_string) {}
        void operator()(std::ostream& out) const {
          out << identification_string;
        }
      };

      // ##################################################

      OKLIB_USING_MESSAGES

      struct RunTestOpening : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        ::OKlib::TestSystem::TestLevel& test_level;
        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type number_testobjects;

        RunTestOpening(::OKlib::TestSystem::TestLevel& level, const size_type number_tests) : test_level(level), number_testobjects(number_tests) {}

        void opening(std::ostream& out, S<Basic>) const {
          out << "\n";
          IdentificationTestSystem()(out);
          out << "\n";
        }

        void opening(std::ostream& out, S<Full>) const {
          out << "\n";
          IdentificationTestSystem()(out);
          out << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

        void opening(std::ostream& out, S<Extensive>) const {
          out << "\n";
          Banner()(out);
          out << "\n";
          IdentificationTestSystem()(out);
          out << "\n\n";
          out << ::OKlib::Messages::Utilities::TimeDateStandardImplementation() << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
        }

        template <class Level>
        void print(std::ostream& out, L<en_GB>, Level) const {
          opening(out, Level());
          out << number_testobjects << " testobject";
          if (number_testobjects != 1) out << "s";
          out << ". \n" << std::endl;
        }

        template <class Level>
        void print(std::ostream& out, L<de_DE>, Level) const {
          opening(out, Level());
          out << number_testobjects << " Testobjekt";
          if (number_testobjects != 1) out << "e";
          out << ". \n" << std::endl;
        }

      };

      // ##################################################

      struct RunTestClosing : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        ::OKlib::TestSystem::TestLevel& test_level;
        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type number_errors;
        const size_type number_testobjects;
        const double system_time;
        const double total_time;

        RunTestClosing(::OKlib::TestSystem::TestLevel& level, const size_type number_errors, const size_type number_tests, const double system_time, const double total_time) : test_level(level), number_errors(number_errors), number_testobjects(number_tests), system_time(system_time), total_time(total_time) {}

        void closing(std::ostream& out, S<Basic>) const {
          out << std::endl;
        }
        void closing(std::ostream& out, S<Full>) const {
          out << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
          out << std::endl;
        }
        void closing(std::ostream& out, S<Extensive>) const {
          out << "\n";
          out << ::OKlib::TestSystem::messages::TestLevelDescriptions(test_level) << "\n";
          out << ::OKlib::Messages::Utilities::TimeDateStandardImplementation() << "\n";
          out << "\n";
          IdentificationTestSystem()(out);
          out << "\n";
          Banner()(out);
          out << "\n" << std::endl;
        }

        void print(std::ostream& out, L<en_GB>, S<Basic> l) const {
          out << number_errors << " error";
          if (number_errors != 1) out << "s";
          out << ".\n";
          closing(out, l);
        }
        void print(std::ostream& out, L<en_GB>, S<Full> l) const {
          out << "\nTEST END\n";
          out << number_errors << " error";
          if (number_errors != 1) out << "s";
          out << ".";
          closing(out, l);
        }
        void print(std::ostream& out, L<en_GB>, S<Extensive> l) const {
          out << "\nTEST END\n\n";
          out << number_errors << " error";
          if (number_errors != 1) out << "s";
          out << ",\n";
          out << number_testobjects << " testobject";
          if (number_testobjects != 1) out << "s";
          out << ". \n";
          out << "\nElapsed system time: " << system_time << "s\n";
          out << "Elapsed total time: " << total_time << "s\n";
          closing(out, l);
        }

        void print(std::ostream& out, L<de_DE>, S<Basic> l) const {
          out << number_errors << " Fehler.\n";
          closing(out, l);
        }
        void print(std::ostream& out, L<de_DE>, S<Full> l) const {
          out << "\nTESTENDE\n";
          out << number_errors << " Fehler.";
          closing(out, l);
        }
        void print(std::ostream& out, L<de_DE>, S<Extensive> l) const {
          out << "\nTESTENDE\n\n";
          out << number_errors << " Fehler,\n";
          out << number_testobjects << " Testobjekt";
          if (number_testobjects != 1) out << "e";
          out << ". \n";
          out << "\nVerbrauchte Systemzeit: " << system_time << "s\n";
          out << "Verbrauchte Gesamtzeit: " << total_time << "s\n";
          closing(out, l);
        }

      };

      // ##################################################

      struct BannerSingle {
        const char banner_character;
        const unsigned int number_banner_characters;
        BannerSingle() :  banner_character('#'), number_banner_characters(40) {}
        BannerSingle(const char banner_character, const unsigned int number_banner_characters) : banner_character(banner_character), number_banner_characters(number_banner_characters) {}
        void operator()(std::ostream& out) const {
          out << std::string(number_banner_characters, banner_character);
        }
      };

      struct SingleTestOpening : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type count;

        SingleTestOpening(const size_type count) : count(count) {}

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "\n" << "Test No. " << count << std::endl;
        }
        void print(std::ostream& out, L<en_GB>, S<Extensive>) const {
          out << "\n";
          BannerSingle()(out);
          out << "\n" << "Test No. " << count << std::endl;
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {}
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          out << "\n" << "Test Nr. " << count << std::endl;
        }
        void print(std::ostream& out, L<de_DE>, S<Extensive>) const {
          out << "\n";
          BannerSingle()(out);
          out << "\n" << "Test Nr. " << count << std::endl;
        }

      };

      // ##################################################

      struct SingleTestClosing : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        typedef ::OKlib::TestSystem::RunTest::container_type::size_type size_type;
        const size_type count;
        const bool failed;

        SingleTestClosing(const size_type count, const bool failed) : count(count), failed(failed) {}

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          out << "Test No. " << count << " ";
          if (failed)
            out << "FAILED";
          else
            out << "SUCCEEDED";
          out << std::endl;
        }
        void print(std::ostream& out, L<en_GB>, S<Extensive>) const {
          out << "Test No. " << count << " ";
          if (failed)
            out << "FAILED (error found)";
          else
            out << "SUCCEEDED (no error found)";
          out << "\n";
          BannerSingle()(out);
          out << std::endl;
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {}
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          out << "Test Nr. " << count << " ";
          if (failed)
            out << "FEHLGESCHLAGEN";
          else
            out << "ERFOLGREICH";
          out << std::endl;
        }
        void print(std::ostream& out, L<de_DE>, S<Extensive>) const {
          out << "Test No. " << count << " ";
          if (failed)
            out << "FEHLGESCHLAGEN (ein Fehler wurde gefunden)";
          else
            out << "ERFOLGREICH (kein Fehler wurde gefunden)";
          out << "\n";
          BannerSingle()(out);
          out << std::endl;
        }

      };

    }

  }

}

#endif
