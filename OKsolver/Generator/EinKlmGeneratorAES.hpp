// Oliver Kullmann, 18.1.2002 (Swansea)

// Name: EinKlmGeneratorAES.hpp

// Copyright Oliver Kullmann, 18.1.2002
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

