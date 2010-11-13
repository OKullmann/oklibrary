// Oliver Kullmann, 21.11.2001 (Swansea)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// Name: HashMD5.cpp
// Autor: Oliver Kullmann (O.Kullmann@Swansea.ac.uk)
// Version: 0.9
// Datum: 21.1.2001 
// Aufruf: HashMD5
// liest von der Standardeingabe bis EOF, und gibt den
// Hashwert aus (auf die Standardausgabe, als Hexadezimalwert).
// Entnommen der Man-Seite von mhash.

#include <mhash.h>
#include <stdio.h>         
#include <stdlib.h>  

int main(void)         {
  MHASH td;
  unsigned char buffer;
  unsigned char *hash;  

  td = mhash_init(MHASH_MD5);  

  if (td == MHASH_FAILED) exit(1);  

  while (fread(&buffer, 1, 1, stdin) == 1) {
    mhash(td, &buffer, 1);
  }
  hash = (unsigned char *) mhash_end(td);
 
  printf("Hash:");
  for (unsigned int i = 0; i < mhash_get_block_size(MHASH_MD5); ++i) {
    printf("%.2x", hash[i]);
  }
  printf("\n");
  
  exit(0);
}
