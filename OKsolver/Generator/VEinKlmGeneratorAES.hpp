// Oliver Kullmann, 25.1.2002 (Swansea)

// Name: VEinKlmGeneratorAES.hpp

// Copyright Oliver Kullmann, 25.1.2002
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

