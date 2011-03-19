# Matthew Gwynne, 18.9.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading the output of a minisat2 computation from filename and returning a
# data.frame containing the statistics on the computation.
#
# Inputs:
#   filename
#     The filename containing the output of a run of the minisat2 solver.
#
# Output:
#   A data.frame with a single row with the following fields in the
#   following order:
#
#     n (positive integer)
#       The number of variables in the DIMACS file minisat2 was run on to
#       generate filename.
#     c (positive integer)
#       The number of variables in the DIMACS file minisat2 was run on to
#       generate filename.
#     parse_time (double)
#       The time in seconds that minisat2 took to parse the input file given
#       to it.
#     restarts (positive integer)
#       The number of restarts minisat2 has performed while solving filename.
#     conflicts (positive integer)
#       The number of conflicts minisat2 has found while solving filename.
#     decisions (positive integer)
#       The number of decisions minisat2 has performed while solving filename.
#     propagations (positive integer)
#       The number of unit clause propagations that minisat2 has performed
#       while solving filename.
#     mem (double)
#       The maximum amount of main memory in Megabytes used by minisat2 when
#       solving filename.
#     time (double)
#       The number of seconds it took minisat2 to solve filename.
#     sat ({0,1,2})
#       Whether minisat2 found filename to be SATISFIABLE (1),
#       UNSATISFIABLE (0) or it was unable to determine satisfiablity (2).
#     filename (string)
#       The name of the output file minisat2 was written to. Note that this
#       is not the file input to minisat2, as minisat2 does not have the
#       filename it is working on as part of it's output.
#
# Note this function also reads minisat-2.2.0 output.
#  
read_minisat2_output = function(filename) {
  S = system(paste("cat ", filename,
    " | grep \"\\(restarts\\|conflicts\\|decisions\\|propagations\\|",
    "conflict\\|Memory\\|CPU\\|variables\\|clauses\\|time\\|",
    "SATISFIABLE\\|UNSATISFIABLE\\|INTERRUPTED\\|INDETERMINATE\\)\"", sep=""),
    intern=TRUE)
  result = list()
  for (line in S) {
    name_value = unlist(strsplit(line," *: *"))
    name_value[1] = gsub("^\\| *","",name_value[1])
    if (name_value[1] == "restarts") {
      result = c(result, list(restarts = as.integer(name_value[2])))
    } else if (name_value[1] == "conflicts") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result, list(conflicts = as.integer(temp[1])))
    } else if (name_value[1] == "propagations") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result,
        list(propagations = as.integer(temp[1])))
    } else if (name_value[1] == "Memory used") {
      result = c(result,
        list(mem = as.double(gsub(" *MB *$","",name_value[2]))))
    } else if (name_value[1] == "CPU time") {
      result = c(result,
        list(time = as.double(gsub(" *s *","",name_value[2]))))
    } else if (name_value[1] == "decisions") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result, list(decisions = as.integer(temp[1])))
    } else if (name_value[1] == "Number of variables") {
      result = c(result,
        list(n = as.integer(gsub(" *\\|","",name_value[2]))))
    } else if (name_value[1] == "Number of clauses") {
      result = c(result,
        list(c = as.integer(gsub(" *\\|","",name_value[2]))))
    } else if (name_value[1] == "Parsing time") {
      result = c(result,
        list(parse_time = as.double(gsub(" *s *\\|","",name_value[2]))))
    } else if (name_value[1] == "SATISFIABLE") {
      result = c(result, list(sat = 1))
    } else if (name_value[1] == "UNSATISFIABLE") {
      result = c(result, list(sat = 0))
    } else if (name_value[1] == "*** INTERRUPTED ***") {
      if (is.null(result$sat)) {
        result = c(result, list(sat = 2))
      }
    } else if (name_value[1] == "INDETERMINATE") {
      result = c(result, list(sat = 2))
    }
  }
  result = c(result,list(filename = filename))
  data.frame(result)
}
# As an example, we can generate the CNF for the 4-bit AES Sbox in maxima:
#
# maxima> oklib_load_all()$
# maxima> output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4)$
#
# and then running minisat2 on the file:
#
# shell> minisat2 AES_sbox_2_4_full.cnf > sbox.result 2>&1
#
# we get the following data.frame:
#
# R> oklib_load_all()
# R> E = read_minisat2_output("sbox.result")
# R> E
#   n   c parse_time restarts conflicts decisions propagations   mem time sat
# 1 8 240          0        1         4         9           19 14.63 0.01   1
#      filename
# 1 sbox.result
#

# Reading multiple minisat2 output files into a data.frame.
# See read_minisat2_output.
read_minisat2_outputs = function(filenames) {
  result_df = NULL
  for(file in filenames) {
    result_df = rbind(result_df,read_minisat2_output(file))
  }
  result_df
}
# As an example, we can generate the CNFs for the 4-bit AES field
# multiplications in maxima:
#
# maxima> oklib_load_all()$
# maxima> for i : 2 thru 10 do output_ssmult_fullcnf_stdname(i,2,4,ss_polynomial_2_4);
#
# and then running minisat2 on these files:
#
# shell> for i in $(seq 2 10); do minisat2 ss_byte2_4_field_mul_full_${i}.cnf > mult_${i}.result 2>&1; done
#
# we get the following data.frame (using Sys.glob to generate the list of
# files):
#
# R> oklib_load_all()
# R> E = read_minisat2_outputs(Sys.glob("mult_*.result"))
# R> E
#   n   c parse_time restarts conflicts decisions propagations   mem time sat
# 1 8 240          0        1         0         7            8 14.63 0.01   1
# 2 8 240          0        1         0         1            0 14.64 0.01   1
# 3 8 240          0        1         0         6            8 14.63 0.00   1
# 4 8 240          0        1         0         1            0 14.64 0.01   1
# 5 8 240          0        1         0         7            8 14.64 0.01   1
# 6 8 240          0        1         0         8            8 14.63 0.01   1
# 7 8 240          0        1         0         8            8 14.64 0.01   1
# 8 8 240          0        1         0         6            8 14.64 0.01   1
# 9 8 240          0        1         0         1            0 14.63 0.01   1
#         filename
# 1 mult_10.result
# 2  mult_2.result
# 3  mult_3.result
# 4  mult_4.result
# 5  mult_5.result
# 6  mult_6.result
# 7  mult_7.result
# 8  mult_8.result
# 9  mult_9.result
#
#
# If one has parameters in the output files that minisat2 produced,
# we can add an extra column to the data.frame, extracting the value from
# the filename:
#
# R> E$element = as.integer(gsub("(^mult_|.result$)","",E$filename))
# R> plot(E$element, E$decisions)
# 
#
