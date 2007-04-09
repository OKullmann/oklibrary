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

#include "ubcsat.h"

/*  
    This file contains the code to make the various reports work
*/

/***** Trigger ReportOut *****/

BOOL bReportOutputSupress = FALSE;

void ReportOutSetup();
void ReportOutSplash();
void ReportOutHeader();
void ReportOutRow();
void ReportOutSupressed();

/***** Trigger ReportStats *****/
void ReportStatsSetup();
void ReportStatsPrint();

/***** Trigger ReportRTD *****/
void ReportRTDSetup();
void ReportRTDPrint();

/***** Trigger ReportModelPrint *****/
void ReportModelPrint();

/***** Trigger ReportCNFStatsPrint *****/
void ReportCNFStatsPrint();

/***** Trigger ReportStatePrint *****/
void ReportStatePrint();

/***** Trigger ReportSolutionPrint *****/
void ReportSolutionPrint();

/***** Trigger ReportBestPrint *****/
void ReportBestPrint();

/***** Trigger ReportUnsatClausesPrint *****/
void ReportUnsatClausesPrint();

/***** Trigger ReportFlipCountsPrint *****/
void ReportFlipCountsPrint();

/***** Trigger ReportUnsatCountsPrint *****/
void ReportUnsatCountsPrint();

/***** Trigger ReportVarLastPrint *****/
void ReportVarLastPrint();

/***** Trigger ReportClauseLastPrint *****/
void ReportClauseLastPrint();

/***** Trigger ReportSQGridPrint *****/
void ReportSQGridPrint();

/***** Trigger ReportSatCompetitionPrint *****/
void ReportSatCompetitionPrint();

/***** Trigger CalcPercentSolve *****/
void CalcPercentSolve();
FLOAT fPercentSuccess;

/***** Trigger ColumnRunCalculation *****/
void ColumnRunCalculation();

/***** Trigger ColumnStepCalculation *****/
void ColumnStepCalculation();

/***** Trigger ColumnInit *****/
void ColumnInit();

/***** Trigger InitSolveModev *****/
void InitSolveMode();

/***** Trigger UpdatePercents *****/
void UpdatePercents();

/***** Trigger UpdateTimes *****/
void UpdateTimes();

/***** Trigger SortByStepPerformance *****/
UINT32 *aSortedBySteps;
UINT32 *aSortedByCurrent;
void SortByCurrentColData(REPORTCOL *pCol);
void SortByStepPerformance();

/***** CalcFPS *****/
void CalcFPS();

void AddReportTriggers() {

  CreateTrigger("ReportOutSetup",PostParameters,ReportOutSetup,"","");
  CreateTrigger("ReportOutSplash",PostParameters,ReportOutSplash,"","");
  CreateTrigger("ReportOutHeader",PreStart,ReportOutHeader,"","");
  CreateTrigger("ReportOutRow",PostRun,ReportOutRow,"ColumnRunCalculation","");
  CreateTrigger("ReportOutSupressed",FinalReports,ReportOutSupressed,"","");
  CreateContainerTrigger("ReportOut","ReportOutSetup,ReportOutSplash,ReportOutHeader,ReportOutRow,ReportOutSupressed");

  CreateTrigger("ReportStatsSetup",PostParameters,ReportStatsSetup,"","");
  CreateTrigger("ReportStatsPrint",FinalReports,ReportStatsPrint,"","");
  CreateContainerTrigger("ReportStats","ReportStatsSetup,SortByStepPerformance,ReportStatsPrint");

  CreateTrigger("ReportRTDSetup",PostParameters,ReportRTDSetup,"","");
  CreateTrigger("ReportRTDPrint",FinalReports,ReportRTDPrint,"","");
  CreateContainerTrigger("ReportRTD","ReportRTDSetup,ReportRTDPrint");

  CreateTrigger("ReportModelPrint",FinalReports,ReportModelPrint,"","");

  CreateTrigger("ReportCNFStatsPrint",FinalReports,ReportCNFStatsPrint,"","");

  CreateTrigger("ReportStatePrint",PostStep,ReportStatePrint,"","");
  
  CreateTrigger("ReportSolutionPrint",PostRun,ReportSolutionPrint,"","");

  CreateTrigger("ReportBestPrint",PostRun,ReportBestPrint,"SaveBest","");

  CreateTrigger("ReportUnsatClausesPrint",PostRun,ReportUnsatClausesPrint,"","");

  CreateTrigger("ReportFlipCountsPrint",PostRun,ReportFlipCountsPrint,"FlipCounts","");

  CreateTrigger("ReportUnsatCountsPrint",PostRun,ReportUnsatCountsPrint,"UnsatCounts","");

  CreateTrigger("ReportVarLastPrint",PostRun,ReportVarLastPrint,"VarLastChange","");

  CreateTrigger("ReportClauseLastPrint",PostRun,ReportClauseLastPrint,"ClauseLast","");

  CreateTrigger("ReportSQGridPrint",PostRun,ReportSQGridPrint,"SQGrid","");

  CreateTrigger("ReportSatCompetitionPrint",FinalReports,ReportSatCompetitionPrint,"","");

  CreateTrigger("CalcPercentSolve",FinalCalculations,CalcPercentSolve,"","");
  CreateTrigger("CalcFPS",FinalCalculations,CalcFPS,"","");
  CreateTrigger("ColumnRunCalculation",PostRun,ColumnRunCalculation,"","");
  CreateTrigger("ColumnStepCalculation",StepCalculations,ColumnStepCalculation,"ColumnInit","");
  CreateTrigger("ColumnInit",PreRun,ColumnInit,"","");
  CreateTrigger("SolveMode",PreStart,InitSolveMode,"ReportModelPrint","");
  CreateTrigger("UpdatePercents",FinalCalculations,UpdatePercents,"SortByStepPerformance","");
  CreateTrigger("UpdateTimes",FinalCalculations,UpdateTimes,"SortByStepPerformance","");  
  CreateTrigger("SortByStepPerformance",FinalCalculations,SortByStepPerformance,"","");

}

