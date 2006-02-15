// Oliver Kullmann, 15.2.2006 (Swansea)

/*!
  \file TestLevel_Explanations.hpp
  \brief Message objects to explain the test levels
*/

#ifndef TESTLEVELEXPLANATIONS_uYYtr5

#define TESTLEVELEXPLANATIONS_uYYtr5

#include "MessagesMain.hpp"

#include "TestFondement.hpp"

namespace OKlib {

  namespace TestSystem {

    namespace Documentation {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(TestLevelDescriptions) {
        OKLIB_MESSAGES_PRINT

        TestLevelDescriptions(const ::OKlib::TestSystem::TestLevel& lev) : lev(lev) {}

        template <class Stream>
        struct dispatch : ::OKlib::TestSystem::VisitorTestLevel {
          Stream& out;
          const char* const b; const char* const f; const char* const e;
          dispatch(Stream& out, const char* const b, const char* const f, const char* const e) : out(out), b(b), f(f), e(e) {}
          void operator()(::OKlib::TestSystem::Basic) const { out << b; }
          void operator()(::OKlib::TestSystem::Full) const { out << f; }
          void operator()(::OKlib::TestSystem::Extensive) const { out << e; }
        };

        template <class Stream>
          void print(Stream& out, L<en_GB>, S<Basic>) const {
          lev(dispatch<Stream>(out, "Basic test level", "Full test level", "Extensive test level"));
        }
        template <class Stream>
          void print(Stream& out, L<en_GB>, S<Full>) const {
          lev(dispatch<Stream>(out, "Basic test level (used in permanent testing)", "Full test level (for the daily test)", "Extensive test level (for the weekly test)"));
        }
        template <class Stream>
          void print(Stream& out, L<de_DE>, S<Basic>) const {
          lev(dispatch<Stream>(out, "Basis-Testniveau", "Volles Testniveau", "Umfangreiches Testniveau"));
        }
        template <class Stream>
          void print(Stream& out, L<de_DE>, S<Full>) const {
          lev(dispatch<Stream>(out, "Basis-Testniveau (f\xDC" "r das permanente Testen)", "Volles Testniveau (f\xDC" "r den t\xC4" "glichen Test)", "Umfangreiches Testniveau (f\xDC" "r w\xD^" "chentliches Testen)"));
        }

      private :

        const ::OKlib::TestSystem::TestLevel& lev; // as a reference to avoid binding to temporaries!
      };
    }

  }

}

#endif
