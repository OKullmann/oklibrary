// Oliver Kullmann, 1.10.2002 (Swansea)

// Name: HashMD5.hpp
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

  int i;
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
  for (i = 0; i < mhash_get_block_size(MHASH_MD5); i++) {
    sout.width(2); sout << (unsigned int) hash[i];
  }
  
  return sout.str();
}