/***************************************************************************/

/***** Report -r out *****/

void ReportOutSetup() {
  iNumOutputColumns = 0;
  ParseItemList(&listColumns,(char *)pRepOut->aParameters[0],ActivateColumn);
  ParseItemList(&listColumns,(char *)pRepOut->aParameters[0],AddOutputColumn);
}

void ReportOutSplash() {
  PrintUBCSATHeader(pRepOut);
  if (strcmp(sFilenameIn,"")==0) {
    ReportHdrPrefix(pRepOut);
    ReportHdrPrint(pRepOut,"no -inst file specified: reading from stdin -- e.g.: ubcsat <myfile.cnf\n");
    ReportHdrPrefix(pRepOut);
    ReportHdrPrint(pRepOut,"\n");
  }
}

void PrintColHeaders(REPORT *pRep,UINT32 iNumCols, UINT32 *aCols) {

  UINT32 j;
  if (!bReportClean) {

    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"Output Columns: ");
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint1(pRep,"|%s",listColumns.aItems[aCols[j]].sID);
    }
    ReportHdrPrint(pRep,"|\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrefix(pRep);
      ReportHdrPrint2(pRep,"%s: %s\n",listColumns.aItems[aCols[j]].sID,aColumns[aCols[j]].sDescription);
    }
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader1);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader2);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {      
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader3);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
  }

}

void ReportOutHeader() {
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint1(pRepOut," -alg %s",pActiveAlgorithm->sName);
  if (*(pActiveAlgorithm->sVariant))
    ReportHdrPrint1(pRepOut," -v %s",pActiveAlgorithm->sVariant);
  if (pActiveAlgorithm->bWeighted)
    ReportHdrPrint(pRepOut," -w");
  
  ReportHdrPrint(pRepOut,"\n");

  PrintAlgParmSettings(pRepOut,&parmUBCSAT);
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  PrintAlgParmSettings(pRepOut,&pActiveAlgorithm->parmList);
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"UBCSAT default output: \n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"   'ubcsat -r out null' to supress, 'ubcsat -hc' to customise\n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  PrintColHeaders(pRepOut,iNumOutputColumns,aOutputColumns);
}

void PrintRow(REPORT *pRep, UINT32 iRow, UINT32 iNumCols, UINT32 *aCols) {

  UINT32 j;
  REPORTCOL *pCol;

  ReportPrint(pRep,"  ");

  for (j=0;j<iNumCols;j++) {
    pCol = &aColumns[aCols[j]];

    if ((pCol->eDataType == DTypeUInt) && ((pCol->eColType == ColTypeFinal)||(pCol->eColType == ColTypeMin)||(pCol->eColType == ColTypeMax))) {
      ReportPrint1(pRep,pCol->sPrintFormat,pCol->puiColumnData[iRow]);
    } else if ((pCol->eDataType == DTypeSInt) && ((pCol->eColType == ColTypeFinal)||(pCol->eColType == ColTypeMin)||(pCol->eColType == ColTypeMax))) {
      ReportPrint1(pRep,pCol->sPrintFormat,pCol->psiColumnData[iRow]);
    } else {
      ReportPrint1(pRep,pCol->sPrintFormat,pCol->pfColumnData[iRow]);
    }
    ReportPrint(pRep," ");
  }
  ReportPrint(pRep,"\n");

}

void ReportOutRow() {
  if (!bReportOutputSupress) {
    PrintRow(pRepOut,iRun-1,iNumOutputColumns,aOutputColumns);
  }
}

