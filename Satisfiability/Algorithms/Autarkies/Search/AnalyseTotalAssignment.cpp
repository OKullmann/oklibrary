// Matthew Lewsey, 2.2.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Autarkies/Search/AnalyseTotalAssignment.cpp
  \brief Supplementing UBCSAT by analysing total assignments for
  contained autarkies.

  \bug Program is faulty:
  <code>
    > OKplatform/system_directories/bin/AnalyseTotalAssignment example.cnf
    AnalyseTotalAssignment: /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/OKsystem/OKlib/AutarkySearch/AnalyseTotalAssignment.cpp:92: void<unnamed>::AnalyseTotalAssignment(): Assertion `iCurrVar <= iNumVars' failed.
  </code>
  This was to be expected, given the low coding standard. So first the program
  has to be transferred into adequate C++, using proper modularisation and testing;
  then those problems will disappear automatically.

  \bug Code is not proper C++:
  <ul>
   <li> Use the standard library (especially std::vector). </li>
   <li> Don't use malloc etc. </li>
   <li> Use assert's to check every array-access. </li>
  </ul>

  \todo Create Doxygen documentation.

  \todo Use Messages for output.

  \todo Use ProgramOptions.
*/


#include <iostream>
#include <cassert>
#include <cstdlib>

#include <OKlib/Satisfiability/Algorithms/LocalSearch/Ubcsat/local/ubcsat.h>

namespace {

  extern "C" void ReadCNF();
  extern "C" void CreateLitOccurence();
  extern "C" void CreateCandidateList();
  extern "C" void CreateDefaultStateInfo();
  extern "C" void CreateFalseClauseList();
  extern "C" void DefaultInitVars();
  extern "C" void InitDefaultStateInfo();
  extern "C" void InitFalseClauseList();
  extern "C" void DefaultFlip();
  extern "C" void CheckTermination();
  extern "C" void UpdateFalseClauseList();
  
  void Initialise() {
    // call the relevant UBCSAT library procedures to detach this function from being inside the UBCSAT system
    ReadCNF();
    CreateLitOccurence(); // allows usage of aNumLitOcc, pLitClause
    CreateCandidateList(); // allows usage of aCandidateList
    CreateDefaultStateInfo(); // allows the usage of aVarValue array once initialised
    CreateFalseClauseList(); // allows usage of the new false clause list after variable flip
    
    // UBCSAT uses aVarInit to determine how variable i is to be assigned (2 = random)
    assert(iNumVars >= 0);
    for(UINT32 i=1; i <= UINT32(iNumVars); ++i)
      aVarInit[i] = 2;
    
    DefaultInitVars(); // initially assigns ALL variables randomnly (randomnly due to above loop)
    InitDefaultStateInfo(); // necessary to get aNumTrueLit working
    InitFalseClauseList(); // necessary to get aFalseList working
  }
  
  
  void AnalyseTotalAssignment() {
    BOOL bOneLitTrue = FALSE, bNotDone = TRUE;
    BOOL* aCurrAutarky;
    UINT32* aStack;
    UINT32 iStackSize, iCurrStackVar;
    UINT32 iCurrClauseNum, iCurrVar, iCurrLit,  iCurrFlipVarLitValue, iCurrAutarkySize, iClauseLen;
    UINT32* aLitClauses;
    
    aCurrAutarky = (BOOL *)malloc((iNumVars+1) * sizeof(BOOL)); // UNSAFE! XXX
    iStackSize = 0;
    aStack = (UINT32 *)malloc((iNumVars) * sizeof(UINT32)); // UNSAFE! XXX
    
    for(UINT32 j=0; j <= iNumVars; ++j)
      aCurrAutarky[j] = TRUE;
    
    for(UINT32 j=0; j < iNumFalse; ++j) {
      iCurrClauseNum = aFalseList[j];
      iClauseLen = aClauseLen[iCurrClauseNum];
      for(UINT32 k=0; k < iClauseLen; ++k) {
        // ERROR XXX asserts needed for aClauseLen and pClauseLits
        iCurrVar = pClauseLits[iCurrClauseNum][k-aClauseLen[iCurrClauseNum]] / 2;
        assert(iCurrVar >= 0);
        assert(iCurrVar <= iNumVars);
        aCurrAutarky[iCurrVar] = FALSE;
      }
    }
    
    for(UINT32 j=1; j <= iNumVars; ++j) {
      if(aCurrAutarky[j] == FALSE) continue;
      iCurrFlipVarLitValue = aVarValue[j] ? (j*2)+1 : j*2;
      aLitClauses = pLitClause[iCurrFlipVarLitValue];
      for(UINT32 k=0; k < aNumLitOcc[iCurrFlipVarLitValue]; ++k) {
        iCurrClauseNum = aLitClauses[k];
        iClauseLen = aClauseLen[iCurrClauseNum];
        for(UINT32 m=0; m < iClauseLen*2; ++m) {
          if(m >= iClauseLen){
            iCurrVar = pClauseLits[iCurrClauseNum][m - iClauseLen] / 2;
            assert(iCurrVar >= 0);
            assert(iCurrVar <= iNumVars);
            if(aCurrAutarky[iCurrVar]) {
              aStack[iStackSize] = iCurrVar;
              iStackSize++;
              aCurrAutarky[iCurrVar] = FALSE;
            }
          } else {
            iCurrLit = pClauseLits[iCurrClauseNum][m];
            iCurrVar = iCurrLit / 2;
            bOneLitTrue = aCurrAutarky[iCurrVar] and ((aVarValue[iCurrVar] and iCurrLit % 2 == 0)  || (not aVarValue[iCurrVar] and iCurrLit % 2 == 1 ));
            if(bOneLitTrue) break;
          }
        }
        if(! bOneLitTrue) { 
          while(bNotDone) {
            iCurrStackVar = aStack[iStackSize - 1];
            --iStackSize;
            iCurrFlipVarLitValue = aVarValue[iCurrStackVar] ? (j*2)+1 : j*2;
            aLitClauses = pLitClause[iCurrFlipVarLitValue];
            for(UINT32 m=0; m < aNumLitOcc[iCurrFlipVarLitValue]; ++m) {
              iCurrClauseNum = aLitClauses[m];
              iClauseLen = aClauseLen[iCurrClauseNum];
              for(UINT32 n=0; n < iClauseLen; ++n) {
                iCurrLit = pClauseLits[iCurrClauseNum][n];
                iCurrVar = iCurrLit / 2;
                if(aCurrAutarky[iCurrVar] == TRUE) {
                  aStack[iStackSize] = iCurrVar;
                  ++iStackSize;
                  aCurrAutarky[iCurrVar] = FALSE;
                }
              }
            }
            bNotDone = iStackSize > 0;
          }
        }
      }
    }
    
    iCurrAutarkySize = 0;
    for(UINT32 j = 1; j <= iNumVars; ++j) {
      if(aCurrAutarky[j]) ++iCurrAutarkySize;
    }
    std::cout << "iCurrAutarkySize = " << iCurrAutarkySize << "\n";
    
    DefaultFlip();
    CheckTermination();
    if(bSolutionFound)
      std::cout << "solution found\n";
    else std::cout << "solution not found yet\n";
    // Output function needs to be added
    UpdateFalseClauseList();
  }

}

int main(const int argc, char* const argv[]) {
  if (argc != 2) {
    std::cerr << "Exactly one argument needed (the filename of the input-cnf)!\n";
    return EXIT_FAILURE;
  }
  sFilenameIn = argv[1];
  Initialise();
  AnalyseTotalAssignment();
}
