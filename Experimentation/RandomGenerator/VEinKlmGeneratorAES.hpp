// Oliver Kullmann, 25.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/VEinKlmGeneratorAES.hpp
  \brief Definitions regarding the generator for single non-boolean clause-sets
*/


#ifndef VEINKLMGENERATORAESWAECHTER

#include "VLiteralGeneratorAES.hpp"

namespace VEKLMAES {

  using namespace VLGAES;

  void setzenParameter(const char *Klauselanfang, const char *Klauselende, const char *Literaltrenner, const char *Literalanfang, const char *Literalende, const char *Werttrenner);

  void setzenXML();

  int VEinfKlm(const Wort64 Schluessel, const UWort32 Nummer, const UWort32 n, const UWort32 d, const UWort32 p, const UWort32 c);

}

#endif

#define VEINKLMGENERATORAESWAECHTER