void ReportOutSupressed() {
  UINT32 j;
  if (bReportOutputSupress) {
    for (j=0;j<iRun;j++) {
      PrintRow(pRepOut,j,iNumOutputColumns,aOutputColumns);
    }
  }
}



/***** Report -r stats *****/

void ReportStatsSetup() {
  ParseItemList(&listStats,(char *)pRepStats->aParameters[0],ActivateStat);
}

FLOAT GetRowElement(REPORTCOL *pCol,UINT32 iRowRequested, BOOL bSorted, BOOL bSortByStep) {

  UINT32 iRow;

  if (bSorted) {
    if (bSortByStep) {
      iRow = aSortedBySteps[iRowRequested];
    } else {
      iRow = aSortedByCurrent[iRowRequested];
    }
  } else {
    iRow = iRowRequested;
  }

  if ((pCol->eDataType == DTypeUInt) && ((pCol->eColType == ColTypeFinal)||(pCol->eColType == ColTypeMin)||(pCol->eColType == ColTypeMax))) {
    return ((FLOAT)pCol->puiColumnData[iRow]);
  } else if ((pCol->eDataType == DTypeSInt) && ((pCol->eColType == ColTypeFinal)||(pCol->eColType == ColTypeMin)||(pCol->eColType == ColTypeMax))) {
    return ((FLOAT)pCol->psiColumnData[iRow]);
  } else {
    return (pCol->pfColumnData[iRow]);
  }
}

