# Oleg Zaikin, 16.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######################################################
# Analyse and compare solvers' experimental results     #
# #######################################################

# The script is aimed at analysing SAT solvers' results on families of instances.
# It should be run in a directory, where files in the format scYY_SOLVER_TIMELIMIT
# are located (e.g., in the directory DataSolvers from the corresponding repository).
# Here YY stands for year, SOLVER stands for a solver name, e.g. taw or ttaw.
#
# For each family first a table of instances if created for each solver, then the
# tables are merged (inner joined by instance filename) to make a new united table.
# From this table those instances which were not solved by both solvers are removed.
# Then the difference between solvers' runtime and the number of nodes is calculated
# and added as new columns. These columns can be used to find families of instances
# where a certain solver perform well. If the united table in not empty, plots
# are drawn to compare the solvers via time, number of nodes, etc.

# Usage:
# AnalyseSolversResults.R families solver1 solver2 timelimit
#
# families - a file where families of instances are described.
# solver1 - the first solver to compare.
# solver2 - the second solver to compare.
# timelimit -  a runtime limit in seconds.

# Example:
# AnalyseSolversResults.R families tawSolver ttawSolver 1000

version = "0.4.3"

# Rename columns to see solvers' names:
rename_columns <- function(E, solver1, solver2) {
  names(E)[names(E) == "t.x"] = paste("t_", solver1, sep="")
  names(E)[names(E) == "t.y"] = paste("t_", solver2, sep="")
  names(E)[names(E) == "sat.x"] = paste("sat_", solver1, sep="")
  names(E)[names(E) == "sat.y"] = paste("sat_", solver2, sep="")
  names(E)[names(E) == "nds.x"] = paste("nds_", solver1, sep="")
  names(E)[names(E) == "nds.y"] = paste("nds_", solver2, sep="")
	names(E)[names(E) == "r1.x"] = paste("r1_", solver1, sep="")
  names(E)[names(E) == "r1.y"] = paste("r1_", solver2, sep="")
  names(E)[names(E) == "nds_per_t.x"] = paste("nds_per_t_", solver1, sep="")
  names(E)[names(E) == "nds_per_t.y"] = paste("nds_per_t_", solver2, sep="")
  return(E)
}

# Fill table with results for  instances from a given family
get_solver_family_results <- function(file_label, familiy_mask, solver, timelimit) {
  results_filename = paste(file_label, solver, timelimit, sep = "_")
	#print(paste("reading results_filename:", results_filename, sep=" "))
  E = read.table(results_filename, header=TRUE, sep=" ", row.names=NULL, stringsAsFactors=FALSE)[ ,c('file', 'sat', 't', 'nds', 'r1')]
  reg_expr = glob2rx(familiy_mask)
  # Remove '\\' from the regex:'
  reg_expr = gsub("\\\\", "", reg_expr)
  # Get all matching rows:
  E = subset(E, grepl(reg_expr, file) )
  # Check if at least one row:
  if(nrow(E) == 0) {
    cat("Empty family. file_label", file_label, "; solver", solver, "; mask", familiy_mask,
		            "; reg expr", reg_expr, "\n", sep=" ")
    quit("yes")
  }
  return(E)
}

# Analyse solvers' results on instances from a family:
merge_solvers_results_on_family <- function(file_label, familiy_mask, solver1, solver2, timelimit) {
  E_solver1 = get_solver_family_results(file_label, familiy_mask, solver1, timelimit)
  E_solver2 = get_solver_family_results(file_label, familiy_mask, solver2, timelimit)
  if(nrow(E_solver1) != nrow(E_solver2)) {
    cat("Inconsistent number of instances. file_label", file_label, " ; mask", familiy_mask, "\n", sep=" ")
    quit("yes")
  }
  # Check if the family is not empty:
  if((nrow(E_solver1) == 0) || (nrow(E_solver2) == 0)) {
    cat("Empty family. file_label", file_label, " ; mask", familiy_mask, "\n", sep=" ")
    quit("yes")
  }
  # Merge tables:
  E_merged = merge(x = E_solver1, y = E_solver2, by = "file")
  # Add column with difference between solvers' runtimes:
  E_merged$dif_t = (E_merged$t.x - E_merged$t.y)
  # Add column with difference between solvers' nodes number:
  E_merged$dif_nds = (E_merged$nds.x - E_merged$nds.y)
  # Add column with difference between solvers' ucps number:
  E_merged$dif_r1 = (E_merged$r1.x - E_merged$r1.y)
  # Add column with nodes per second for solver1:
  E_merged$nds_per_t.x = (E_merged$nds.x / E_merged$t.x)
  # Add column with nodes per second for solver2:
  E_merged$nds_per_t.y = (E_merged$nds.y / E_merged$t.y)
  return(E_merged)
}

