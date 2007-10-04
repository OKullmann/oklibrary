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

void AddParameters() {

  AddParmString(&parmAlg,"-alg","algorithm","name of the algorithm","",&sAlgName,&sNull);
  AddParmString(&parmAlg,"-v","variant","algorithm variant (if any)","",&sVarName,&sNull);
  AddParmBool(&parmAlg,"-w","weighted","algorithm uses weighted clauses","",&bWeighted,FALSE);

  AddParmBool(&parmHelp,"-h|-help|--help","general help","display general help and algorithm help if specified","",&bShowHelp,FALSE);
  AddParmBool(&parmHelp,"-ha|-helpalg","algorithm help","display list of all algorithms","",&bShowHelpA,FALSE);
  AddParmBool(&parmHelp,"-hv|-helpverbose","verbose help","display (verbose) help for all algorithms","",&bShowHelpV,FALSE);
  AddParmBool(&parmHelp,"-ht|-helpterse","terse help","display (terse) help for all algorithms","",&bShowHelpT,FALSE);
  AddParmBool(&parmHelp,"-hr|-helpreports","report help","display list of all reports","",&bShowHelpR,FALSE);
  AddParmBool(&parmHelp,"-hc|-helpcolumns","report columns help","display help for report columns","",&bShowHelpC,FALSE);
  AddParmBool(&parmHelp,"-hs|-helpstats","statistics report help","display help for the statistics reports","",&bShowHelpS,FALSE);

  AddParmUInt(&parmUBCSAT,"-runs","runs","number of independent attempts (runs)","",&iNumRuns,1);
  AddParmUInt(&parmUBCSAT,"-cutoff","cutoff","maximum number of search steps per run","",&iCutoff,100000);
  AddParmUInt(&parmUBCSAT,"-timeout","timeout","maximum number of seconds for all runs","CheckTimeout",&iTimeOut,0);
  
  AddParmUInt(&parmUBCSAT,"-target","target","solution found if number false clauses <= target","",&iTarget,0);
  AddParmFloat(&parmUBCSAT,"-wtarget","weighted target","solution found if sum of false clause weights <= wtarget","",&fTargetW,0);
  
  AddParmUInt(&parmUBCSAT,"-seed","seed","force a specific random seed","",&iSeed,iSeed);
  
  AddParmBool(&parmUBCSAT,"-solve","solve mode","terminate when a solution has been found and output solution","SolveMode",&bSolveMode,FALSE);
  AddParmUInt(&parmUBCSAT,"-find|-numsol","find N solutions","terminate when N solutions have been found","",&iFind,0);
  
  AddParmUInt(&parmUBCSAT,"-prestart","periodic restart","periodically reinitialize variables every N steps","CheckForRestarts",&iPeriodicRestart,0);
  AddParmProbability(&parmUBCSAT,"-rrestart","random restart","randomly reinitialize variables with a probability","CheckForRestarts",&iProbRestart,0.0f);
  AddParmUInt(&parmUBCSAT,"-srestart","stagnation restart","reinitialize variables if no improvement in N steps","CheckForRestarts,BestFalse",&iStagnateRestart,0);

  AddParmUInt(&parmUBCSAT,"-noimprove","no improve","terminate run if no improvement in N steps","NoImprove",&iNoImprove,0);

  AddParmString(&parmIO,"-inst|-i","input instance file","specify an input instance file (default is stdin)","",&sFilenameIn,"");

  AddParmString(&parmIO,"-varinit","variable initialisation file","variable initialisation file format = -1 3 -4 ...","",&sFilenameVarInit,"");
  AddParmUInt(&parmIO,"-varinitflip","flip after initialisation","force N random variables to be flipped after initialisation","CandidateList",&iInitVarFlip,0);
  AddParmBool(&parmIO,"-varinitgreedy","greedy variable initialisation","if more pos or neg lits for a varaible, initialised to 1 or 0","",&bVarInitGreedy,FALSE);

  AddParmString(&parmIO,"-param|-fp","parameter file","specify a parameter file (default is command line)","",&sFilenameParms,"");

  AddParmReport(&parmIO,"-report|-r","report(s)","specify reports to run: use -hr for more info","");

  AddParmBool(&parmIO, "-recho","echo to screen","echo all file output to screen","",&bReportEcho,FALSE);
  AddParmBool(&parmIO, "-rclean","clean output","supress all header output","",&bReportClean,FALSE);

  AddParmString(&parmIO, "-rcomment","comment character","change the header / comment string (# is default)","",&sCommentString,"#");

}
