// Oliver Kullmann, 14.2.2006 (Swansea)

/*!
  \file Messages/messages/Levels.hpp
  \brief Translations of level names (provided by a message class).
*/

#ifndef LEVELSEXPLANATIONS_khgFFr4
#define LEVELSEXPLANATIONS_khgFFr4

#include <Transitional/Messages/MessagesMain.hpp>

namespace OKlib {

  namespace Messages {

    namespace messages {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(LevelNames) {
        OKLIB_MESSAGES_PRINT

        LevelNames(const ::OKlib::Messages::Strata lev) : lev(lev) {}
        LevelNames() : lev(::OKlib::Messages::Strata(0)) {}

        ::OKlib::Messages::Strata other_level() const { return lev; }
        ::OKlib::Messages::Strata& other_level() { return lev; }
        
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {
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
          void print(std::ostream& out, L<en_GB>, S<Full>) const {
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

          void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          switch (lev) {
          case Basic :
            out << "Basisniveau"; break;
          case Full :
            out << "Normalniveau"; break;
          case Extensive :
            out << "Ausführliches Niveau"; break;
          default :
            out << "Undokumentiertes Niveau"; break;
          }
        }
          void print(std::ostream& out, L<de_DE>, S<Full>) const {
          switch (lev) {
          case Basic :
            out << "Basisniveau (kurz und bündig)"; break;
          case Full :
            out << "Normalniveau (Vollständigkeit angestrebt)"; break;
          case Extensive :
            out << "Ausführliches Niveau (auch mit Beispielen und weiteren Erklärungen)"; break;
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
