/*!
  \file LocalSearch/Ubcsat/ubcsat-types.h
  \brief Corrected definition of fundamental types for UBCSAT.

  \bug It appears we never had the original file in the OKlibrary(-repository)?
  This must be corrected.
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

#ifndef UBCSATTYPES_hGGtr4g
#define UBCSATTYPES_hGGtr4g

#define ALTERNATEMAIN

#ifdef BOOL
# error "BOOL predefined, which might conflict with the C99 definition of bool"
#endif
#ifdef TRUE
# error "TRUE predefined, which might conflict with the C99 definition of true"
#endif
#ifdef FALSE
# error "FALSE predefined, which might conflict with the C99 definition of false"
#endif
#include <stdbool.h>
#define BOOL bool
#define TRUE true
#define FALSE false

#include <float.h>
#if DBL_MAX_10_EXP < 308
# error "Maximal decimal exponent for double is only " DBL_MAX_10_EXP ", but should be at least 308"
#endif
#define FLOAT double
#define FLOATMAX (1E+300)

#include <stdint.h>
#ifdef UINT32
# error "UINT32 predefined, which might conflict with the C99 definition of uint32_t"
#endif
#define UINT32 uint32_t
#ifdef SINT32
# error "SINT32 predefined, which might conflict with the C99 definition of int32_t"
#endif
#define SINT32 int32_t

#define PROBABILITY UINT32

#ifndef NULL
#define NULL 0
#endif

enum EVENTPOINT
{
  PostParameters,
  ReadInInstance,
  CreateData,
  CreateStateInfo,
  PreStart,
  PreRun,
  CheckRestart,
  InitData,
  InitStateInfo,
  PostInit,
  PreStep,
  ChooseCandidate,
  PreFlip,
  FlipCandidate,
  UpdateStateInfo,
  PostFlip,
  PostStep,
  StepCalculations,
  CheckTerminate,
  RunCalculations,
  PostRun,
  FinalCalculations,
  FinalReports,
  NUMEVENTPOINTS
};

enum PARMTYPE {
   PTypeUInt,
   PTypeSInt,
   PTypeBool,
   PTypeString,
   PTypeProbability,
   PTypeFloat,
   PTypeReport
};

enum CDATATYPE {
  DTypeUInt,
  DTypeSInt,
  DTypeFloat
};

enum COLTYPE {
  ColTypeFinal,
  ColTypeMean,
  ColTypeStddev,
  ColTypeCV,
  ColTypeMin,
  ColTypeMax,
  ColTypeFinalDivStep,
  ColTypeFinalDivStep100
};

typedef void (*FXNPTR)();
typedef void (*CALLBACKPTR)(UINT32, const char *sItem); 
typedef UINT32 (*FXNRAND32)();

typedef struct typeITEM {
  char *sID;
  BOOL bContainer;
  char *sContainerList;
} ITEM;

typedef struct typeITEMLIST {
  UINT32 iNumItems;
  ITEM aItems[MAXITEMLIST];
} ITEMLIST;

typedef struct typeTRIGGER {
  
  enum EVENTPOINT eEventPoint;
  FXNPTR pProcedure;
  
  BOOL bActive;
  BOOL bDisabled;

  char *sDependencyList;
  char *sDeactivateList;

} TRIGGER;

typedef union typePARAMETERDEFAULT {
  UINT32 iUInt;
  SINT32 iSInt;
  BOOL bBool;
  char *sString;
  PROBABILITY iProb;
  FLOAT fFloat;
} PARAMETERDEFAULT;

typedef struct typeALGPARM {
  enum PARMTYPE eType;
  char *sSwitch;
  char *sName;
  char *sDescription;
  void *pParmValue;
  BOOL bSpecified;
  char *sTriggers;
  PARAMETERDEFAULT defDefault;
} ALGPARM;

typedef struct ALGPARAMETERS {
  UINT32 iNumParms;
  ALGPARM aParms[MAXALGPARMS];
} ALGPARMLIST;

typedef struct typeALGORITHM {
  
  char *sName;
  char *sVariant;
  BOOL bWeighted;

  char *sDescription;
  char *sAuthors;

  char *sHeuristicTriggers;
  char *sDataTriggers;

  char *sDefaultOutput;
  char *sDefaultStats;

  ALGPARMLIST parmList;

} ALGORITHM;

typedef struct typeREPORT {
  FILE *fileOut;
  char *sID;
  char *sDescription;
  BOOL bActive;
  char *sOutputFile;
  char *sTriggers;
  UINT32 iNumParms;
  char *aParmName[MAXREPORTPARMS];
  enum PARMTYPE aParmTypes[MAXREPORTPARMS];
  void *aParameters[MAXREPORTPARMS];
} REPORT;

typedef struct typeREPORTCOL {
  BOOL bActive;

  char *sDescription;
  char *sHeader1;
  char *sHeader2;
  char *sHeader3;
  char *sPrintFormat;

  UINT32 *puiCurValue;
  SINT32 *psiCurValue;
  FLOAT *pfCurValue;

  UINT32 *puiColumnData;
  SINT32 *psiColumnData;
  FLOAT *pfColumnData;

  char *sTriggers;

  enum CDATATYPE eDataType;
  enum COLTYPE eColType;

  SINT32 siMinMaxVal;
  UINT32 uiMinMaxVal;
  FLOAT  fMinMaxVal;

  FLOAT fSum;
  FLOAT fSum2;

} REPORTCOL;


typedef struct typeREPORTSTAT {
  
  unsigned char bActive;
  UINT32 iActiveID;
  
  char *sDescription;

  BOOL bCustomField;
  void *pCustomValue;
  enum CDATATYPE eCustomType;
  char *sPrintCustomFormat;

  char *sBaseDescription;

  char *sStatParms;

  UINT32 iStatFlags;
  
  char *sRequiredCols;
  char *sDataColumn;

  char *sTriggers;

  BOOL bSortByStep;

} REPORTSTAT;

#endif
