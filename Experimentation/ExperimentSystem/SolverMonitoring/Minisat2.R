# Matthew Gwynne, 18.9.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading minisat2 output (stdout) and returning a data.frame
read_minisat2_output = function(filename, ...) {
  S = system(paste("cat ", filename,
    " | grep \"\\(restarts\\|conflicts\\|decisions\\|propagations\\|",
    "conflict\\|Memory\\|CPU\\|variables\\|clauses\\|time\\)\""),intern=TRUE)
  result = list()
  for (line in S) {
    name_value = unlist(strsplit(line," *: *"))
    name_value[1] = gsub("^\\| *","",name_value[1])
    if (name_value[1] == "restarts") {
      result = c(result, list(restarts = as.integer(name_value[2])))
    } else if (name_value[1] == "conflicts") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result,
        list(conflicts=temp[1],
             conflicts_per_sec=as.integer(gsub(" */sec) *$", "",temp[2]))))
    } else if (name_value[1] == "propagations") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result,
        list(propagations=temp[1],
             propagations_per_sec=as.integer(gsub(" */sec) *$", "",temp[2]))))
    } else if (name_value[1] == "Memory used") {
      result = c(result,
        list(memory_used=as.double(gsub(" *MB *$","",name_value[2]))))
    } else if (name_value[1] == "CPU time") {
      result = c(result,
        list(cpu_time=as.double(gsub(" *s *","",name_value[2]))))
    } else if (name_value[1] == "decisions") {
      temp = unlist(strsplit(name_value[2], " *\\("))
      result = c(result,
        list(decisions=temp[1],
             random_decisions_pc=as.double(gsub(" *%.*$","",temp[2])),
             decisions_per_sec=as.integer(gsub(" */sec) *$", "",temp[3]))))
    } else if (name_value[1] == "Number of variables") {
      result = c(result,
        list(num_variables=as.integer(gsub(" *\\|","",name_value[2]))))
    } else if (name_value[1] == "Number of clauses") {
      result = c(result,
        list(num_clauses=as.integer(gsub(" *\\|","",name_value[2]))))
    } else if (name_value[1] == "Parsing time") {
      result = c(result,
        list(parsing_time=as.integer(gsub(" *s *\\|","",name_value[2]))))
    }
  }
  result = c(result,list(filename=filename))
  data.frame(result)
}
# An example of the data.frame output is given below:
#
#   num_variables num_clauses parsing_time restarts conflicts conflicts_per_sec
# 1         32176      510492            0        1         0                 0
#   decisions random_decisions_pc decisions_per_sec propagations
# 1         1                   0                 1        32176
#   propagations_per_sec memory_used cpu_time
# 1                19666       33.23   1.6361
#                                  filename
# 1 AES_R1_P0_K0_CX_KN0.cnf.result.minisat2
#
# This illustrates the included fields, which are all read directly from the
# minisat2 output.
#


# Reading a set of minisat2 outputs into a data.frame (given as a list of
# filenames)
read_minisat2_outputs = function(filenames) {
  result_df = NULL
  for(file in filenames) {
    result_df = rbind(result_df,read_minisat2_output(file))
  }
  result_df
}
# Note one can use:
# E = read_minisat2_outputs(dir(pattern=glob2rx("*.result")))
# to read all files with ".result" as the end.
#
# The format of the data.frame is the same as with
# read_minisat2_output, except with a row for each
# filename given as input.
#
# For example:
#
# > E = read_minisat2_outputs(list("AES_R1_P0_K0_CX_KN0.cnf.result.minisat2","AES_R1_P0_K0_CX_KN2.cnf.result.minisat2"))
#   num_variables num_clauses parsing_time restarts conflicts conflicts_per_sec
# 1         32176      510492            0        1         0                 0
# 2         32176      510490            0        1         0                 0
#   decisions random_decisions_pc decisions_per_sec propagations
# 1         1                   0                 1        32176
# 2         1                   0                 1        32176
#   propagations_per_sec memory_used cpu_time
# 1                19666       33.23   1.6361
# 2                20415       33.23   1.5761
#                                  filename
# 1 AES_R1_P0_K0_CX_KN0.cnf.result.minisat2
# 2 AES_R1_P0_K0_CX_KN2.cnf.result.minisat2
#
