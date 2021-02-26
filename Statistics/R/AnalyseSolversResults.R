# Oleg Zaikin, 16.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######################################################
# Analyse and compare solvers' experimental results     #
# #######################################################

# The script is aimed at analysing solvers' results on SAT Competitions.
# It should be run in a directory, where files in the format scYY_SOLVER_TIMELIMIT
# are located (e.g., in the directory DataSolvers from the corresponding repository).
# Here YY stands for year (11-20 in the current version), SOLVER stands for a solver
# name, e.g. taw or ttaw.
#
# The main function compare_solvers_all_sc() compares results of a given pair of
# solvers of each SC from the interval. First a table instances if created for each
# solver, then they are merged (inner joined by instance filename) to make a new
# united table. From this table those instances wich were not solver by both solvers
# are removed. Then the difference between solvers' runtime and the number of nodes
# is calculated and added as new columns. These columns can be used to find families
# of instances where a certain solver perform well.

# Usage:
# AnalyseSolversResults.R solver1 solver2 timelimit
#
# Here 'solver1' and 'solver2' are two solvers to compare, while 'timelimit' is
# the runtim limit in seconds used to run the solvers. # The script will try reading
# files of the type scYY_solver1_timelimit for solver1, the similar for solver2.
# By now YY are varied from 11 to 20.

# Example:
# AnalyseSolversResults.R taw ttaw 1000

version = "0.2.1"

# Interval of SAT Competitions for analysis:
SC_min_year = 11
SC_max_year = 20
tables_num = SC_max_year - SC_min_year + 1

# Plot scatters to compare solvers' runtimes:
plot_scatters_time <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,timelimit),
       ylim=c(0,timelimit), xlab=col1, ylab=col2, cex.lab=2,
       axes = FALSE, xaxs="i", yaxs="i")
  axis(side=1, at=seq(0, timelimit, by=100), las=2)
  axis(side=2, at=seq(0, timelimit, by=100), las=2)
  grid (tables_num, tables_num, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
  abline(v = timelimit, col="red")
  abline(h = timelimit, col="red")
}

# Plot scatters to compare solvers' number of nodes:
plot_scatters_nds <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(x = E[[col1]], y = E[[col2]], xlab=col1, ylab=col2, cex.lab=2,
       xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
}

# Plot scatters to compare solvers' nds per time:
plot_scatters_nds_per_time <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds_per_t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(x = E[[col1]], y = E[[col2]], xlab=col1, ylab=col2, cex.lab=2,
       xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
}

# Plot log2(nds1 / nds2) :
plot_log2_nds <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(log2(E[[col1]] / E[[col2]]), xlab=col1, ylab=col2, cex.lab=2,
       xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(h = 0, col="red", lwd=2)
}

# Plot log2(nds1 / nds2) :
plot_density_log2_nds <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(density(log2(E[[col1]] / E[[col2]])), xlab=col1, ylab=col2, cex.lab=2,
       xaxs="i", yaxs="i", main=NULL)
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(v = 0, col="red", lwd=2)
}

# Rename columns to see solvers' names
rename_columns <- function(E, solver1, solver2) {
  names(E)[names(E) == "t.x"] = paste("t_", solver1, sep="")
  names(E)[names(E) == "t.y"] = paste("t_", solver2, sep="")
  names(E)[names(E) == "sat.x"] = paste("sat_", solver1, sep="")
  names(E)[names(E) == "sat.y"] = paste("sat_", solver2, sep="")
  names(E)[names(E) == "nds.x"] = paste("nds_", solver1, sep="")
  names(E)[names(E) == "nds.y"] = paste("nds_", solver2, sep="")
  names(E)[names(E) == "nds_per_t.x"] = paste("nds_per_t_", solver1, sep="")
  names(E)[names(E) == "nds_per_t.y"] = paste("nds_per_t_", solver2, sep="")
  return(E)
}

# Get table with results of a given solver:
get_solver_results <- function(solver, timelimit, results_mask) {
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
  print("Summary on solved instances:")
  print(summary(E_solved))
  # Return the whole table:
  return(E)
}

