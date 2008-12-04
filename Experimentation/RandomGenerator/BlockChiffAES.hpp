// Oliver Kullmann, 15.10.2001 (Swansea)
/* Copyright 2001 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/BlockChiffAES.hpp
  \brief Definitions related to the AES chiffre
*/

#ifndef BLOCKCHIFFAESWAECHTER


#define ANZAHLBYTESBLOCK 16

// Die beiden folgenden Tests sind noetig, um das Funktionen der
// aes-Implementation zu garantieren:

#  include <climits>

#  if CHAR_BIT != 8

#    error "ACHTUNG: Ein Byte hat mehr als 8 Bits!"

#  endif

#  if ULONG_MAX != 4294967295

// #    error "ACHTUNG: Ein unsigned long hat mehr als 32 Bits!"
// ??????????????????????????????????????????????????

#  endif


namespace BCAES {

typedef unsigned char Byte;
typedef Byte Wort_128[ANZAHLBYTESBLOCK];

void BlockChiffreAES(const Wort_128 Eingabe, const Wort_128 Schluessel, Wort_128 Ausgabe);

}

#endif

#define BLOCKCHIFFAESWAECHTER