void ReportStatsPrint() {
  
  UINT32 j,k;
  UINT32 iRow;
  REPORTCOL *pCol;
  REPORTSTAT *pStat;
  
  FLOAT fMean;
  FLOAT fMedian;
  FLOAT fStdDev;
  FLOAT fTemp;

  ReportPrint(pRepStats,"\n\n");

  for (k=0;k<iNumStatsActive;k++) {
    for (j=0;j<listStats.iNumItems;j++) {
      pStat = &aStats[j];
      if ((pStat->bActive)&&(pStat->iActiveID==k)) {
        
        if (pStat->bCustomField) {
          switch (pStat->eCustomType) {
            case DTypeUInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(UINT32 *) pStat->pCustomValue);
              break;
            case DTypeSInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(SINT32 *) pStat->pCustomValue);
              break;
            case DTypeFloat:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(FLOAT *) pStat->pCustomValue);
              break;
          }
        } else {

          pCol = &aColumns[FindItem(&listColumns,pStat->sDataColumn)];

          if (!pStat->bSortByStep) {
            SortByCurrentColData(pCol);
          }

          if (iRun) {

            fMean = 0.0; 
            for (iRow = 0; iRow < iRun; iRow++) {
              fMean += GetRowElement(pCol,iRow,FALSE,pStat->bSortByStep);
            }
            fMean /= (FLOAT) iRun;

            fStdDev = 0.0; 
            if (iRun > 1) {
              for (iRow = 0; iRow < iRun; iRow++) {
                fTemp = GetRowElement(pCol,iRow,FALSE,pStat->bSortByStep)-fMean;
                fStdDev += (fTemp*fTemp);
              }
              fStdDev /= (FLOAT) (iRun-1);
              fStdDev = sqrt(fStdDev);
            }

            fMedian = GetRowElement(pCol,(iRun-1)>>1,TRUE,pStat->bSortByStep);
            if (iRun % 2 == 0) {
              fMedian += GetRowElement(pCol,(iRun)>>1,TRUE,pStat->bSortByStep);
              fMedian /= 2.0;
            }

            PrintFullStat(pStat,"mean","Mean",fMean);
            PrintFullStat(pStat,"stddev","StdDev",fStdDev);
            PrintFullStat(pStat,"cv","CoeffVariance",fStdDev / fMean);
            PrintFullStat(pStat,"median","Median",fMedian);
            PrintFullStat(pStat,"min","Min",GetRowElement(pCol,0,TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"max","Max",GetRowElement(pCol,iRun-1,TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q05","Q.05",GetRowElement(pCol,(UINT32)(floor(0.05 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q10","Q.10",GetRowElement(pCol,(UINT32)(floor(0.10 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q25","Q.25",GetRowElement(pCol,(UINT32)(floor(0.25 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q75","Q.75",GetRowElement(pCol,(UINT32)(floor(0.75 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q90","Q.90",GetRowElement(pCol,(UINT32)(floor(0.90 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q95","Q.95",GetRowElement(pCol,(UINT32)(floor(0.95 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"q98","Q.98",GetRowElement(pCol,(UINT32)(floor(0.98 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"qr75/25","Q.75/25",GetRowElement(pCol,(UINT32)(floor(0.75 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep)/GetRowElement(pCol,(UINT32)(floor(0.25 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"qr90/10","Q.90/10",GetRowElement(pCol,(UINT32)(floor(0.90 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep)/GetRowElement(pCol,(UINT32)(floor(0.10 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            PrintFullStat(pStat,"qr95/05","Q.95/05",GetRowElement(pCol,(UINT32)(floor(0.95 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep)/GetRowElement(pCol,(UINT32)(floor(0.05 * (FLOAT) (iRun-1))),TRUE,pStat->bSortByStep));
            
          }
        }
      }
    }
  }
  ReportPrint(pRepStats,"\n\n");
}



/***** Report -r rtd *****/

void ReportRTDSetup() {
  iNumRTDColumns = 0;
  ParseItemList(&listColumns,(char *)pRepRTD->aParameters[0],ActivateColumn);
  ParseItemList(&listColumns,(char *)pRepRTD->aParameters[0],AddRTDColumn);
}

void PrintRTDRow(UINT32 iRow) {
  PrintRow(pRepRTD,aSortedBySteps[iRow],iNumRTDColumns,aRTDColumns);
}

void ReportRTDPrint() {
  UINT32 j;
  PrintColHeaders(pRepRTD,iNumRTDColumns,aRTDColumns);
  for (j=0;j<iNumSolutionsFound;j++) {
    PrintRTDRow(j);
  }
}



/***** Report -r model *****/

void InitSolveMode() {
  if (bSolveMode) {
    if (iFind==0)
      iFind = 1;
  }
}

void ReportModelPrint() {
  UINT32 j;
  if ((bSolveMode)||(pRepModel->bActive)) {
    if (bSolutionFound) {
      ReportHdrPrefix(pRepModel);
      ReportHdrPrint(pRepModel,"\n");
      ReportHdrPrefix(pRepModel);
      if (bWeighted) {
        ReportHdrPrint1(pRepModel,"Solution found for -wtarget %f\n\n", fTargetW);
      } else {
        ReportHdrPrint1(pRepModel,"Solution found for -target %u\n\n", iTarget);
      }
      for (j=1;j<=iNumVars;j++) {
        if (!aVarValue[j]) {
          ReportPrint1(pRepModel," -%u",j);
        } else {
          ReportPrint1(pRepModel," %u",j);
        }
	      if (j % 10 == 0)
          ReportPrint(pRepModel,"\n");
      }
      if ((j-1) % 10 != 0) 
        ReportPrint(pRepModel,"\n");
    } else {
      ReportHdrPrefix(pRepModel);
      if (bWeighted) {
        ReportHdrPrint1(pRepModel,"No Solution found for -wtarget %f\n\n", fTargetW);
      } else {
        ReportHdrPrint1(pRepModel,"No Solution found for -target %u\n", iTarget);
      }

      
    }
  }
}



/***** Report -r cnfstats *****/

void ReportCNFStatsPrint() {

  UINT32 j;
  UINT32 iMaxClauseLen;
  FLOAT fAvgLitClause = 0.0;
  FLOAT fAvgVarOccur = 0.0;
  FLOAT fStdDevVarOccur = 0.0;
  FLOAT fTemp;
  UINT32 iNumPos;
  UINT32 iNumNeg;
  FLOAT fPosNegRatio;

  UINT32 *aClauseBins;
  

  ReportPrint1(pRepCNFStats,"Clauses = %u\n",iNumClauses);
  ReportPrint1(pRepCNFStats,"Variables = %u \n",iNumVars);
  ReportPrint1(pRepCNFStats,"TotalLiterals = %u\n",iNumLits);

  iMaxClauseLen = 0;
  for (j=0;j<iNumClauses;j++) {
    if (aClauseLen[j] > iMaxClauseLen)
      iMaxClauseLen = aClauseLen[j];
  }
  ReportPrint1(pRepCNFStats,"MaxClauseLen = %u\n",iMaxClauseLen);

  if (iNumClauses > 0) {

    aClauseBins = AllocateRAM((iMaxClauseLen + 3) * sizeof(UINT32));
    
    for (j=0;j<iMaxClauseLen + 3;j++) {
      aClauseBins[j] = 0;
    }

    for (j=0;j<iNumClauses;j++) {
      aClauseBins[aClauseLen[j]]++;
    }

    ReportPrint1(pRepCNFStats,"NumClauseLen1 = %u \n",aClauseBins[1]);
    ReportPrint1(pRepCNFStats,"NumClauseLen2 =  %u \n",aClauseBins[2]);
    ReportPrint1(pRepCNFStats,"NumClauseLen3+ = %u \n",iNumClauses - aClauseBins[1] - aClauseBins[2]);

    ReportPrint(pRepCNFStats,"FullClauseDistribution = ");
    for (j=0;j<=iMaxClauseLen;j++) {
      if (aClauseBins[j] > 0) {
        ReportPrint1(pRepCNFStats," %u",j);
        ReportPrint1(pRepCNFStats,":%u",aClauseBins[j]);
      }
    }
    ReportPrint(pRepCNFStats,"\n");

    fAvgLitClause = (FLOAT)iNumLits / (FLOAT) iNumClauses;
    ReportPrint1(pRepCNFStats,"MeanClauseLen = %f \n",fAvgLitClause);
    
    fAvgVarOccur = (FLOAT) iNumLits / (FLOAT) iNumVars;
    ReportPrint1(pRepCNFStats,"MeanVariableOcc = %f \n",fAvgVarOccur);

    fStdDevVarOccur = 0.0; 
    for (j=1;j<=iNumVars;j++) {
      fTemp = (aNumLitOcc[GetPosLit(j)] + aNumLitOcc[GetNegLit(j)])-fAvgVarOccur;
      fStdDevVarOccur += (fTemp*fTemp);
    }
    fStdDevVarOccur /= (FLOAT) (iNumVars-1);
    fStdDevVarOccur = sqrt(fStdDevVarOccur);
    
    ReportPrint1(pRepCNFStats,"StdDevVariableOcc = %f \n",fStdDevVarOccur);

    iNumPos = 0;
    iNumNeg = 0;
    for (j=1;j<=iNumVars;j++) {
      iNumPos += aNumLitOcc[GetPosLit(j)];
      iNumNeg += aNumLitOcc[GetNegLit(j)];
    }
    fPosNegRatio = (FLOAT) iNumPos / (FLOAT) iNumNeg;
    ReportPrint1(pRepCNFStats,"NumPosLit = %u \n",iNumPos);
    ReportPrint1(pRepCNFStats,"NumNegLit = %u \n",iNumNeg);
    ReportPrint1(pRepCNFStats,"RatioPos:NegLit = %f \n",fPosNegRatio);
  }
}



/***** Report -r state *****/

void ReportStatePrint() {
  UINT32 j;
  UINT32 bLocalMin = 0;
  UINT32 bPrint;
  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepState);
    ReportHdrPrint(pRepState," Run ID | Step No. | Num False (or sum of false weights) | VarFlip | IsLocalMin | vararray\n");
    if (fReportStateQuality < 0) {
      bReportStateQuality = 0;
    } else {
      bReportStateQuality = 1;
      if (bWeighted==0) {
        iReportStateQuality = (UINT32) fReportStateQuality;
      }
    }
  }

  bPrint = 1;
  if (bReportStateQuality) {
   if (bWeighted) {
     if (fSumFalseW != fReportStateQuality) {
       bPrint = 0;
     }
   } else {
     if (iNumFalse != iReportStateQuality) {
       bPrint = 0;
     }
   }
  }

  if ((bPrint) && (bReportStateLMOnly)) {
    bLocalMin = IsLocalMinimum();
    if (!bLocalMin) {
      bPrint = 0;
    }
  }

  if (bPrint) {
    ReportPrint1(pRepState,"%u ",iRun);
    ReportPrint1(pRepState,"%u ",iStep);

    if (bWeighted) {
      ReportPrint1(pRepState,"%f ",fSumFalseW);
    } else {
      ReportPrint1(pRepState,"%u ",iNumFalse);
    }

    ReportPrint1(pRepState,"%u ",iFlipCandidate);
    if (bReportStateLMOnly) {
      ReportPrint1(pRepState,"%u ",bLocalMin);
    } else {
      ReportPrint1(pRepState,"%u ",IsLocalMinimum());
    }
    for (j=1;j<=iNumVars;j++) {
      if (aVarValue[j]) {
        ReportPrint(pRepState,"1");
      } else {
        ReportPrint(pRepState,"0");
      }
    }
    ReportPrint(pRepState,"\n");
  }
}



/***** Report -best  *****/

void ReportBestPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepBest);
    ReportHdrPrint(pRepBest," Run ID | Solution Found? | Best # false (or weighted best) | vararray\n");
  }
  ReportPrint2(pRepBest,"%u %u ",iRun, bSolutionFound);
  if (bWeighted) {
    ReportPrint1(pRepBest,"%f ",fBestSumFalseW);
  } else {
    ReportPrint1(pRepBest,"%u ",iBestNumFalse);
  }
  for (j=1;j<=iNumVars;j++) {
    if (aVarStateBest[j]) {
      ReportPrint(pRepBest,"1");
    } else {
      ReportPrint(pRepBest,"0");
    }
  }
  ReportPrint(pRepBest,"\n");
}



/***** Report -solution  *****/

void ReportSolutionPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepSolution);
    ReportHdrPrint(pRepSolution," Run ID | vararray\n");
  }
  if (bSolutionFound) {
    ReportPrint1(pRepSolution,"%u ",iRun);
    for (j=1;j<=iNumVars;j++) {
      if (aVarValue[j]) {
        ReportPrint(pRepSolution,"1");
      } else {
        ReportPrint(pRepSolution,"0");
      }
    }
    ReportPrint(pRepSolution,"\n");
  }
}



/***** Report -r unsatclauses *****/

void ReportUnsatClausesPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepOptClauses);
    ReportHdrPrint(pRepOptClauses," Run ID | clauses at target solution (1=satisfied) ...\n");
  }
  if (bSolutionFound) {
    ReportPrint1(pRepOptClauses,"%u ",iRun);
    for (j=0;j<iNumClauses;j++) {
      if (aNumTrueLit[j]==0) {
        ReportPrint(pRepOptClauses,"0");
      } else {
        ReportPrint(pRepOptClauses,"1");
      }
    }
    ReportPrint(pRepOptClauses,"\n");
  }
}



/***** Report -r flipcount *****/

void ReportFlipCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepFlipCounts);
    ReportHdrPrint(pRepFlipCounts," Run ID | FlipCount[0] (NullFlips) | FlipCount[1] | FlipCount[2]...\n");
  }
  ReportPrint1(pRepFlipCounts,"%u",iRun);
  for (j=0;j<=iNumVars;j++) {
    ReportPrint1(pRepFlipCounts," %u",aFlipCounts[j]);
  }
  ReportPrint(pRepFlipCounts,"\n");
}



/***** Report -r unsatcount *****/

void ReportUnsatCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepUnsatCounts);
    ReportHdrPrint(pRepUnsatCounts," Run ID | UnsatCount[1] | UnsatCount[2] ...\n");
  }
  ReportPrint1(pRepUnsatCounts,"%u",iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepUnsatCounts," %u",aUnsatCounts[j]);
  }
  ReportPrint(pRepUnsatCounts,"\n");
}



/***** Report -r varlastflip *****/

void ReportVarLastPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepVarLast);
    ReportHdrPrint(pRepVarLast," Run ID | StepLastFlip[1] | StepLastFlip[2] ...\n");
  }

  ReportPrint1(pRepVarLast,"%u",iRun);
  for (j=1;j<=iNumVars;j++) {
    ReportPrint1(pRepVarLast," %u",aVarLastChange[j]);
  }
  ReportPrint(pRepVarLast,"\n");
}



/***** Report -r clauselast *****/

void ReportClauseLastPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepClauseLast);
    ReportHdrPrint(pRepClauseLast," Run ID | StepLastUnsat[1] | StepLastUnsat[2] ...\n");
  }
  ReportPrint1(pRepClauseLast,"%u",iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepClauseLast," %u",aClauseLast[j]);
  }
  ReportPrint(pRepClauseLast,"\n");
}



