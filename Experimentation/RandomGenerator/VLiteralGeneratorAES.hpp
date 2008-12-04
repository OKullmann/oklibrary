// Oliver Kullmann, 25.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/VLiteralGeneratorAES.hpp
  \brief Definitions regarding the basic generator for non-boolean literals
*/


#ifndef VLITERALGENERATORWAECHTER

namespace VLGAES {

#include <climits>

#if UINT_MAX == 4294967295

  typedef unsigned int UWort32;

#  elif ULONG_MAX == 4294967295

  typedef unsigned long int UWort32;

#  else

#    error "Es kann kein 32-Bit-Integer-Typ gefunden werden!"

#  endif

  typedef UWort32 Wort64[2];
  // Position 0 steht fuer die niedrigwertigen Teil

  typedef struct {
    UWort32 V; // Variable
    UWort32 W; // Wert
  } VLiterale;

  VLiterale VLiteral(const Wort64 Schluessel, const UWort32 FormelNummer, const UWort32 Variablenzahl, const UWort32 Klausellaenge, const UWort32 Domaingroesse, const Wort64 Literalzaehler);

}

#endif

#define VLITERALGENERATORWAECHTER

