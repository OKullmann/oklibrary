// Oliver Kullmann, 14.2.2006 (Swansea)

/*!
  \file Languages_Explanations.hpp
  \brief Translations of language names (provided by a message class).
  \todo "Austria" in French?
  \todo "Untranslated" in French?
*/

#ifndef LANGUAGESEXPLANATIONS_8Uh26266

#define LANGUAGESEXPLANATIONS_8Uh26266

#include "MessagesMain.hpp"

namespace OKlib {

  namespace Messages {

    namespace Documentation {

      OKLIB_USING_MESSAGES

      OKLIB_MESSAGES(LanguageNames) {
        OKLIB_MESSAGES_PRINT

        LanguageNames(const ::OKlib::Messages::Languages lang) : lang(lang) {}
        LanguageNames() : lang(::OKlib::Messages::Languages(0)) {}

        ::OKlib::Messages::Languages foreign_language() const { return lang; }
        ::OKlib::Messages::Languages& foreign_language() { return lang; }
        
          void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "English"; break;
          case en_US :
            out << "English (USA)"; break;
          case en_CA :
            out << "English (Canada)"; break;
          case en_AU :
            out << "English (Australia)"; break;
          case fr_FR :
            out << "French"; break;
          case fr_CH :
            out << "French (Swiss)"; break;
          case de_DE :
            out << "German"; break;
          case de_AT :
            out << "German (Austria)"; break;
          case de_CH :
            out << "German (Swiss)"; break;
          default :
            out << "Untranslated"; break;
          }
        }

          void print(std::ostream& out, L<fr_FR>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "Anglais"; break;
          case en_US :
            out << "Anglais (USA)"; break;
          case en_CA :
            out << "Anglais (Canada)"; break;
          case en_AU :
            out << "Anglais (Australie)"; break;
          case fr_FR :
            out << "Francais"; break;
          case fr_CH :
            out << "Francais (Suisse)"; break;
          case de_DE :
            out << "Allemand"; break;
          case de_AT :
            out << "Allemand (Austria ??? )"; break;
          case de_CH :
            out << "Allemand (Suisse)"; break;
          default :
            out << "???"; break;
          }
        }

          void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "Englisch"; break;
          case en_US :
            out << "Englisch (USA)"; break;
          case en_CA :
            out << "Englisch (Kanada)"; break;
          case en_AU :
            out << "Englisch (Australien)"; break;
          case fr_FR :
            out << "Französisch"; break;
          case fr_CH :
            out << "Französisch (Schweiz)"; break;
          case de_DE :
            out << "Deutsch"; break;
          case de_AT :
            out << "Deutsch (Österreich)"; break;
          case de_CH :
            out << "Deutsch (Schweiz)"; break;
          default :
            out << "Ohne Übersetzung"; break;
          }
        }

      private :

        ::OKlib::Messages::Languages lang;

      };
    }

  }

}

#endif
