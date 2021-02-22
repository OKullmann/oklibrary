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

# version 0.1.1


options(width=300)

for (year in 11:20){
  print(paste("SC-", year, sep = ""))
  # Read tawSolver's result data:
  taw_file_name <- paste("sc", year, "_taw_1000", sep = "")
  print(paste("reading file ", taw_file_name, sep = ""))
  Et = read.table(taw_file_name, header=TRUE)[ ,c('file', 'sat', 't', 'nds')]
  # Find benchmarks solved by tawSolver:
  Et_solved = Et[Et$sat_taw!=2,]
  print("summary on solved instances:")
  print(summary(Et_solved))
  # Read ttawSolver's result data:
  ttaw_file_name <- paste("sc", year, "_ttaw_1000", sep = "")
  print(paste("reading file ", ttaw_file_name, sep = ""))
  Ett = read.table(ttaw_file_name, header=TRUE)[ ,c('file', 'sat', 't', 'nds')]
  # Find benchmarks solved by ttawSolver:
  Ett_solved = Ett[Ett$sat_taw!=2,]
  print("summary on solved instances:")
  print(summary(Ett_solved))
  # Merge tables:
  E_merged = merge(x = Et, y = Ett, by = "file")
  # Rename columns:
  names(E_merged)[names(E_merged) == "t.x"] <- "t_taw"
  names(E_merged)[names(E_merged) == "t.y"] <- "t_ttaw"
  names(E_merged)[names(E_merged) == "sat.x"] <- "sat_taw"
  names(E_merged)[names(E_merged) == "sat.y"] <- "sat_ttaw"
  names(E_merged)[names(E_merged) == "nds.x"] <- "nds_taw"
  names(E_merged)[names(E_merged) == "nds.y"] <- "nds_ttaw"
  # Find subtable of the merged table where at least one solver coped:
  E_inner = E_merged[E_merged$sat_taw+E_merged$sat_ttaw!=4,]
  # Add column with difference between solvers' runtimes:
  E_inner$dif_t_taw_ttaw <- (E_inner$t_taw - E_inner$t_ttaw)
  # Add column with difference between solvers' nodes number:
  E_inner$dif_nds_taw_ttaw <- (E_inner$nds_taw - E_inner$nds_ttaw)
  # Print the obtained table:
  print(E_inner)
  cat("", sep="\n\n")
}