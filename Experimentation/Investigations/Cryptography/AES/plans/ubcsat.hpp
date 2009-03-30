// Matthew Gwynne, 29.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/ubcsat.hpp
  \brief On investigations into the Advanced Encryption Standard using ubcsat

  \todo Determining a "good" local search algorithm to use on AES instances
  <ul>
   <li> Question : what is a good algorithm to use, considering the algorithms
   tested in the "eval_ubcsat" code provided in
   ExperimentSystem/plans/RunUBCSAT.hpp ? </li>
   <li> Example code being used to run an experiment into this : 
   \verbatim
source("UBCSAT.R")
ubcsat_command <- "ubcsat-1-1-0 -rclean \\
 -r out '$OUTPUT' $OUTPUT_PARAMS \\
  -r stats '$STATS_OUTPUT'  numclauses,numvars,numlits,totaltime,time \\
   $STD_PARAMS -alg $ALG -i $TARGET"
aes_cnfs <- list.files(".","AES_r.*_kn_.*_SAT\\.cnf$")
for (aes_cnf in aes_cnfs) {
  result_df <- eval_ubcsat(aes_cnf,monitor=TRUE)
  result_df <- add_constant_column(result_df, aes_cnf, "input")
  if (exists("aes_df")) {
    aes_df <- rbind(aes_df, result_df)
  } else {
    aes_df <- result_df
  }
  write.table(aes_df, paste("After_", aes_cnf,"_Result", sep=""))
}
   \endverbatim
   </li>
   <li> What sort of metrics to use to determine a good algorithm once the
   experiment has been run?
   <ul>
    <li> Sorting first by the average number of falsified clauses and then
    by the number of steps seems reasonable, as we wish to minimise the number
    of falsified clauses as much as possible. </li>
   </ul>
   </li>
  </ul>


*/