get_family_name <- function(label, mask) {
  return(paste(label, mask, sep="-"))
}

# Plot log2(nds1 / nds2):
plot_log2_nds <- function(E, solver1, solver2) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  L = log2(E[[col1]] / E[[col2]])
	ymax = max(L)
	if (ymax < 1) ymax = 1
	ymin = min(L)
	if (ymin > -1) ymin = -1
  plot(L, xlab="Instance index", ylab="log2", cex.lab=1.5, cex.main = 2,
       main = paste("log2(", solver1, " nodes / ", solver2, " nodes)", sep=""),
       xaxs="i", yaxs="i", ylim=c(ymin,ymax))
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  abline(h = 0, col="red", lwd=2)
}

# Plot log2(nds1 / nds2) :
plot_density_log2_nds <- function(E, solver1, solver2) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  D = density(log2(E[[col1]] / E[[col2]]))
  plot(D, xlab="Instance index", ylab="Density", cex.lab=1.5, cex.main = 2,
       main=paste("density(log2(", solver1, " nodes / ", solver2, " nodes))", sep=""),
       xaxs="i", yaxs="i")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  abline(v = 0, col="red", lwd=2)
}

# Plot scatter to compare solvers' runtimes:
plot_scatter_time <- function(E, solver1, solver2, timelimit) {
  col_name_mask = "t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,timelimit),
       ylim=c(0,timelimit), xlab=paste(solver1, "time", sep=" "), 
       ylab=paste(solver2, "time", sep=" "), cex.lab=1.5, cex.main = 2,
       axes = FALSE, xaxs="i", yaxs="i", main = "Time")
  axis(side=1, at=seq(0, timelimit, by=100), las=2)
  axis(side=2, at=seq(0, timelimit, by=100), las=2)
  grid (NULL, NULL, lty = 6, col = "cornsilk2")
  abline(0,1,col="red")
  abline(v = timelimit, col="red")
  abline(h = timelimit, col="red")
}

# Plot scatter to compare solvers' nds per time:
plot_scatter_nds_per_time <- function(E, solver1, solver2) {
  col_name_mask = "nds_per_t"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  E_col1_finite_max = max(E[[col1]][is.finite(E[[col1]])], na.rm = TRUE)
  E_col2_finite_max = max(E[[col2]][is.finite(E[[col2]])], na.rm = TRUE)
  maxnds = max(E_col1_finite_max, E_col2_finite_max)
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,maxnds), ylim=c(0,maxnds),
       xlab=paste(solver1, "nodes per time", sep=" "),
       ylab=paste(solver2, "nodes per time", sep=" "), cex.lab=1.5, cex.main = 2,
       xaxs="i", yaxs="i", main = "Nodes per time")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  abline(0,1,col="red")
}

# Plot scatter to compare solvers' number of nodes:
plot_scatter_nds <- function(E, solver1, solver2) {
  col_name_mask = "nds"
  col1 = paste(col_name_mask, solver1, sep="_")
  col2 = paste(col_name_mask, solver2, sep="_")
  maxnds = max(max(E[[col1]], na.rm = TRUE),max(E[[col2]], na.rm = TRUE))
  plot(x = E[[col1]], y = E[[col2]], xlim=c(0,maxnds), ylim=c(0,maxnds),
       xlab=paste(solver1, "nodes", sep=" "), ylab=paste(solver2, "nodes", sep=" "),
       cex.lab=1.5, cex.main = 2, xaxs="i", yaxs="i", main = "Number of nodes")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  abline(0,1,col="red")
}

plot_comparison_two_solvers <- function(E, file_label, family_mask, solver1, solver2, timelimit) {
  family_name = get_family_name(file_label, family_mask)
  pdf(paste(solver1, "_", solver2, "_", timelimit, "_", family_name,".pdf", sep=""), 
      width = 16, height = 8)
  par(mfrow = c(2, 3))

  # Plot scatter to compare runtime:
  plot_scatter_time(E, solver1, solver2, timelimit)
  # Plot scatter to compare number of nodes:
  plot_scatter_nds(E, solver1, solver2)
  # Plot scatter to compare number of nodes per time:
  plot_scatter_nds_per_time(E, solver1, solver2)
  # Plot log2(nds1 / nds2), where nd1 and nds2 are number of nodes for solver1 and solver2:
  plot_log2_nds(E, solver1, solver2)
  # Plot density(log2(nds1 / nds2)):
  if(nrow(E) > 1) { # At least 2 points are required:
    plot_density_log2_nds(E, solver1, solver2)
  }
}