# Analyse solvers' results from a given file:
compare_solvers_one_sc <- function(solver1, solver2, timelimit, results_mask) {
  E_solver1 = get_solver_results(solver1, timelimit, results_mask)
  E_solver2 = get_solver_results(solver2, timelimit, results_mask)
  # Merge tables:
  E_merged = merge(x = E_solver1, y = E_solver2, by = "file")
  # Find subtable of the merged table where at least one solver coped:
  E_merged = E_merged[E_merged$sat.x + E_merged$sat.y != 4,]
  # Add column with difference between solvers' runtimes:
  E_merged$dif_t = (E_merged$t.x - E_merged$t.y)
  # Add column with difference between solvers' nodes number:
  E_merged$dif_nds = (E_merged$nds.x - E_merged$nds.y)
  # Add column with nodes per second for solver1:
  E_merged$nds_per_t.x = (E_merged$nds.x / E_merged$t.x)
    # Add column with nodes per second for solver2:
  E_merged$nds_per_t.y = (E_merged$nds.y / E_merged$t.y)
  return(E_merged)
}

make_sc_mask <- function(i) {
  return(paste("sc", SC_min_year + i - 1, sep = ""))
}

# Compare a given pair of solvers on intances from SAT Competitions:
compare_solvers_all_sc <- function(solver1, solver2, timelimit) {
  SCtables <- vector(mode = "list", length = tables_num)
  # Get a table for every year
  for (i in 1:tables_num) {
	  results_mask = make_sc_mask(i)
	  print(results_mask)
	  E_merged = compare_solvers_one_sc(solver1, solver2, timelimit, results_mask)
    # Rename columns - replace x and y by solver1 and solver2:
    E_merged = rename_columns(E_merged, solver1, solver2)
    # Print the obtained table:
    cat("", sep="\n")
    print("Instances solved by at least one solver:")
    print(E_merged)
    cat("", sep="\n\n")
    # Add the table to the vector:
    SCtables[[i]] = E_merged
  }
  return(SCtables)
}

# Set wide terminal to see results with no line breaks:
options(width=300)
options(scipen=999)

args = commandArgs(trailingOnly = TRUE)
print(paste("AnalyseSolversResults, version=", version, sep=""))
print("Command line parameters :")
print(args)

if(length(args) < 3) {
  print(paste("Usage: script solver1 solver2 timelimit"))
  quit("yes")
}

solver1 = args[1]
solver2 = args[2]
timelimit = strtoi(args[3])

print(paste("tables_num:", tables_num, sep=" "))

# Get vector of tables with results of comparison:
SCtables = compare_solvers_all_sc(solver1, solver2, timelimit)

# Plot scatters for time:
pdf(paste("SC_", solver1, "_", solver2, "_scatters_time.pdf", sep=""), width = 16, height = 8)
par(mfrow = c(2, 5))
for(i in 1:tables_num) {
  plot_scatters_time(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
}

# Plot scatters for nds:
pdf(paste("SC_", solver1, "_", solver2, "_scatters_nds.pdf", sep=""), width = 16, height = 8)
par(mfrow = c(2, 5))
for(i in 1:tables_num) {
  plot_scatters_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
}

# Plot scatters for nds per time:
pdf(paste("SC_", solver1, "_", solver2, "_scatters_nds_per_time.pdf", sep=""), width = 16, height = 8)
par(mfrow = c(2, 5))
for(i in 1:tables_num) {
  plot_scatters_nds_per_time(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
}

# Plot log2(nds1 / nds2):
pdf(paste("SC_", solver1, "_", solver2, "_log2_nds.pdf", sep=""), width = 16, height = 8)
par(mfrow = c(2, 5))
for(i in 1:tables_num) {
  plot_log2_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
}

# Plot density(log2(nds1 / nds2)):
pdf(paste("SC_", solver1, "_", solver2, "_density_log2_nds.pdf", sep=""), width = 16, height = 8)
par(mfrow = c(2, 5))
for(i in 1:tables_num) {
  plot_density_log2_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
}
