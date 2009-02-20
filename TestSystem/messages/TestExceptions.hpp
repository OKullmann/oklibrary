// Oliver Kullmann, 17.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/messages/TestExceptions.hpp
  \brief Messages related to test-failure notification
*/

#ifndef TESTEXCEPTIONSMESSAGES_jJbv448865RtFd
#define TESTEXCEPTIONSMESSAGES_jJbv448865RtFd

#include <string>
#include <cassert>
#include <tr1/memory>
#include <stdexcept>
#include <vector>

#include <OKlib/Programming/SystemSpecifics/Demangling.hpp>
#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>
#include <OKlib/Programming/Messages/Utilities/FileIdentification.hpp>
#include <OKlib/Programming/Messages/Utilities/LineIdentification.hpp>

#include <OKlib/TestSystem/BasicDeclarations.hpp>
#include <OKlib/TestSystem/messages/TestLevel.hpp>
#include <OKlib/TestSystem/messages/TestBaseClass.hpp>

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

      // ######################################################


      /*!
        \class TestException
        \brief The root of the exception class hierarchy, to be thrown in case of test failure.
        
        A message class, derived from std::runtime_error.
        Contains a container of error descriptions, the first one being the actual error description,
        followed by descriptions for unwinding the call-stack.
        Should normally not be used directly, but is invoked by OKLIB_THROW and derived macros
        (like OKLIB_TEST_EQUAL).

        \todo Provide different output-languages.
      */

      class TestException : public std::runtime_error, public ::OKlib::Messages::MessagesPrePost {
        //! Vector of error descriptions. Assumes, that the destructor of std::vector does not throw.
        typedef std::vector< ::OKlib::TestSystem::messages::ErrorDescription> ErrorContainer; 
        ErrorContainer errors;
        typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;
        MessagePointer additional_circumstances;
      public :
        OKLIB_MESSAGES_PRINT
        TestException(
                      const std::string& circumstances,
                      ::OKlib::Messages::MessagesPrePost* const additional_circumstances_ = 0
                      ) :
          std::runtime_error(circumstances),
          additional_circumstances(additional_circumstances_)
        {}
        ~TestException() throw() {}
        
        TestException& add(const ::OKlib::TestSystem::messages::ErrorDescription e) {
          errors.push_back(e);
          return *this;
        }
        
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          typedef ErrorContainer::size_type size_type;
          l_start(out); l_end(out);
          l_start(out) << "EXCEPTION THROWN:"; l_end(out);
          l_start(out) << " error message = " << what(); l_end(out);
          l_start(out) << " stack trace (";
          {
            const size_type size(errors.size());
            out << size << " element";
            if (size != 1) out << "s";
          }
          out << "):"; l_end(out);
          typedef ErrorContainer::const_iterator iterator;
          const iterator& end(errors.end());
          size_type counter = 0;
          for (iterator i(errors.begin()); i != end; ++i, ++counter) {
            l_start(out) << "Error description " << counter << ":"; l_end(out);
            out << ::OKlib::TestSystem::messages::ErrorDescription(*i).cp_pp(*this);
          }
        }

        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          typedef ErrorContainer::size_type size_type;
          l_start(out); l_end(out);
          l_start(out) << "TEST EXCEPTION THROWN:"; l_end(out);
          l_start(out) << " short error message = " << what(); l_end(out);
          if (additional_circumstances.get()) {
            l_start(out) << " additional error circumstances :"; l_end(out);
            out << additional_circumstances -> cp_pp(*this);
          }
          l_start(out) << " stack trace call-history (with ";
          {
            const size_type size(errors.size());
            out << size << " element";
            if (size != 1) out << "s";
          }
          out << "):"; l_end(out);
          typedef ErrorContainer::const_iterator iterator;
          const iterator& end(errors.end());
          size_type counter = 0;
          for (iterator i(errors.begin()); i != end; ++i, ++counter) {
            l_start(out) << "Error description " << counter << ":"; l_end(out);
            out << ::OKlib::TestSystem::messages::ErrorDescription(*i).cp_pp(*this);
          }
          l_start(out) << "END TEST EXCEPTION THROWN."; l_end(out);
        }

      };

      // ######################################################


      /*!
        \class NotEqual
        \brief Output of streamable objects which are not equal (unexpectedly)

        \todo Provide different output-languages.
      */

      template <typename T1, typename T2>
      struct NotEqual : OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT
        const T1& a, b;
        NotEqual(const T1& a, const T2& b) : a(a), b(b) {}
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          l_start(out) << "a != b, where"; l_end(out);
          l_start(out) << "a = " << a; l_end(out);
          l_start(out) << "b = " << b; l_end(out);
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          l_start(out) << "Test \"a == b\" returned false, where a is the tested value, b the expected value, and where"; l_end(out);
          l_start(out) << "a = " << a; l_end(out);
          l_start(out) << "b = " << b; l_end(out);
        }
      };
      //! Helper function template: guaranteeing that a,b are evaluated only once, returning a pointer which is null if the test succeeded
      template <typename T1, typename T2>
      inline NotEqual<T1, T2>* test_not_equal(const T1& a, const T2& b) {
        if (not (a == b)) return new NotEqual<T1,T2>(a,b);
        else return 0;
      }

    }
  }
}

#endif
