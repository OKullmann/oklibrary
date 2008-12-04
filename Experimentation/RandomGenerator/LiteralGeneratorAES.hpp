// Oliver Kullmann, 16.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/LiteralGeneratorAES.hpp
  \brief Definitions regarding the basic literal generator
*/


#ifndef LITERALGENERATORAESWAECHTER

#include <climits>

namespace LGAES {

#if UINT_MAX == 4294967295

  typedef int SWort32;
  typedef unsigned int UWort32;

#  elif ULONG_MAX == 4294967295

  typedef long int SWort32;
  typedef unsigned long int UWort32;

#  else

#    error "Es kann kein 32-Bit-Integer-Typ gefunden werden!"

#  endif

  typedef UWort32 Wort64[2];

  SWort32 Literal(const Wort64 Schluessel, const Wort64 FormelNummer, const SWort32 Variablenzahl, const SWort32 Klausellaenge, const Wort64 Literalzaehler);

}

#endif

#define LITERALGENERATORAESWAECHTER

