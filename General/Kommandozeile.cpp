// Oliver Kullmann, 19.1.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// Name: Kommandozeile.cpp

// Copyright Oliver Kullmann, 19.1.2002
// O.Kullmann@Swansea.ac.uk

/*
   -------------------------------------------------------------------------
   Copyright (c) 2002, Dr Oliver Kullmann <O.Kullmann@Swansea.ac.uk>, Swansea, UK.
   All rights reserved.
   
   TERMS

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted subject to the following conditions:

   1. Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer. 

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution. 

   3. The copyright holder's name must not be used to endorse or promote 
      any products derived from this software without his specific prior 
      written permission. 

   This software is provided 'as is' with no express or implied warranties 
   of correctness or fitness for purpose.
   -------------------------------------------------------------------------
*/


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>

#include <OKlib/General/Kommandozeile.hpp>

using namespace std;

namespace {

  const char * const Selbst = "Kommandozeile";

  const int AnzahlSprachen = 2;

  const char * const Meldungen [][AnzahlSprachen] = {
    {"FEHLER[%s]: Die Setzung von Argument %s (%s) ergab einen Fehler!\n", // 0
     "ERROR[%s]: The placement of argument %s (%s) resulted in an error!\n"}, 
    {"FEHLER[%s]: %s\n", // 1
     "ERROR[%s]: %s\n"},
    {"FEHLER[%s]: Die Bearbeitung von Argument %s (%s) ergab einen Fehler!\n", // 2
     "ERROR[%s]: The execution of argument %s (%s) resulted in an error!\n"},
    {"FEHLER[%s]: Das Argument %s wurde nicht erkannt (vermutlich meinten Sie %s).\n", // 3
     "ERROR[%s]: Argument %s was not recognised (likely you meant %s).\n"},
    {"FEHLER[%s]: Das Argument %s wurde nicht erkannt (vermutlich meinten Sie %s=...).\n", // 4
     "ERROR[%s]: Argument %s was not recognised (likely you meant %s=...).\n"},
    {"FEHLER[%s]: Der Parameter im Argument %s ist zu gross!\n", // 5
     "ERROR[%s]: The parameter in argument %s is too big!\n"},
    {"FEHLER[%s]: Der Parameter im Argument %s ist zu klein!\n", // 6
     "ERROR[%s]: The parameter in argument %s is too small!\n"},
    {"FEHLER[%s]: Das Argument %s wurde nicht erkannt!\n", // 7
     "ERROR[%s]: Argument %s was not recognised!\n"},
    {"FEHLER[%s]: Das Argument %s stellt keine Zahl dar!\n", // 8
     "ERROR[%s]: Argument %s is not a number!\n"},
    {"FEHLER[%s]: Der Absolutbetrag des Parameters im Argument %s ist zu gross!\n", // 9
     "ERROR[%s]: The absolut value of the parameter in argument %s is too big!\n"},
    {"Die Programmoptionen sind (Kurz- und Langform):\n", // 10
     "The program options are (short form and long form):\n"},
    {"Neben der Hilfeoption (\"--help\" oder \"-h\") gibt es die Infooption (\"--info\"), die bedingt, dass alle folgenden Programmoptionen anstelle ihrer Aktionen eine Ausgabe eines erklaerenden Textes ergeben (mit einer weiteren Anwendungen von \"--info\" wird dieser Info-Modus wieder verlassen).\n", // 11
     "Besides the help option (\"--help\" or \"-h\") there is the info option (only long form \"--info\") with the result, that all subsequent program options do not lead to any actions but an explanation (a second application of \"--info\" switches off this info mode).\n"},
    {"FEHLER[%s]: Keine negativen Parameterwerte sind erlaubt in %s!\n", // 12
     "ERROR[%s]: No negative parameter values are allowed in %s!\n"},
    {"Liste der moeglichen Programm-Parameter und Optionen.", // 13
     "List of possible program parameters and options."},
  };

}


namespace {

  const char * Praefix(const char *a, const char *b) {
    // wenn Zeichenkette a ein Praefix von b ist, dann wird ein Zeiger auf
    // die erste Position in b "nach a" zurueckgegeben, andernfalls 0
    size_t L = strlen(a);
    if (strncmp(b, a, L) == 0)
      return b + L;
    else
      return 0;
  }

  /*!
    \brief Interpretes an escaped sequence, so that for example the string
    'a\n' (taken literally!) becomes interpreted (changing its length
    accordingly from 3 to 2).
    \todo Should be moved to a string module.
    \todo Needs testing!
  */

  const char* Ersatzdarstellung(const char* const a) {
    // 
    char* const b = new char[strlen(a)+1];
    char* p2 = b;
    bool GSS = false; // true iff we have to interprete the next character
    for (const char* p = a; *p != '\0'; ++p) {
      char c(*p);
      if (GSS) {
        GSS = false;
        switch (c) {
        case '\\' : c = '\\'; break;
        case 'a' : c = '\a'; break;
        case 'b' : c = '\b'; break;
        case 'f' : c = '\f'; break;
        case 'n' : c = '\n'; break;
        case 'r' : c = '\r'; break;
        case 't' : c = '\t'; break;
        case 'v' : c = '\v'; break;
        default : *p2++ = '\\'; break; // case of uninterpretable '\c'
        }
        *p2++ = c;
      }
      else
        if (c == '\\')
          GSS = true;
        else
          *p2++ = c;
    }
    if (GSS)
      *p2++ = '\\';
    *p2 = '\0';
    return const_cast<char*>(b);
  }

}

