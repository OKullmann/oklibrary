// Oliver Kullmann, 14.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/messages/Levels.hpp
  \brief Translations of level names (provided by a message class).

  Defines class Messages::messages::LevelNames for output of level descriptions.
*/

#ifndef LEVELSEXPLANATIONS_khgFFr4
#define LEVELSEXPLANATIONS_khgFFr4

#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LineHandling.hpp>

namespace OKlib {
  namespace Messages {
    namespace messages {

      /*!
        \class LevelNames
        \brief Outputs a description for all provided levels

        Constructed with a level-constant.
      */

      OKLIB_USING_MESSAGES

      struct LevelNames : ::OKlib::Messages::MessagesPrePost {
        OKLIB_MESSAGES_PRINT

        explicit LevelNames(const ::OKlib::Messages::Strata lev = ::OKlib::Messages::Strata(0)) : lev(lev) {}

        ::OKlib::Messages::Strata other_level() const { return lev; }
        ::OKlib::Messages::Strata& other_level() { return lev; }
        
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          l_start(out);
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
          l_start(out);
          switch (lev) {
          case Basic :
            out << "basic level (short, but still complete)"; break;
          case Full :
            out << "full level (explicitly stating everything relevant)"; break;
          case Extensive :
            out << "extensive level (adding to the full level examples and further explanations, considering also more special cases)"; break;
          default :
            out << "unexplained level (constant with value " << lev << " not yet handled in switch statement)";
          }
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          l_start(out);
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
          l_start(out);
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
