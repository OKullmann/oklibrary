# Oleg Zaikin, 16.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######################################################
# # Analyze solvers' experimental results               #
# #######################################################

# By now is aimed only at analysing solvers' results on SAT Competitions.
# Should be run in a directory, where files in the format scYY_SOLVER are located
# (e.g., in the directory DataTawSolver from the corresponding repository).
# Here YY stands for year (11-20 in the current version), SOLVER stands for a solver
# name, e.g. taw or ttaw.
#
# For each SC first a table of solved instances is formed for each solver, then inner
# join merging (by filename) is performed to make a new united table. From this table
# those rows where all solvers did not solve corresponding benchmakrs are deleted.
# Then the difference between solvers' runtime is calculated and added # as a new
# column. This column can be used to find out families of benchmarks where a certain
# solver perform well.

# version 0.1.7

# Interval of SAT Competitions for analysis:
SC_min_year = 11
SC_max_year = 20

# Analyse results of a given solver and return a table with solved instances:
analyse_solver_results <- function(solver, timelimit, results_mask) {
  # Read results from the file:
  results_filename = paste(results_mask, solver, timelimit, sep = "_")
  print(paste("Solver", solver, sep = " "))
  print(paste("Reading file", results_filename, sep = " "))
  E = read.table(results_filename, header=TRUE)[ ,c('file', 'sat', 't', 'nds')]
  # Find instances solved by the solver (status 0=UNSAT or 1=SAT):
  E_solved = E[E$sat!=2,]
  E_solved_size = nrow(E_solved)
  # Print statistics on solved instances:
  print(paste("Total solved instances: ", E_solved_size, sep = ""))
  print("List of solved instances:")
  print(E_solved)
  print("Summary on solved instances:")
  print(summary(E_solved))
  # Return the whole table:
  return(E)
}

# Analyse solvers' results from a given file
analyse_results <- function(solver1, solver2, timelimit, results_mask) {
  E_solver1 = analyse_solver_results(solver1, timelimit, results_mask)
  E_solver2 = analyse_solver_results(solver2, timelimit, results_mask)
  # Merge tables:
  E_merged = merge(x = E_solver1, y = E_solver2, by = "file")
  # Find subtable of the merged table where at least one solver coped:
  E_merged = E_merged[E_merged$sat.x + E_merged$sat.y != 4,]
  print(E_merged)
  # Add column with difference between solvers' runtimes:
  E_merged$dif_t = (E_merged$t.x - E_merged$t.y)
  # Add column with difference between solvers' nodes number:
  E_merged$dif_nds = (E_merged$nds.x - E_merged$nds.y)
  # Plot runtime on scatter plots:
  plot(x = E_merged$t.x, y = E_merged$t.y, xlim=c(0,timelimit), ylim=c(0,timelimit))
  title(main = results_mask, sub = "")
  abline(0,1,col="red")
  abline(v = timelimit,col="red")
  abline(h = timelimit,col="red")
  # Rename columns:
  names(E_merged)[names(E_merged) == "t.x"] = paste("t_", solver1, sep="")
  names(E_merged)[names(E_merged) == "t.y"] = paste("t_", solver2, sep="")
  names(E_merged)[names(E_merged) == "sat.x"] = paste("sat_", solver1, sep="")
  names(E_merged)[names(E_merged) == "sat.y"] = paste("sat_", solver2, sep="")
  names(E_merged)[names(E_merged) == "nds.x"] = paste("nds_", solver1, sep="")
  names(E_merged)[names(E_merged) == "nds.y"] = paste("nds_", solver2, sep="")

  return(E_merged)
}

# Analyse results of given two solvers on all SAT Competitions
analyse_sc_results <- function(solver1, solver2, timelimit) {
  # Analyse results for every year
  for (year in SC_min_year:SC_max_year){
	  results_mask = paste("sc", year, sep = "")
	  print(results_mask)
	  E_merged = analyse_results(solver1, solver2, timelimit, results_mask)
    # Print the obtained table:
    cat("", sep="\n")
    print("Instances solved by at least one solver:")
    print(E_merged)
    cat("", sep="\n\n")
  }
}

# Set wide terminal to see results with no line breaks:
options(width=300)

# Set plot settings:
pdf( "SCplots.pdf", width = 16, height = 8 )
par(mfrow = c(2, 5))

analyse_sc_results("taw", "ttaw", 1000)
