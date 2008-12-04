// Oliver Kullmann, 18.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/EinKlmGeneratorAES.hpp
  \brief Definitions regarding the generator for a single clause-set
*/


#ifndef EINKLMGENERATORAESWAECHTER

#include "LiteralGeneratorAES.hpp"

namespace EKLMAES {

  using namespace LGAES;

  void setzenParameter(const char *Klauselanfang, const char *Klauselende, const char *Literaltrenner);

  void setzenXML();

  int EinfKlm(const Wort64 Schluessel, const Wort64 Nummer, const SWort32 n, const SWort32 p, const UWort32 c);

}

#endif

#define EINKLMGENERATORAESWAECHTER

