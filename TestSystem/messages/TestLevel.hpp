// Oliver Kullmann, 15.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/messages/TestLevel.hpp
  \brief Message objects to explain the test levels
*/

#ifndef TESTLEVELEXPLANATIONS_uYYtr5
#define TESTLEVELEXPLANATIONS_uYYtr5

#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>

#include <OKlib/TestSystem/TestFondement.hpp>

namespace OKlib {

  namespace TestSystem {

    namespace messages {

      OKLIB_USING_MESSAGES

      /*!
        \class TestLevelDescriptions
        \brief Output of test-level descriptions

         Attention: The const-reference to the level object must not be bound to a temporary!
         (This leads to undefined behaviour, but compilers usually don't catch this.)

         \todo How to handle that sometimes we may want to start with "Test level" instead of "test level" ?
      */

      struct TestLevelDescriptions : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

        explicit TestLevelDescriptions(const ::OKlib::TestSystem::TestLevel& lev) : lev(lev) {}

        struct dispatch : ::OKlib::TestSystem::VisitorTestLevel {
          std::ostream& out;
          const char* const b; const char* const f; const char* const e;
          dispatch(std::ostream& out, const char* const b, const char* const f, const char* const e) : out(out), b(b), f(f), e(e) {}
          void operator()(::OKlib::TestSystem::Basic) const { out << b; }
          void operator()(::OKlib::TestSystem::Full) const { out << f; }
          void operator()(::OKlib::TestSystem::Extensive) const { out << e; }
        };

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          l_start(out) << "test level = ";
          lev(dispatch(out, "Basic test level", "Full test level", "Extensive test level"));
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          l_start(out) << "test level = ";
          lev(dispatch(out, "Basic test level (used for permanent testing)", "Full test level (for the daily test)", "Extensive test level (for the weekly test)"));
        }
        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          l_start(out) << "Testniveau = ";
          lev(dispatch(out, "Basis-Testniveau (\"basic\")", "Volles Testniveau (\"full\")", "Umfangreiches Testniveau (\"extensive\")"));
        }
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          l_start(out) << "Testniveau = ";
          lev(dispatch(out, "Basis-Testniveau (\"basic\"; für das laufende Testen)", "Volles Testniveau (\"full\"; z.B. für den täglichen Test)", "Umfangreiches Testniveau (\"extensive\"; z.B. für den wöchentlichen Test)"));
        }

      private :

        const ::OKlib::TestSystem::TestLevel& lev;
      };
    }

  }

}

#endif
