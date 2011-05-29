# Matthew Gwynne, 19.9.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading the output of a picosat913 computation from filename and returning a
# data.frame containing the statistics on the computation.
# Inputs:
#   stats_filename
#     The filename containing the output of the solver run.
# Output:
#   Statistics corresponding to the run of the solver on filename.
#   A data.frame with a single row with the following fields in the
#   following order:
#
#     filename (string): Name of the DIMACS file the solver was run on.
#     n (int): Initial number of variables.
#     c (int): Initial number of clauses.
#     sat ({0,1,2}): SATISFIABLE (1), UNSATISFIABLE (0) or UNKNOWN (2).
#     restarts (int): Number of restarts.
#     failed_l (int): Number of forced assignments found using
#       failed literal reductions.
#     conflicts (int): Number of conflicts.
#     decisions (int): Number of "decisions".
#     fixed_vars (int): Number of variables fixed during search.
#     learned_l (int): Number of literals determined by conflict
#       clause analysis.
#     deleted_l (double): Percentage of literals deleted.
#     prop (int): Total number of unit-clause propagations.
#     simps (int): Total number of simplications performed.
#     mem (double): Maximum amount of main memory in Megabytes used.
#     time (double): Total time taken in seconds to solve.
#
# Note this function will not read the output of any of the precosat solvers.
#  
read_picosat_output = function(stats_filename, ...) {
  S = system(paste("cat ", stats_filename," | grep \"^c\\|s\""), intern=TRUE)
  result = list()
  for (line in S) {
    line = gsub("^[cs] *", "",line)
    if (length(grep("^parsing *",line)) > 0) {
      result = c(result,list(filename = gsub("^parsing *","",line)))
    } else if (length(grep("^initialized", line)) > 0) {
      result = c(result,
        list(n = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("trivial clauses",line)) > 0) {
      result = c(result,
        list(c = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("decisions",line)) > 0) {
      result = c(result,
        list(decisions = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("conflicts *$",line)) > 0) {
      result = c(result,
        list(conflicts = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("fixed variables",line)) > 0) {
      result = c(result,
        list(fixed_vars = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("learned literals",line)) > 0) {
      result = c(result,
        list(learned_l = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("failed literals",line)) > 0) {
      result = c(result,
        list(failed_l = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("deleted literals",line)) > 0) {
      result = c(result,
        list(deleted_l = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("propagations",line)) > 0) {
      result = c(result,
        list(prop = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("simplifications",line)) > 0) {
      result = c(result,
        list(simps = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("iterations",line)) > 0) {
      result = c(result,
        list(iter = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("seconds total run time",line)) > 0) {
      result = c(result,
        list(time = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("MB maximally allocated",line)) > 0) {
      result = c(result,
        list(mem = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("restarts",line)) > 0) {
      result = c(result,
        list(restarts = as.integer(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("SATISFIABLE",line)) > 0) {
      result = c(result, list(sat = 1))
    } else if (length(grep("UNSATISFIABLE",line)) > 0) {
      result = c(result, list(sat = 0))
    }
  }
  data.frame(result)
}
# From the following picosat913 output (in sbox.result):
# 
# c PicoSAT SAT Solver Version 913
# c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
# c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
# c
# c parsing test1.cnf
# c parsed header 'p cnf 8 240'
# c initialized 8 variables
# c found 240 non trivial clauses
# c
# *snip*
# c
# s SATISFIABLE
# v -1 -2 -3 -4 -5 6 7 -8 0
# c
# c 0 iterations
# c 0 restarts
# c 0 failed literals
# c 2 conflicts
# c 8 decisions
# c 0 fixed variables
# c 13 learned literals
# c 0.0% deleted literals
# c 11 propagations
# c 100.0% variables used
# c 0.0 seconds in library
# c 0.0 megaprops/second
# c 1 simplifications
# c 0 reductions
# c 0.0 MB recycled
# c 0.0 MB maximally allocated
# c 0.0 seconds total run time
#
# we get the following data.frame:
#
# R> oklib_load_all()
# R> E = read_picosat_output("sbox.result")
# R> E
#    filename n   c sat iter restarts failed_l conflicts decisions
# 1 test1.cnf 8 240   1    0        0        0         2         8
#   fixed_vars learned_l deleted_l prop simps mem time
# 1          0        13         0   11     1   0    0
#

# Reading the outputs of picosat913 computations from stats_filename_l, a list
# of filenames. A data.frame containing the statistics on the computations
# is returned.
# Inputs:
#   stats_filename_l
#     A list of filenames each containing the output of a solver run.
# Output:
#   A data.frame with a row for each file in stats_filename_l. Each row
#   contains the following fields in the following order:
#
#     filename (string): Name of the DIMACS file the solver was run on.
#     n (int): Initial number of variables.
#     c (int): Initial number of clauses.
#     sat ({0,1,2}): SATISFIABLE (1), UNSATISFIABLE (0) or UNKNOWN (2).
#     restarts (int): Number of restarts.
#     failed_l (int): Number of forced assignments found using
#       failed literal reductions.
#     conflicts (int): Number of conflicts.
#     decisions (int): Number of "decisions".
#     fixed_vars (int): Number of variables fixed during search.
#     learned_l (int): Number of literals determined by conflict
#       clause analysis.
#     deleted_l (double): Percentage of literals deleted.
#     prop (int): Total number of unit-clause propagations.
#     simps (int): Total number of simplications performed.
#     mem (double): Maximum amount of main memory in Megabytes used.
#     time (double): Total time taken in seconds to solve.
#
# Note this function will not read the output of any of the precosat solvers.
#  
read_picosat_outputs = function(filenames) {
 result_df = NULL
 for(file in filenames) {
     result_df = rbind(result_df,read_picosat_output(file))
 }
 result_df
}
# From the following picosat913 outputs
# (in testdir/test1.result and testdir/test2.result):
# 
# c PicoSAT SAT Solver Version 913
# c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
# c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
# c
# c parsing test1.cnf
# c parsed header 'p cnf 8 240'
# c initialized 8 variables
# c found 240 non trivial clauses
# c
# *snip*
# c
# s SATISFIABLE
# v -1 -2 -3 -4 -5 6 7 -8 0
# c
# c 0 iterations
# c 0 restarts
# c 0 failed literals
# c 2 conflicts
# c 8 decisions
# c 0 fixed variables
# c 13 learned literals
# c 0.0% deleted literals
# c 11 propagations
# c 100.0% variables used
# c 0.0 seconds in library
# c 0.0 megaprops/second
# c 1 simplifications
# c 0 reductions
# c 0.0 MB recycled
# c 0.0 MB maximally allocated
# c 0.0 seconds total run time
#
# and
#
# c PicoSAT SAT Solver Version 913
# c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
# c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
# c
# c parsing test2.cnf
# c parsed header 'p cnf 16 480'
# c initialized 16 variables
# c found 480 non trivial clauses
# c
# *snip*
# c
# s SATISFIABLE
# v -1 -2 -3 -4 -5 6 7 -8 9 10 11 12 13 14 15 16 0
# c
# c 0 iterations
# c 0 restarts
# c 0 failed literals
# c 4 conflicts
# c 16 decisions
# c 0 fixed variables
# c 26 learned literals
# c 0.0% deleted literals
# c 22 propagations
# c 100.0% variables used
# c 0.0 seconds in library
# c 0.0 megaprops/second
# c 2 simplifications
# c 0 reductions
# c 0.0 MB recycled
# c 0.0 MB maximally allocated
# c 0.1 seconds total run time
#
# we get the following data.frame:
#
# R> oklib_load_all()
# R> E = read_picosat_outputs(Sys.glob("testdir/*.result"))
# R> E
#    filename  n   c sat iter restarts failed_l conflicts decisions
# 1 test1.cnf  8 240   1    0        0        0         2         8
# 2 test2.cnf 16 480   1    0        0        0         4        16
#   fixed_vars learned_l deleted_l prop simps mem time
# 1          0        13         0   11     1   0  0.0
# 2          0        26         0   22     2   0  0.1
#