calc_family_stats <- function(E, solver1, solver2, family_size) {
  col_sat1 = paste("sat_", solver1, sep="")
  col_sat2 = paste("sat_", solver2, sep="")
  col_r11 = paste("r1_", solver1, sep="")
  col_r12 = paste("r1_", solver2, sep="")
  col_nds1 = paste("nds_", solver1, sep="")
  col_nds2 = paste("nds_", solver2, sep="")
  col_t1 = paste("t_", solver1, sep="")
  col_t2 = paste("t_", solver2, sep="")
  cat("Instances solved by at least one solver:\n")
  E[[col_sat1]] = as.factor(E[[col_sat1]])
  E[[col_sat2]] = as.factor(E[[col_sat2]])
  print(E)
  cat("\n")
  print(summary(E))
  cat("\n")
  num_solved_either_solver = nrow(E)
  E1unsat = E[E[[col_sat1]] == 0,]
  E1sat = E[E[[col_sat1]] == 1,]
  E2unsat = E[E[[col_sat2]] == 0,]
  E2sat = E[E[[col_sat2]] == 1,]
  # Number of solved:
  num_unsat_solver1 = nrow(E1unsat)
  num_sat_solver1 = nrow(E1sat)
  num_solved_solver1 = num_unsat_solver1 + num_sat_solver1
  num_unsat_solver2 = nrow(E2unsat)
  num_sat_solver2 = nrow(E2sat)
  num_solved_solver2 = num_unsat_solver2 + num_sat_solver2
  num_unsat_both_solvers = nrow(E[(E[[col_sat1]] == 0) & (E[[col_sat2]] == 0),])
  num_sat_both_solvers = nrow(E[(E[[col_sat1]] == 1) & (E[[col_sat2]] == 1),])
  num_solved_both_solvers = num_unsat_both_solvers + num_sat_both_solvers
  # Mean:
  mean_r1_unsat_solver1 = mean(E2unsat[[col_r11]])
  mean_nds_unsat_solver1 = mean(E2unsat[[col_nds1]])
  mean_t_unsat_solver1 = mean(E2unsat[[col_t1]])
  mean_r1_sat_solver1 = mean(E1sat[[col_r11]])
  mean_nds_sat_solver1 = mean(E1sat[[col_nds1]])
  mean_t_sat_solver1 = mean(E1sat[[col_t1]])
  mean_r1_unsat_solver2 = mean(E2unsat[[col_r12]])
  mean_nds_unsat_solver2 = mean(E2unsat[[col_nds2]])
  mean_t_unsat_solver2 = mean(E2unsat[[col_t2]])
  mean_r1_sat_solver2 = mean(E2sat[[col_r12]])
  mean_nds_sat_solver2 = mean(E2sat[[col_nds2]])
  mean_t_sat_solver2 = mean(E2sat[[col_t2]])
  # Print:
  cat("*** family stats:\n")
  cat(" family size:", family_size, "\n", sep=" ")
  cat(" solved by either solver: ", num_solved_either_solver, "\n", sep="")
  cat(" solved by both solvers : ", num_solved_both_solvers, "\n", sep="")
  cat(" unsat by both solvers  : ", num_unsat_both_solvers, "\n", sep="")
  cat(" sat by both solvers: ", num_sat_both_solvers, "\n", sep="")
  cat("*** ", solver1, ":", "\n", sep=" ")
  cat(" solved:", num_solved_solver1, "\n", sep=" ")
  cat(" unsat:", num_unsat_solver1, "\n", sep=" ")
  cat(" sat:", num_sat_solver1, "\n", sep=" ")
  cat(" mean r1 unsat :", mean_r1_unsat_solver1, "\n", sep=" ")
  cat(" mean nds unsat:", mean_nds_unsat_solver1, "\n", sep=" ")
  cat(" mean t unsat  :", mean_t_unsat_solver1, "\n", sep=" ")
  cat(" mean r1 sat   :", mean_r1_sat_solver1, "\n", sep=" ")
  cat(" mean nds sat  :", mean_nds_sat_solver1, "\n", sep=" ")
  cat(" mean t sat    :", mean_t_sat_solver1, "\n", sep=" ")
  cat("*** ", solver2, ":", "\n", sep=" ")
  cat(" solved:", num_solved_solver2, "\n", sep=" ")
  cat(" unsat: ", num_unsat_solver2, "\n", sep=" ")
  cat(" sat: ", num_sat_solver2, "\n", sep="")
  cat(" mean r1 unsat :", mean_r1_unsat_solver2, "\n", sep=" ")
  cat(" mean nds unsat:", mean_nds_unsat_solver2, "\n", sep=" ")
  cat(" mean t unsat  :", mean_t_unsat_solver2, "\n", sep=" ")
  cat(" mean r1 sat   :", mean_r1_sat_solver2, "\n", sep=" ")
  cat(" mean nds sat  :", mean_nds_sat_solver2, "\n", sep=" ")
  cat(" mean t sat    :", mean_t_sat_solver2, "\n", sep=" ")
  cat("\n")
  solvedNum <- list("num_solved_solver1" = num_solved_solver1,
                    "num_unsat_solver1" = num_unsat_solver1,
                    "num_sat_solver1" = num_sat_solver1,
                    "num_solved_solver2" = num_solved_solver2,
                    "num_unsat_solver2" = num_unsat_solver2,
                    "num_sat_solver2" = num_sat_solver2,
                    "mean_r1_unsat_solver1" = mean_r1_unsat_solver1,
                    "mean_nds_unsat_solver1" = mean_nds_unsat_solver1,
                    "mean_t_unsat_solver1" = mean_t_unsat_solver1,
                    "mean_r1_sat_solver1" = mean_r1_sat_solver1,
                    "mean_nds_sat_solver1" = mean_nds_sat_solver1,
                    "mean_t_sat_solver1" = mean_t_sat_solver1,
                    "mean_r1_unsat_solver2" = mean_r1_unsat_solver2,
                    "mean_nds_unsat_solver2" = mean_nds_unsat_solver2,
                    "mean_t_unsat_solver2" = mean_t_unsat_solver2,
                    "mean_r1_sat_solver2" = mean_r1_sat_solver2,
                    "mean_nds_sat_solver2" = mean_nds_sat_solver2,
                    "mean_t_sat_solver2" = mean_t_sat_solver2)
  return(solvedNum)
}

