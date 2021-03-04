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

version = "0.3.0"

# Plot scatter to compare solvers' runtimes:
plot_scatter_time <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,timelimit),
       ylim=c(0,timelimit), xlab=col1, ylab=col2, cex.lab=2,
       axes = FALSE, xaxs="i", yaxs="i")
  axis(side=1, at=seq(0, timelimit, by=100), las=2)
  axis(side=2, at=seq(0, timelimit, by=100), las=2)
  grid (NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
  abline(v = timelimit, col="red")
  abline(h = timelimit, col="red")
}

# Plot scatter to compare solvers' number of nodes:
plot_scatter_nds <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  maxnds = max(max(E[[col1]], na.rm = TRUE),max(E[[col2]], na.rm = TRUE))
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,maxnds), ylim=c(0,maxnds), xlab=col1,
       ylab=col2, cex.lab=2, xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
}

# Plot scatters to compare solvers' nds per time:
plot_scatter_nds_per_time <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds_per_t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  E_col1_finite_max = max(E[[col1]][is.finite(E[[col1]])], na.rm = TRUE)
  E_col2_finite_max = max(E[[col2]][is.finite(E[[col2]])], na.rm = TRUE)
  maxnds = max(E_col1_finite_max, E_col2_finite_max)
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,maxnds), ylim=c(0,maxnds), xlab=col1,
       ylab=col2, cex.lab=2, xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, col.main= "black")
  abline(0,1,col="red")
}

# Plot log2(nds1 / nds2):
plot_log2_nds <- function(E, solver1, solver2, timelimit, results_mask) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
	ymax = max(E[[col2]])
	if (ymax < 0) ymax = 1
	ymin = min(E[[col2]])
	if (ymin > 0) ymin = -1
  plot(log2(E[[col1]] / E[[col2]]), xlab=col1, ylab=col2, cex.lab=2,
       xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  title(main = results_mask, sub = "", cex.main = 2, ylim=c(ymin,ymax), col.main= "black")
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

# Rename columns to see solvers' names:
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

# Fill table with results for  instances from a given family
get_solver_family_results <- function(file_label, familiy_mask, solver, timelimit) {
  results_filename = paste(file_label, solver, timelimit, sep = "_")
  E = read.table(results_filename, header=TRUE)[ ,c('file', 'sat', 't', 'nds')]
  E = subset(E,  grepl(glob2rx(familiy_mask) , file) )
  return(E)
}

# Analyse solvers' results on instances from a family:
merge_solvers_results_on_family <- function(file_label, familiy_mask, solver1, solver2, timelimit) {
  E_solver1 = get_solver_family_results(file_label, familiy_mask, solver1, timelimit)
  E_solver2 = get_solver_family_results(file_label, familiy_mask, solver2, timelimit)
  # Check if the family is not empty:
  if((nrow(E_solver1) == 0) || (nrow(E_solver2) == 0)) {
    print(paste("Empty family."))
    quit("yes")
  }
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


# Set wide terminal to see results with no line breaks:
options(width=300)
options(scipen=999)

args = commandArgs(trailingOnly = TRUE)
print(paste("AnalyseSolversResults, version=", version, sep=""))
print("Command line parameters :")
print(args)

if(length(args) < 4) {
  print(paste("Usage: script families solver1 solver2 timelimit"))
  quit("yes")
}

families = args[1]
solver1 = args[2]
solver2 = args[3]
timelimit = strtoi(args[4])

families_table = read.table(families, header=TRUE)
families_num = nrow(families_table)
print(paste("families number ", families_num, sep=""))
#print("families:")
solved_families_num = 0
for(i in 1:families_num){
  #print(families[i,]$mask, max.levels=0)
	E_merged = merge_solvers_results_on_family(families_table[i,]$label, families_table[i,]$mask, solver1, solver2, timelimit)
	E_merged = rename_columns(E_merged, solver1, solver2)
	if(nrow(E_merged) > 0) {
		print(E_merged)
		solved_families_num = solved_families_num + 1
	}
}
print(paste("families with at least one solved:", solved_families_num, "out of total", families_num, "families", sep=" "))

# Get vector of tables with results of comparison:
#SCtables = compare_solvers_all_sc(solver1, solver2, timelimit)

# Plot scatters for time:
#pdf(paste("SC_", solver1, "_", solver2, "_scatters_time.pdf", sep=""), width = 16, height = 8)
#par(mfrow = c(2, 5))
#for(i in 1:tables_num) {
#  plot_scatters_time(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
#}

# Plot scatters for nds:
#pdf(paste("SC_", solver1, "_", solver2, "_scatters_nds.pdf", sep=""), width = 16, height = 8)
#par(mfrow = c(2, 5))
#for(i in 1:tables_num) {
#  plot_scatters_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
#}

# Plot scatters for nds per time:
#pdf(paste("SC_", solver1, "_", solver2, "_scatters_nds_per_time.pdf", sep=""), width = 16, height = 8)
#par(mfrow = c(2, 5))
#for(i in 1:tables_num) {
#  plot_scatters_nds_per_time(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
#}

# Plot log2(nds1 / nds2):
#pdf(paste("SC_", solver1, "_", solver2, "_log2_nds.pdf", sep=""), width = 16, height = 8)
#par(mfrow = c(2, 5))
#for(i in 1:tables_num) {
#  plot_log2_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
#}

# Plot density(log2(nds1 / nds2)):
#pdf(paste("SC_", solver1, "_", solver2, "_density_log2_nds.pdf", sep=""), width = 16, height = 8)
#par(mfrow = c(2, 5))
#for(i in 1:tables_num) {
#  plot_density_log2_nds(SCtables[[i]], solver1, solver2, timelimit, make_sc_mask(i))
#}
