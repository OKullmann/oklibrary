// Oliver Kullmann, 15.2.2006 (Swansea)

/*!
  \file Testsystem/messages/TestLevel.hpp
  \brief Message objects to explain the test levels
*/

#ifndef TESTLEVELEXPLANATIONS_uYYtr5

#define TESTLEVELEXPLANATIONS_uYYtr5

#include <Transitional/Messages/MessagesMain.hpp>

#include <Transitional/Testsystem/TestFondement.hpp>

namespace OKlib {

  namespace TestSystem {

    namespace messages {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(TestLevelDescriptions) {
        OKLIB_MESSAGES_PRINT

        TestLevelDescriptions(::OKlib::TestSystem::TestLevel& lev) : lev(lev) {}

        struct dispatch : ::OKlib::TestSystem::VisitorTestLevel {
          std::ostream& out;
          const char* const b; const char* const f; const char* const e;
          dispatch(std::ostream& out, const char* const b, const char* const f, const char* const e) : out(out), b(b), f(f), e(e) {}
          void operator()(::OKlib::TestSystem::Basic) const { out << b; }
          void operator()(::OKlib::TestSystem::Full) const { out << f; }
          void operator()(::OKlib::TestSystem::Extensive) const { out << e; }
        };

        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          lev(dispatch(out, "Basic test level", "Full test level", "Extensive test level"));
        }
        void print(std::ostream& out, L<en_GB>, S<Full>) const {
          lev(dispatch(out, "Basic test level (used for permanent testing)", "Full test level (for the daily test)", "Extensive test level (for the weekly test)"));
        }
        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          lev(dispatch(out, "Basis-Testniveau (\"basic\")", "Volles Testniveau (\"full\")", "Umfangreiches Testniveau (\"extensive\")"));
        }
        void print(std::ostream& out, L<de_DE>, S<Full>) const {
          lev(dispatch(out, "Basis-Testniveau (\"basic\"; für das laufende Testen)", "Volles Testniveau (\"full\"; z.B. für den täglichen Test)", "Umfangreiches Testniveau (\"extensive\"; z.B. für den wöchentlichen Test)"));
        }

      private :

        ::OKlib::TestSystem::TestLevel& lev; // as a non-const reference to avoid binding to temporaries!
      };
    }

  }

}

#endif
