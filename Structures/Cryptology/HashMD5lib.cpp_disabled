// Oliver Kullmann, 1.10.2002 (Swansea)
/* Copyright 2002 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// Name: HashMD5lib.cpp
// Autor: Oliver Kullmann (O.Kullmann@Swansea.ac.uk)
// Version: 0.9
// Datum: 1.10.2002
// Entnommen der Man-Seite von mhash.

#include <mhash.h>
#include <stdio.h>         
#include <stdlib.h> 

#include <string>
#include <sstream>
#include <iomanip>

std::string hashMD5 (const char* in) {

  MHASH td;
  unsigned char* p;
  unsigned char *hash;

  td = mhash_init(MHASH_MD5);  

  if (td == MHASH_FAILED) exit(1);  

  for (p = (unsigned char*) in; *p != 0; ++p)
    mhash(td, p, 1);

  hash = (unsigned char *) mhash_end(td);

  std::ostringstream sout;
  sout.unsetf(std::ios::dec); sout.setf(std::ios::hex);
  sout.fill('0');
  for (unsigned int i = 0; i < mhash_get_block_size(MHASH_MD5); i++) {
    sout.width(2); sout << (unsigned int) hash[i];
  }
  
  return sout.str();
}