# Set wide terminal to see results with no line breaks:
options(width=300)
#options(scipen=999)

args = commandArgs(trailingOnly = TRUE)
cat("AnalyseSolversResults, version=", version, "\n", sep="")
cat("Command line parameters :\n")
print(args)

if(length(args) < 4) {
  cat("Usage: script families solver1 solver2 timelimit\n")
  quit("yes")
}

families = args[1]
solver1 = args[2]
solver2 = args[3]
timelimit = strtoi(args[4])

solved_families = vector()
solved_families_better_r1_nds_solver2 = vector()
solved_families_better_t_solver2 = vector()

families_table = read.table(families, header=TRUE, stringsAsFactors=FALSE)
families_num = nrow(families_table)
cat("total number of families:", families_num, "\n\n", sep=" ")
eq_or_gr_solved_solver2 = 0
for(i in 1:families_num) {
  E_merged = merge_solvers_results_on_family(families_table[i,]$label, families_table[i,]$mask, solver1, solver2, timelimit)
  # Find subtable of the merged table where at least one solver coped:
  E_merged_solved = E_merged[E_merged$sat.x + E_merged$sat.y != 4,]
  E_merged_solved = rename_columns(E_merged_solved, solver1, solver2)
  if(nrow(E_merged_solved) > 0) {
    print(families_table[i,]$mask)
    solvedNum = calc_family_stats(E_merged_solved, solver1, solver2, nrow(E_merged))
    plot_comparison_two_solvers(E_merged_solved, families_table[i,]$label, families_table[i,]$mask, solver1, solver2, timelimit)
    family_name = get_family_name(families_table[i,]$label, families_table[i,]$mask)
    solved_families = append(solved_families, family_name)
    if(solvedNum$num_unsat_solver2 >= solvedNum$num_unsat_solver1) {
      eq_or_gr_solved_solver2 = eq_or_gr_solved_solver2 + 1
      if(is.na(solvedNum$mean_r1_unsat_solver2))
        next
      if((is.na(solvedNum$mean_r1_unsat_solver1)) |
         (solvedNum$mean_r1_unsat_solver2 < solvedNum$mean_r1_unsat_solver1) &
         (solvedNum$mean_nds_unsat_solver2 < solvedNum$mean_nds_unsat_solver1)
        ) {
            solved_families_better_r1_nds_solver2 = append(solved_families_better_r1_nds_solver2, family_name)
            if(solvedNum$mean_t_unsat_solver2 < solvedNum$mean_t_unsat_solver1) {
              solved_families_better_t_solver2 = append(solved_families_better_t_solver2, family_name)
            }
          }
    }
  }
}

cat("\n\n")
cat("***", length(solved_families), "families with at least one solved instance out of total", families_num, "families:", "\n", sep=" ")
for(i in 1:length(solved_families)){
  print(solved_families[i])
}
cat("\n\n")
cat("***", eq_or_gr_solved_solver2, "families where solver", solver2, "solved >= unsat instances than solver", solver1, "\n", sep=" ")
cat("***", length(solved_families_better_r1_nds_solver2), "families where on solver", solver2, " mean r1 and nds are lower \n", sep=" ")
for(i in 1:length(solved_families_better_r1_nds_solver2)){
  print(solved_families_better_r1_nds_solver2[i])
}
cat("\n")
cat("***", length(solved_families_better_t_solver2), "families where on solver", solver2, " mean t is lower \n", sep=" ")
for(i in 1:length(solved_families_better_t_solver2)){
  print(solved_families_better_t_solver2[i])
}
