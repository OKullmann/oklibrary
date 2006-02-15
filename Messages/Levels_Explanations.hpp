// Oliver Kullmann, 14.2.2006 (Swansea)

/*!
  \file Levels_Explanations.hpp
  \brief Translations of level names (provided by a message class).
*/

#ifndef LEVELSEXPLANATIONS_khgFFr4

#define LEVELSEXPLANATIONS_khgFFr4

#include "MessagesMain.hpp"

namespace OKlib {

  namespace Messages {

    namespace Documentation {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(LevelNames) {
        OKLIB_MESSAGES_PRINT

        LevelNames(const ::OKlib::Messages::Strata lev) : lev(lev) {}
        LevelNames() : lev(::OKlib::Messages::Strata(0)) {}

        ::OKlib::Messages::Strata other_level() const { return lev; }
        ::OKlib::Messages::Strata& other_level() { return lev; }
        
        template <class Stream>
          void print(Stream& out, L<en_GB>, S<Basic>) const {
          switch (lev) {
          case Basic :
            out << "basic level"; break;
          case Full :
            out << "full level"; break;
          case Extensive :
            out << "extensive level"; break;
          default :
            out << "unexplained level"; break;
          }
        }
        template <class Stream>
          void print(Stream& out, L<en_GB>, S<Full>) const {
          switch (lev) {
          case Basic :
            out << "basic level (short, but still complete)"; break;
          case Full :
            out << "full level (explicitely stating everything relevant)"; break;
          case Extensive :
            out << "extensive level (adding to the full level examples and further explanations, considering also more special cases)"; break;
          default :
            out << "unexplained level (constant with value " << lev << " not yet handled in switch statement)";
          }
        }

        template <class Stream>
          void print(Stream& out, L<de_DE>, S<Basic>) const {
          switch (lev) {
          case Basic :
            out << "Basisniveau"; break;
          case Full :
            out << "Normalniveau"; break;
          case Extensive :
            out << "Ausf\xFC" "hrliches Niveau"; break;
          default :
            out << "Undokumentiertes Niveau"; break;
          }
        }
        template <class Stream>
          void print(Stream& out, L<de_DE>, S<Full>) const {
          switch (lev) {
          case Basic :
            out << "Basisniveau (kurz und b\xFC" "ndig)"; break;
          case Full :
            out << "Normalniveau (Vollst\xE4" "ndigkeit angestrebt)"; break;
          case Extensive :
            out << "Ausf\xFC" "hrliches Niveau (auch mit Beispielen und weiteren Erkl\xE4" "rungen)"; break;
          default :
            out << "Undokumentiertes Niveau (Konstante mit Wert " << lev << " wird noch nicht in der entsprechenden Auswahlanweisung behandelt)"; break;

          }
        }

      private :
        
        ::OKlib::Messages::Strata lev;

      };

    }

  }

}

#endif
