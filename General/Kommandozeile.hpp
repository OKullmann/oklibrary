// Oliver Kullmann, 19.1.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/Kommandozeile.hpp
  \brief Header file for the old program-options-scaffolding.
  \deprecated To be dissolved in module OKlib/ProgramOptions.
*/

// Name: Kommandozeile.hpp

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


#ifndef KOMMANDOZEILEWAECHTER

namespace KMZ {

  typedef union {
    const char * s;
    long int li;
    unsigned long int uli;
    double d;
  } GenArg; // generisches Argument


  typedef bool Fehler(const GenArg * const, const char ** const);
  // Ueberpruefung des Argumentes
  // falls vorhanden: liefert false, falls Argument fehlerhaft,
  // und legt im zweiten Argument einen Fehlertext ab, sonst true;

  typedef bool Aktion(const GenArg * const);
  // Durchfuehrung einer Aktion gemaess dem Parameter;
  // gibt false zurueck im Falle des Scheitern der Aktion

  typedef void Erklaerung();
  
  

  typedef enum {ZEin, LIEin, ULIEin, DEin, KEin} Argumenttyp;
  // Z   : Zeichenkette
  // LI  : long int
  // ULI : unsigned long int
  // D   : double
  // K   : kein Argument

  typedef enum {Deutsch = 0, Englisch = 1} Sprachen;

  struct Argumentstruktur {
    const char * const kurz;
    const char * const lang;
    const Argumenttyp typ;
    // die folgenden drei Elemente koennen mit 0 belegt werden, falls
    // nicht vorhanden
    Fehler * const f; 
    Aktion * const a;
    Erklaerung * const e;
  };

  bool ArgumentBearbeitung(const char * const, const Argumentstruktur [], const int, const char * const [], const int, const Sprachen&);
  // Rueckgabe ist true, falls kein Fehler auftrat

}

#endif

#define KOMMANDOZEILEWAECHTER