/***** Report -r sqgrid *****/

void ReportSQGridPrint() {

  UINT32 j;

  if (pRepSQGrid->bActive) {
    if (iRun==1) {
      ReportHdrPrefix(pRepSQGrid);
      ReportHdrPrint(pRepSQGrid," Run ID | Solution Quality at steps:");
      for (j=0;j<iNumLogDistValues;j++) {
        ReportHdrPrint1(pRepSQGrid," %u",aLogDistValues[j]);
      }
      ReportHdrPrint(pRepSQGrid,"\n");
    }

    ReportPrint1(pRepSQGrid,"%u",iRun);

    if (bWeighted) {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid," %f",aSQGridW[iNumLogDistValues * (iRun-1) + j]);
      }
    } else {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid," %u",aSQGrid[iNumLogDistValues * (iRun-1) + j]);
      }
    }
    ReportPrint(pRepSQGrid,"\n");
  }
}



/***** Report -r satcomp *****/

void ReportSatCompetitionPrint() {
  UINT32 j;

  SetString(&sCommentString,"c");

  PrintUBCSATHeader(pRepSATComp);
  PrintAlgParmSettings(pRepSATComp,&parmUBCSAT);
  ReportHdrPrefix(pRepSATComp);
  ReportHdrPrint(pRepSATComp,"\n");
  PrintAlgParmSettings(pRepSATComp,&pActiveAlgorithm->parmList);
  ReportHdrPrefix(pRepSATComp);
  ReportHdrPrint(pRepSATComp,"\n");

  if (bSolutionFound) {
    ReportPrint(pRepSATComp,"s SATISFIABLE\n");
    ReportPrint(pRepSATComp,"v ");
    for (j=1;j<=iNumVars;j++) {
      if (!aVarValue[j]) {
        ReportPrint1(pRepSATComp," -%u",j);
      } else {
        ReportPrint1(pRepSATComp," %u",j);
      }
	    if (j % 10 == 0)
        ReportPrint(pRepSATComp,"\nv ");
    }
    ReportPrint(pRepSATComp," 0\n");
    exit(10);
  } else {
    ReportPrint(pRepSATComp,"s UNKNOWN\n");
    exit(0);
  }
}



