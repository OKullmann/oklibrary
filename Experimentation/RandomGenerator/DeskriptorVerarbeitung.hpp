// Oliver Kullmann, 2.4.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/DeskriptorVerarbeitung.hpp
  \brief Definitions regarding the experiment descriptors
*/
           

#ifndef DESKRIPTORVERARBEITUNGWAECHTER


#include <istream>
#include <ostream>

#include <string>

// if SELBSTSTAENDIG is defined, then there is a main program;

// if NODATABASE is defined, then access to the database is not provided,
// and the use has to specify seed or formula-number;

namespace Deskriptorverarbeitung {

  namespace Fehler {

    using namespace std;

    struct DeskriptorVerarbeitung {
      virtual void Ausgabe() const;
    };

    struct Syntaxfehler : DeskriptorVerarbeitung {
      string M, A; // Meldung und Argument
      Syntaxfehler(string M2, string A2) : M(M2), A(A2) { }
      void Ausgabe() const;
    };
    
    struct DBFehler : DeskriptorVerarbeitung { // Fehler im Umgang mit OKdatabase
      string M;
      DBFehler(string M2) : M(M2) { }
      void Ausgabe() const;
    };

    struct Bearbeitungsfehler : DeskriptorVerarbeitung {
      string M, A; // Meldung und Argument
      Bearbeitungsfehler(string M2, string A2) : M(M2), A(A2) { }
      void Ausgabe() const;
    };

    struct NullNenner : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };

    struct NullSchrittweite : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };

    struct l_unspezifiziert : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };

    struct n_unspezifiziert : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };

    struct Dateifehler : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };
    struct Eingabestrom : Dateifehler {
      void Ausgabe() const;
    };
    struct Ausgabestrom : Dateifehler {
      void Ausgabe() const;
    };
    struct Lesefehler : Dateifehler {
      void Ausgabe() const;
    };

    struct seed_and_nr_unspecified : DeskriptorVerarbeitung {
      void Ausgabe() const;
    };
    // only used if NODATABASE is defined
    
  }

  void Verarbeitung (std::istream& Ein, std::ostream& Aus, const std::string& generator = "OKgenerator");
  // Attention: After return Ein.fail() is true!
  // by default the current key is requested from the database;

}

#endif

#define DESKRIPTORVERARBEITUNGWAECHTER

