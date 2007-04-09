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

REPORT *pRepErr;

REPORT *pRepOut;
REPORT *pRepRTD;
REPORT *pRepStats;
REPORT *pRepState;
REPORT *pRepModel;
REPORT *pRepSolution;
REPORT *pRepBest;
REPORT *pRepOptClauses;
REPORT *pRepCNFStats;
REPORT *pRepFlipCounts;
REPORT *pRepUnsatCounts;
REPORT *pRepVarLast;
REPORT *pRepClauseLast;
REPORT *pRepSATComp;
REPORT *pRepPenalty;
REPORT *pRepPenSol;
REPORT *pRepPenMean;
REPORT *pRepPenCV;
REPORT *pRepSQGrid;

void AddReports() {

  pRepOut = CreateReport("out", "Standard Output -- Shows the results (columns) from each run (row)","stdout","ReportOut");
  AddReportParmString(pRepOut,"Columns to Display (ubcsat -hc for info)","alg specific");
  AddReportParmUInt(pRepOut,"Supress Output until all runs are complete",&bReportOutputSupress);
  pRepOut->bActive = 1;

  pRepStats = CreateReport("stats","Statistics Report -- Shows summary statistics from all runs","stdout","ReportStats");
  pRepStats->bActive = 1;
  AddReportParmString(pRepStats,"Statistics to Display (ubcsat -hs for info)","alg specific");

  pRepRTD = CreateReport("rtd","Run-Time Distribution -- Results from each run, sorted by step performance", "stdout","ReportRTD");
  AddReportParmString(pRepRTD,"Columns to Display (ubcsat -hc for more info)","rtd");

  pRepModel = CreateReport("model","Solution Model -- Ouput of -solve: show the solution model of the last run","stdout","ReportModelPrint");

  pRepCNFStats = CreateReport("cnfstats","CNF Stats -- Detailed information about the instance","stdout","ReportCNFStatsPrint");

  pRepState = CreateReport("state","State Data -- Detailed state information for each search step","stdout","ReportStatePrint");
  AddReportParmUInt(pRepState,"Only print in local minima",&bReportStateLMOnly);
  AddReportParmFloat(pRepState,"Only print at Solution Quality X (ignored if -1)",&fReportStateQuality);

  pRepSolution = CreateReport("solution","Solutions -- solution for every successful run","stdout","ReportSolutionPrint");

  pRepBest = CreateReport("best","Best Candidate Solutions -- optimal variable settings for each run","stdout","ReportBestPrint");
  
  pRepOptClauses = CreateReport("unsatclauses","Print unsatisfied clauses when target solution quality is reached","stdout","ReportUnsatClausesPrint");

  pRepFlipCounts = CreateReport("flipcount","Flip Count -- number of times each var was flipped","stdout","ReportFlipCountsPrint");

  pRepUnsatCounts = CreateReport("unsatcount","Un-Sat Count -- number of steps each clause was unsat","stdout","ReportUnsatCountsPrint");

  pRepVarLast = CreateReport("varlastflip","Variable Last Flip -- step of most recent flip","stdout","ReportVarLastPrint");

  pRepClauseLast = CreateReport("clauselast","Clause Last Un-Sat -- last step each clause was unsatisfied","stdout","ReportClauseLastPrint");

  pRepSQGrid = CreateReport("sqgrid","Solution Quality Grid -- quality at key step values","stdout","ReportSQGridPrint");
  AddReportParmUInt(pRepSQGrid,"# of key step points per decade",&iLogDistStepsPerDecade);

  pRepSATComp = CreateReport("satcomp","SAT Competition Output (use -solve)","stdout","ReportSatCompetitionPrint");

  /***************************************************************************/

  AddColumnComposite("default","run,found,best,beststep,steps");
  AddColumnComposite("wdefault","run,found,bestw,beststepw,steps");
  AddColumnComposite("rtd","prob,steps,time");

  AddColumnUInt("run","Run Number",
    "     ",
    "  Run",
    "  No.",
    "%5u",
    &iRun,"",ColTypeFinal);

  AddColumnUInt("found","Target Solution Quality Found (1 => yes)",
    "F",
    "N",
    "D",
    "%1u"
    ,&bSolutionFound,"",ColTypeFinal);

  AddColumnUInt("steps","Total Number of Search Steps",
    "     Total",
    "    Search",
    "     Steps",
    "%10u",
    &iStep,"",ColTypeFinal);

  AddColumnUInt("best","Best (Lowest) # of False Clauses Found",
    " Best",
    "Sol'n",
    "Found",
    "%5u",
    &iBestNumFalse,"BestFalse",ColTypeFinal);

  AddColumnUInt("beststep","Step of Best (Lowest) # of False Clauses Found",
    "      Step",
    "        of",
    "      Best",
    "%10u",
    &iBestStepNumFalse,"BestFalse",ColTypeFinal);

  AddColumnFloat("bestw","Best Weighted Solution Quality Found",
    "        Best",
    "    Solution",
    "     Quality",
    "%12.8f",
    &fBestSumFalseW,"BestFalse",ColTypeFinal);

  AddColumnUInt("beststepw","Step of Best Weighted Solution Quality Found",
    "      Step",
    "        of",
    "    W Best",
    "%10u",
    &iBestStepSumFalseW,"BestFalse",ColTypeFinal);
  
  AddColumnUInt("seed","Starting Seed For the Run",
    "          ",
    "  Starting",
    "      Seed",
    "%10u",
    &iStartSeed,"StartSeed",ColTypeFinal);

  AddColumnUInt("nullflips","Number of Null Flips",
    "    Number",
    "   of Null",
    "     Flips",
    "%10u",
    &iNumNullFlips,"NullFlips",ColTypeFinal);
  
  AddColumnUInt("percentnull","Percent of Null Flips",
    "Percent",
    "   Null",
    "  Flips","%7.1f",
    &iNumNullFlips,"NullFlips",ColTypeFinalDivStep100);

  AddColumnUInt("rand","Number of Random Decisions",
    "    Number",
    " of Random",
    " Decisions",
    "%10u",
    &iNumRandomCalls,"CountRandom",ColTypeFinal);

  AddColumnUInt("candidates","Mean Number of Flip Candidates (not all algs support it)",
    " Mean #",
    "of Flip",
    " Candid",
    "%7.2f",
    &iNumCandidates,"",ColTypeMean);

  AddColumnFloat("prob","Probability of Success (RTD Only)",
    "   Prob",
    "     of",
    "Success",
    "%7.5f",
    &fDummy,"UpdatePercents",ColTypeFinal);
  
  AddColumnFloat("time","Time in seconds (RTD Only)",
    "  CPU Time",
    "        in",
    "   seconds",
    "%10.6f",
    &fDummy,"UpdateTimes",ColTypeFinal);

  /***************************************************************************/
    
  AddContainerStat("default","instinfo,totaltime,fps,percentsolve,steps,time");

  AddStatCustom("numclauses","Number of Clauses","Clauses = %u\n",(void *)&iNumClauses,DTypeUInt,"","");
  AddStatCustom("numvars","Number of Variables","Variables = %u\n",(void *)&iNumVars,DTypeUInt,"","");
  AddStatCustom("numlits","Number of Total Literals","TotalLiterals = %u\n",(void *)&iNumLits,DTypeUInt,"","");
  AddContainerStat("instinfo","numvars,numclauses,numlits");
  
  AddStatCustom("percentsolve","Percent of Successful Runs","PercentSuccess = %-6.2f\n",(void *)&fPercentSuccess,DTypeFloat,"","CalcPercentSolve");

  AddStatCustom("totaltime","Total Time Elapsed","TotalCPUTimeElapsed = %-10.5f\n",(void *)&fTotalTime,DTypeFloat,"","");
  
  AddStatCustom("fps","Flips Per Second","FlipsPerSecond = %-7.f\n",(void *)&fFlipsPerSecond,DTypeFloat,"","CalcFPS");

  AddStat("steps","Search Steps ","Steps","mean+cv+median","steps","steps","",TRUE);
  AddStat("stepsall","All Search Step Statistics","Steps","all","steps","steps","",TRUE);
  AddStat("time","Time (in CPU seconds)","CPUTime","mean+median","time","time","",TRUE);
  AddStat("nullflips","Null Flips","NullFlips","mean+median","nullflips","nullflips","",FALSE);
  AddStat("percentnull","Percent Null Flips","PercentNull","mean+median","percentnull","percentnull","",FALSE);

  AddStat("best","Best Solution Found","BestSolution","mean+median","best","best","",FALSE);
  AddStat("beststep","Step of Best Solution Found","BestStep","mean+median","beststep","beststep","",FALSE);
  AddStat("bestw","Best Weighted Solution Quality Found","BestWeightedQuality","mean+median","bestw","bestw","",FALSE);
  AddStat("beststepw","Step of Best Weighted Solution Quality Found","BestWeightedQualityStep","mean+median","beststepw","beststepw","",FALSE);

}