/***** stat "percentsolve" *****/

void CalcPercentSolve() {
  fPercentSuccess = 100.0 * (FLOAT) iNumSolutionsFound / (FLOAT) iRun;
}



/***** Trigger ColumnInit *****/

void ColumnInit() {
  UINT32 j;
  REPORTCOL *pCol;
  
  for (j=0;j<iNumActiveCalcColumns;j++) {
    pCol = &aColumns[aActiveCalcColumns[j]];
    pCol->fSum = 0.0f;
    pCol->fSum2 = 0.0f;
    if (pCol->eColType == ColTypeMax) {
      pCol->fMinMaxVal = 0.0f;
      pCol->uiMinMaxVal = 0;
      pCol->siMinMaxVal = 0x80000000;
    } else {
      pCol->fMinMaxVal = FLOATMAX;
      pCol->uiMinMaxVal = 0xFFFFFFFF;
      pCol->siMinMaxVal = 0x7FFFFFFF;
    }
  }
}



/***** Trigger ColumnRunCalculation *****/

void ColumnRunCalculation() {

  UINT32 j;
  FLOAT fStepMul;
  FLOAT fMean;
  FLOAT fStddev;

  if (iStep == 0) {
    fStepMul = 1.0f;
  } else {
    fStepMul = 1.0f / ((FLOAT) iStep);
  }

  for (j=0;j<listColumns.iNumItems;j++) {
    if (aColumns[j].bActive) {
      switch(aColumns[j].eColType)
      {
        case ColTypeFinal:
          switch(aColumns[j].eDataType)
          {
          case DTypeUInt:
            aColumns[j].puiColumnData[iRun-1] = *aColumns[j].puiCurValue;
            break;
          case DTypeSInt:
            aColumns[j].psiColumnData[iRun-1] = *aColumns[j].psiCurValue;
            break;
          case DTypeFloat:
            aColumns[j].pfColumnData[iRun-1] = *aColumns[j].pfCurValue;
            break;
          }
          break;
        case ColTypeMin:
        case ColTypeMax:
          switch(aColumns[j].eDataType)
          {
          case DTypeUInt:
            aColumns[j].puiColumnData[iRun-1] = aColumns[j].uiMinMaxVal;
            break;
          case DTypeSInt:
            aColumns[j].psiColumnData[iRun-1] = aColumns[j].siMinMaxVal;
            break;
          case DTypeFloat:
            aColumns[j].pfColumnData[iRun-1] = aColumns[j].fMinMaxVal;
            break;
          }
          break;
        case ColTypeFinalDivStep:
        case ColTypeFinalDivStep100:
          switch(aColumns[j].eDataType)
          {
          case DTypeUInt:
            aColumns[j].pfColumnData[iRun-1] = (FLOAT) *aColumns[j].puiCurValue;
            break;
          case DTypeSInt:
            aColumns[j].pfColumnData[iRun-1] = (FLOAT) *aColumns[j].psiCurValue;
            break;
          case DTypeFloat:
            aColumns[j].pfColumnData[iRun-1] = *aColumns[j].psiCurValue;
            break;
          }
          aColumns[j].pfColumnData[iRun-1] *= fStepMul;
          if (aColumns[j].eColType == ColTypeFinalDivStep100) {
            aColumns[j].pfColumnData[iRun-1] *= 100.0f;
          }
          break;

        case ColTypeMean:
          aColumns[j].pfColumnData[iRun-1] = aColumns[j].fSum * fStepMul;
          break;
        case ColTypeStddev:
        case ColTypeCV:
          fMean = aColumns[j].fSum * fStepMul;
          fStddev = aColumns[j].fSum2 * fStepMul - fMean * fMean;
          fStddev = sqrt(fStddev);

          if (aColumns[j].eColType == ColTypeStddev) {
            aColumns[j].pfColumnData[iRun-1] = fStddev;
          } else {
            aColumns[j].pfColumnData[iRun-1] = fStddev / fMean;
          }
          break;
      }
    }
  }
}



