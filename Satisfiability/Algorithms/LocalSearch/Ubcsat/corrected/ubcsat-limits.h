/*!
  \file LocalSearch/Ubcsat/corrected/ubcsat-limits.h
  \brief Some constants have too small default values; also added include-guard.
*/

/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...Developed by Dave Tompkins (davet [@] cs.ubc.ca)...
  ------------------------------------------------------
  .......consult legal.txt for legal information........
  ......consult revisions.txt for revision history......
  ------------------------------------------------------
  ... project website: http://www.satlib.org/ubcsat ....
  ------------------------------------------------------
  .....e-mail ubcsat-help [@] cs.ubc.ca for support.....
  ------------------------------------------------------

*/

#ifndef UBCSATLIMITS_hasvc31er2
#define UBCSATLIMITS_hasvc31er2

#define DEFAULTHEAPSIZE 4194304
#define MAXHEAPS 1024

#define LITSPERCHUNK 262144

#define MAXNUMALG 128
#define MAXALGPARMS 16

#define MAXFXNLIST 32

#define MAXCNFLINELEN 8192

#define MAXREPORTS 64
#define MAXREPORTPARMS 8

#define MAXPARMLINELEN 8192
#define MAXTOTALPARMS 128

#define MAXITEMLIST 256
#define MAXITEMLISTSTRINGLENGTH 128

#define MAXREPORTHEADERSTRING 256

#endif
