// Oliver Kullmann, 19.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/messages/TestBaseClass.hpp
  \brief Messages for class OKlib::TestSystem::TestBase
*/

#ifndef TESTBASECLASSMESSAGES_jjdh6648
#define TESTBASECLASSMESSAGES_jjdh6648

#include <cassert>
#include <string>
#include <tr1/memory>

#include <OKlib/Programming/SystemSpecifics/Demangling.hpp>
#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>
#include <OKlib/Programming/Messages/Utilities/FileIdentification.hpp>
#include <OKlib/Programming/Messages/Utilities/LineIdentification.hpp>

#include <OKlib/TestSystem/BasicDeclarations.hpp>
#include <OKlib/TestSystem/TestFondement.hpp>
#include <OKlib/TestSystem/messages/TestLevel.hpp>

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

        \todo Once the message-construction is more powerful, generalised the handling
        of the different levels and languages accordingly:
        <ul>
         <li> Heading not needed at level Basic. </li>
         <li> Ending not needed at levels Basic and Full. </li>
         <li> The template parameter for the main print-function should go. </li>
        </ul>
      */

      struct LogDescription : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

      private :

        typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;

        MessagePointer test_description;
        MessagePointer file;
        MessagePointer line;

        struct Heading : ::OKlib::Messages::MessagesPrePost {
          OKLIB_MESSAGES_PRINT
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
          void print(std::ostream& out, L<en_GB>, S<Full>) const {
            l_start(out) << "LOG MESSAGE; identification:"; l_end(out);
          }
          void print(std::ostream& out, L<en_GB>, S<Extensive>) const {
            l_start(out) << "LOG MESSAGE"; l_end(out);
            l_start(out) << "identification:"; l_end(out);
          }
          void print(std::ostream& out, L<de_DE>, S<Full>) const {
            l_start(out) << "LOG-MELDUNG; Identifizierung:"; l_end(out);
          }
          void print(std::ostream& out, L<de_DE>, S<Extensive>) const {
            l_start(out) << "LOG-MELDUNG"; l_end(out);
            l_start(out) << "Identifizierung:"; l_end(out);
          }
        };
        struct Ending : ::OKlib::Messages::MessagesPrePost {
          OKLIB_MESSAGES_PRINT
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {}
          void print(std::ostream& out, L<en_GB>, S<Full>) const {}
          void print(std::ostream& out, L<en_GB>, S<Extensive>) const {
            l_start(out) << "message:"; l_end(out);
          }
        };

      public :

        LogDescription(
                         const ::OKlib::TestSystem::messages::BasicTestDescription& test,
                         ::OKlib::Messages::Utilities::FileIdentification* const file_,
                         ::OKlib::Messages::Utilities::LineIdentification* const line_
                         ) :
          test_description(new ::OKlib::TestSystem::messages::BasicTestDescription(test)),
          file(file_),
          line(line_)
        {
          assert(test_description.get());
          assert(file.get());
          assert(line.get());
        }
        
        template <class L>
        void print(std::ostream& out, L, S<Basic>) const {}
        template <class L>
        void print(std::ostream& out, L, S<Full>) const {
          assert(test_description.get());
          assert(file.get());
          assert(line.get());
          out << Heading().cp_pp(*this);
          assert(::OKlib::Messages::MessagesBase::level(out) == Full);
          ::OKlib::Messages::MessagesBase::set(out, Basic);
          LogDescription temp(*this);
          const int indent = 1;
          temp.prefix() += std::string(indent, ' ');
          out << line -> cp_pp(temp); l_end(out);
          out << file -> cp_pp(temp); l_end(out);
          out << test_description -> cp_pp(temp);
          ::OKlib::Messages::MessagesBase::set(out, Full);
        }
        template <class L>
        void print(std::ostream& out, L, S<Extensive>) const {
          assert(test_description.get());
          assert(file.get());
          assert(line.get());
          out << Heading().cp_pp(*this);
          {
            LogDescription temp(*this);
            const int indent = 1;
            temp.prefix() += std::string(indent, ' ');
            out << line -> cp_pp(temp); l_end(out);
            out << file -> cp_pp(temp); l_end(out);
            out << test_description -> cp_pp(temp);
          }
          out << Ending().cp_pp(*this);
        }

      };

    }

  }

}

#endif
