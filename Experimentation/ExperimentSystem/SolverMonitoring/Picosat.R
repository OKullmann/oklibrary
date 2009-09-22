# Matthew Gwynne, 19.9.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading picosat output (stdout) and returning a data.frame
read_picosat_output = function(filename, ...) {
  S = system(paste("cat ", filename," | grep \"^c\""), intern=TRUE)
  result = list()
  for (line in S) {
    line = gsub("^c *", "",line)
    if (length(grep("^parsing *",line)) > 0) {
      result = c(result,list(filename=gsub("^parsing *","",line)))
    } else if (length(grep("^initialized", line)) > 0) {
      result = c(result,
        list(num_variables=gsub("[^0-9]","",line)))
    } else if (length(grep("trivial clauses",line)) > 0) {
      result = c(result,
        list(num_clauses=gsub("[^0-9]","",line)))
    } else if (length(grep("^[0-9\\.]+[ %]+[a-zA-z]",line)) > 0) {
      line = unlist(strsplit(line, " "))
      name = gsub("[^a-zA-Z_\\-]","_",
        gsub(" +"," ",do.call(paste,as.list(line[2:length(line)]))))
      if (length(grep("%",line[1])) > 0) {
        line[1] = gsub("\\%","",line[1])
        name = paste(name,"_pc",sep="")
      }
      result = c(result,
        eval(parse(text=paste("list(",name,"=",as.double(line[1]),")"))))
    }
  }
  data.frame(result)
}
#
# An example of the format of the data.frame can be found below:
#
#                  filename num_variables num_clauses iterations restarts
# 1 AES_R1_P0_K0_CX_KN0.cnf         32176      510492          0        0
#   failed_literals conflicts decisions fixed_variables learned_literals
# 1               0         0         0           32176            31812
#   deleted_literals_pc propagations variables_used_pc seconds_in_library
# 1                   0        32176               100                0.3
#   megaprops_second simplifications reductions MB_recycled
# 1              0.1               0          0           0
#   MB_maximally_allocated seconds_total_run_time
# 1                    9.7                    0.3

#
#
# This illustrates the fields included, which are all read directly
# from the corresponding picosat output fields.
#
# Some fields with names including percentage signs etc have been renamed
# to include "_pc" for "percentage" etc, although such instances should be
# intuitive.

# Reading a set of minisat2 outputs into a data.frame (given as a list of
# filenames)
read_picosat_outputs = function(filenames) {
 result_df = NULL
 for(file in filenames) {
     result_df = rbind(result_df,read_picosat_output(file))
 }
 result_df
}
# Note one can use:
# read_minisat2_outputs(dir(pattern=glob2rx("*.result")))
# to read all files with ".result" as the end.
#
# The format of the output data.frame is the same as
# read_picosat_output, except there is a row in the data.frame
# corresponding to each of the filenames given as input.
#
# For example:
#
# > read_picosat_outputs(list("AES_R1_P0_K0_CX_KN0.cnf.result.picosat","AES_R1_P0_K0_CX_KN2.cnf.result.picosat"))
#
#                  filename num_variables num_clauses iterations restarts
# 1 AES_R1_P0_K0_CX_KN0.cnf         32176      510492          0        0
# 2 AES_R1_P0_K0_CX_KN2.cnf         32176      510490          0        0
#   failed_literals conflicts decisions fixed_variables learned_literals
# 1               0         0         0           32176            31812
# 2               0         0         0           32176            31814
#   deleted_literals_pc propagations variables_used_pc seconds_in_library
# 1                   0        32176               100                0.3
# 2                   0        32176               100                0.3
#   megaprops_second simplifications reductions MB_recycled
# 1              0.1               0          0           0
# 2              0.1               0          0           0
#   MB_maximally_allocated seconds_total_run_time
# 1                    9.7                    0.3
# 2                    9.7                    0.3
#