/***** Trigger ColumnStepCalculation *****/

void ColumnStepCalculation() {

  UINT32 j;
  REPORTCOL *pCol;
  
  for (j=0;j<iNumActiveCalcColumns;j++) {
    pCol = &aColumns[aActiveCalcColumns[j]];
    switch(pCol->eColType)
    {
      case ColTypeStddev:
      case ColTypeCV:
        switch(pCol->eDataType)
        {
        case DTypeUInt:
          pCol->fSum2 += ((FLOAT) *pCol->puiCurValue) * ((FLOAT) *pCol->puiCurValue);
          break;
        case DTypeSInt:
          pCol->fSum2 += ((FLOAT) *pCol->psiCurValue) * ((FLOAT) *pCol->psiCurValue);
          break;
        case DTypeFloat:
          pCol->fSum2 += ((*pCol->pfCurValue) * (*pCol->pfCurValue));
          break;
        }
      case ColTypeMean:
        switch(pCol->eDataType)
        {
        case DTypeUInt:
          pCol->fSum += (FLOAT) *pCol->puiCurValue;
          break;
        case DTypeSInt:
          pCol->fSum += (FLOAT) *pCol->psiCurValue;
          break;
        case DTypeFloat:
          pCol->fSum += *pCol->pfCurValue;
          break;
        }
        break;

      case ColTypeMin:
        switch(pCol->eDataType)
        {
          case DTypeUInt:
            if (*pCol->puiCurValue < pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue < pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue < pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
        }
        break;

      case ColTypeMax:
        switch(pCol->eDataType)
        {
          case DTypeUInt:
            if (*pCol->puiCurValue > pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue > pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue > pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
        }
        break;
    }
  }
}



/***** Trigger CalcFPS *****/

void CalcFPS() {
  UINT32 j;
  REPORTCOL *pCol;

  pCol = &aColumns[FindItem(&listColumns,"steps")];
  fFlipsPerSecond = 0.0;
  for (j=0;j< iRun;j++) {
    fFlipsPerSecond += GetRowElement(pCol,j,FALSE,FALSE);
  }
  fFlipsPerSecond /= fTotalTime;
}



/***** Trigger UpdatePercents *****/

void UpdatePercents() {
  
  UINT32 j;
  REPORTCOL *pCol;
  pCol = &aColumns[FindItem(&listColumns,"prob")];
  for (j=0;j<iRun;j++) {
    pCol->pfColumnData[aSortedBySteps[j]] = ((FLOAT) (j+1))/((FLOAT)iRun);
  }

}



/***** Trigger UpdateTimes *****/

void UpdateTimes() {
  
  UINT32 j;
  REPORTCOL *pColTimes;
  REPORTCOL *pColSteps;
  FLOAT *aTimes;
  UINT32 *aSteps;

  FLOAT fTotalSteps = 0.0;

  pColSteps = &aColumns[FindItem(&listColumns,"steps")];

  aSteps = pColSteps->puiColumnData;
  for (j=0;j<iRun;j++) {
    fTotalSteps += (FLOAT) aSteps[j];
  }

  pColTimes = &aColumns[FindItem(&listColumns,"time")];
  aTimes = pColTimes->pfColumnData;
  for (j=0;j<iRun;j++) {
    aTimes[j] = (FLOAT) aSteps[j];
    aTimes[j] /= fTotalSteps;
    aTimes[j] *= fTotalTime;
  }

}



/***** Trigger CompareSorted *****/

REPORTCOL *pSortCol;

int CompareSorted(const void *a, const void *b) {
  FLOAT fCompare;
  if ((pSortCol->eDataType == DTypeUInt) && ((pSortCol->eColType == ColTypeFinal)||(pSortCol->eColType == ColTypeMin)||(pSortCol->eColType == ColTypeMax))) {
    return (((int)pSortCol->puiColumnData[*(UINT32 *)a]) - ((int)pSortCol->puiColumnData[*(UINT32 *)b]));
  } else if ((pSortCol->eDataType == DTypeSInt) && ((pSortCol->eColType == ColTypeFinal)||(pSortCol->eColType == ColTypeMin)||(pSortCol->eColType == ColTypeMax))) {
    return (pSortCol->psiColumnData[*(UINT32 *)a] - pSortCol->psiColumnData[*(UINT32 *)b]);
  } else {
    fCompare = pSortCol->pfColumnData[*(UINT32 *)a] - pSortCol->pfColumnData[*(UINT32 *)b];
    if (fCompare < 0.0f) {
      return(-1);
    } else if (fCompare > 0.0f) {
      return(1);
    } else {
      return(0);
    }
  }
}

void SortByCurrentColData(REPORTCOL *pCol) {
  
  UINT32 j;
  
  if (aSortedByCurrent == NULL) {
    aSortedByCurrent = AllocateRAM(sizeof(UINT32) * (iRun));
  }

  for (j=0;j<iRun;j++) {
    aSortedByCurrent[j] = j;
  }
  
  pSortCol = pCol;

  qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSorted);

}

void SortByStepPerformance() {

  UINT32 j;
  REPORTCOL *pCol;

  pCol = &aColumns[FindItem(&listColumns,"steps")];

  SortByCurrentColData(pCol);

  aSortedBySteps = AllocateRAM(sizeof(UINT32) * (iRun));

  for (j=0;j<iRun;j++) {
    aSortedBySteps[j] = aSortedByCurrent[j];
  }

}