namespace KMZ {

  bool ArgumentBearbeitung(const char * const Name, const Argumentstruktur ST[], const int AnzAlternativen, const char * const A[], const int AnzahlArgumente, const Sprachen& S) {

    bool Infomodus = false;
    
    for (int i = 0; i < AnzahlArgumente; i++) {

      if ((strcmp(A[i], "--help") == 0) || (strcmp(A[i], "-h") == 0)) {
        if (Infomodus)
          printf("\n%s (%s): %s\n", "--help", "-h", Meldungen[13][S]);
        else {
          printf(Meldungen[10][S]);
          for (int j = 0; j < AnzAlternativen; j++) {
            const char *Zusatz;
            if (ST[j].typ != KEin)
              Zusatz = "=";
            else
              Zusatz = "";
            printf("%s%s\t\t%s%s\n", ST[j].kurz, Zusatz, ST[j].lang, Zusatz);
          }
          printf(Meldungen[11][S]);
        }
        continue;
      }
      else if (strcmp(A[i], "--info") == 0) {
        Infomodus = ! Infomodus;
        continue;
      }

      // Bestimmung der Option
      int j = 0;
      const char* Pos = 0; // erste Position nach der Options-Zeichenkette
      for (; j < AnzAlternativen; ++j)
        if ((Pos = Praefix((char *) ST[j].kurz, (char *) A[i])) || (Pos = Praefix((char *)ST[j].lang, (char *) A[i])))
          break;
      if (j == AnzAlternativen) { // Argument nicht erkannt
        fprintf(stderr, Meldungen[7][S], Name, A[i]);
        return false;
      }

      if (Infomodus) {
        const char *Zusatz;
        if (ST[j].typ != KEin)
          Zusatz = "=";
        else
          Zusatz = "";
        printf("\n%s%s (%s%s): ", ST[j].lang, Zusatz, ST[j].kurz, Zusatz);
        ST[j].e();
        printf("\n");
        continue;
      }

      GenArg Arg; GenArg *Z; // Einlesen der Zusatz-Argumente
	
      if (ST[j].typ == KEin) {
        if (*Pos != 0) {
          fprintf(stderr, Meldungen[3][S], Name, A[i], ST[j].lang);
          return false;
        }
        else
          Z = 0;
      }
      else {
        if (*(Pos++) != '=') {
          fprintf(stderr, Meldungen[4][S], Name, A[i], ST[j].lang);
          return false;
        }
        Z = &Arg;
        const char * Restzeiger;
        switch (ST[j].typ) {
	  
        case ZEin : Arg.s = Ersatzdarstellung(Pos); break;
	  
        case LIEin : Arg.li = strtol(Pos, (char **) &Restzeiger, 0);
          if (errno == ERANGE) {
            if (Arg.li > 0)
              fprintf(stderr, Meldungen[5][S], Name, A[i]);
            else
              fprintf(stderr, Meldungen[9][S], Name, A[i]);
            return false;
          }
          if (*Restzeiger != 0) {
            fprintf(stderr, Meldungen[8][S], Name, A[i]);
            return false;
          }
          break;
	  
        case ULIEin : if (*Pos == '-') {
            fprintf(stderr, Meldungen[12][S], Name, A[i]);
            return false;
          }
          Arg.uli = strtoul(Pos, (char **) &Restzeiger, 0);
          if (errno == ERANGE) {
            fprintf(stderr, Meldungen[5][S], Name, A[i]);
            return false;
          }
          if (*Restzeiger != 0) {
            fprintf(stderr, Meldungen[8][S], Name, A[i]);
            return false;
          }
          break;
	  
        default : Arg.d = strtod(Pos, 0); // case DEin
          if (errno == ERANGE) {
            if (Arg.li != 0)
              fprintf(stderr, Meldungen[5][S], Name, A[i]);
            else
              fprintf(stderr, Meldungen[6][S], Name, A[i]);
            return false;
          }
          break;
	  
        } // switch
      }// der Fall, dass ein Argument zu bearbeiten war

      const char *Ausgabe ;
      if (ST[j].f != 0 && ! ((ST[j].f)(Z, &Ausgabe))) {
        fprintf(stderr, Meldungen[0][S], Name, ST[j].lang, A[i]);
        fprintf(stderr, Meldungen[1][S], Name, Ausgabe);
        return false;
      }
      if (ST[j].a != 0 && ! ((ST[j].a)(Z))) {
        fprintf(stderr, Meldungen[2][S], Name, ST[j].lang, A[i]);
        return false;
      }
    }
    return true;
  }
  
}
