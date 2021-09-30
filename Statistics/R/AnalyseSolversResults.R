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
# For each family first a table of instances is created for each solver, then the
# tables are merged (inner joined by instance filename) to make a new united table.
# From this table those instances which were not solved by both solvers are removed.
# Then the difference between solvers' runtime and the number of nodes is calculated
# and added as new columns. These columns can be used to find families of instances
# where a certain solver perform well. If the united table in not empty, plots
# are drawn to compare the solvers via time, number of nodes, etc.

# Usage:
# AnalyseSolversResults.R families solver1 solver2 timelimit
#
# families  - a file where families of instances are described.
# solver1   - the first solver to compare.
# solver2   - the second solver to compare.
# timelimit -  a runtime limit in seconds.

# Example:
# AnalyseSolversResults.R families tawSolver_2.20.1 ttawSolver_2.20.1 1000

version = "0.5.8"

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
  E$sat = as.factor(E$sat)
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

merge_solvers_results <- function(E_solver1, E_solver2) {
  if(nrow(E_solver1) != nrow(E_solver2)) {
    cat("Inconsistent number of instances. file_label", file_label, " ; mask", familiy_mask, "\n", sep=" ")
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
       main = "log2(nodes1 / nodes2)", xaxs="i", yaxs="i", ylim=c(ymin,ymax))
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
       main="density( log2(nodes1 / nodes2) )", xaxs="i", yaxs="i")
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
       xlab=paste(solver1, "nodes (nodes1)", sep=" "),
       ylab=paste(solver2, "nodes (nodes2)", sep=" "),
       cex.lab=1.5, cex.main = 2, xaxs="i", yaxs="i", main = "Number of nodes")
  grid(NULL, NULL, lty = 6, col = "cornsilk2")
  abline(0,1,col="red")
}

plot_comparison_two_solvers <- function(E, family_name, solver1, solver2, timelimit) {
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
  dev.off()
}

# 0 - default
# 1 - solver2 solved >= instances, but mean on r1, nds, and t are greater
# 2 - solver2 solved >= instances, mean r1 and nds are lower, mean time is higher
# 3 - solver2 solved >= instances, mean r1, nds, t are lower
get_family_class <- function(E1, E2, col_r11, col_r12, col_nds1, col_nds2, col_t1, col_t2) {
  family_class = 0
  if(nrow(E2) > 0) {
    if( nrow(E2) >= nrow(E1) ) {
      if(nrow(E1) == 0) {
        family_class = 3
      }
      else {
        family_class = 1
        if( (mean(E2[[col_r12]]) < mean(E1[[col_r11]])) &
            (mean(E2[[col_nds2]]) < mean(E1[[col_nds1]])) )
        {
          family_class = 2
          if( mean(E2[[col_t2]]) < mean(E1[[col_t1]]) ) {
            family_class = 3
          }
        }
      }
    }
  }
  return(family_class)
}

family_stats <- function(family_name, family_size, E, solver1, solver2) {
  col_sat1 = paste("sat_", solver1, sep="")
  col_sat2 = paste("sat_", solver2, sep="")
  col_r11 = paste("r1_", solver1, sep="")
  col_r12 = paste("r1_", solver2, sep="")
  col_nds1 = paste("nds_", solver1, sep="")
  col_nds2 = paste("nds_", solver2, sep="")
  col_t1 = paste("t_", solver1, sep="")
  col_t2 = paste("t_", solver2, sep="")
  cat("*** family ", family_name, " stats:\n")
  print(E)
  cat(" family size:", family_size, "\n", sep=" ")
  cat(" solved by either solver: ", nrow(E), "\n", sep="")
  cat(" solved by both solvers : ", nrow(E[(E[[col_sat1]] != 2) & (E[[col_sat2]] != 2),]), "\n", sep="")
  cat(" unsat by both solvers  : ", nrow(E[(E[[col_sat1]] == 0) & (E[[col_sat2]] == 0),]), "\n", sep="")
  cat(" sat by both solvers: ", nrow(E[(E[[col_sat1]] == 1) & (E[[col_sat2]] == 1),]), "\n", sep="")
  # Divide tables by sat status:
  E1unsat = E[E[[col_sat1]] == 0,]
  E1unsat = E1unsat[ , ! names(E1unsat) %in% c(col_sat2, col_r12, col_nds2, col_t2)]
  E1sat = E[E[[col_sat1]] == 1,]
  E1sat = E1sat[ , ! names(E1sat) %in% c(col_sat2, col_r12, col_nds2, col_t2)]
  E1unsolved = E[E[[col_sat1]] == 2,]
  E1unsolved = E1unsolved[ , ! names(E1unsolved) %in% c(col_sat2, col_r12, col_nds2, col_t2)]
  E2unsat = E[E[[col_sat2]] == 0,]
  E2unsat = E2unsat[ , ! names(E2unsat) %in% c(col_sat1, col_r11, col_nds1, col_t1)]
  E2sat = E[E[[col_sat2]] == 1,]
  E2sat = E2sat[ , ! names(E2sat) %in% c(col_sat1, col_r11, col_nds1, col_t1)]
  E2unsolved = E[E[[col_sat2]] == 2,]
  E2unsolved = E2unsolved[ , ! names(E2unsolved) %in% c(col_sat1, col_r11, col_nds1, col_t1)]
  # Print stats for every solver:
  cat("\n")
  cat("*** ", solver1, ":", "\n", sep=" ")
  if(nrow(E1unsat) > 0) {
    cat("UNSAT:\n", sep="")
    print(summary(E1unsat))
  }
  if(nrow(E1sat) > 0) {
    cat("SAT:\n", sep="")
    print(summary(E1sat))
  }
  if(nrow(E1unsolved) > 0) {
    cat("UNSOLVED:\n", sep="")
    print(summary(E1unsolved))
  }
  cat("\n")
  cat("*** ", solver2, ":", "\n", sep=" ")
  if(nrow(E2unsat) > 0) {
    cat("UNSAT:\n", sep="")
    print(summary(E2unsat))
  }
  if(nrow(E2sat) > 0) {
    cat("SAT:\n", sep="")
    print(summary(E2sat))
  }
  if(nrow(E2unsolved) > 0) {
    cat("UNSOLVED:\n", sep="")
    print(summary(E2unsolved))
  }

  unsat_class = 0
  sat_class = 0

  unsat_family_class = get_family_class(E1unsat, E2unsat,
                        col_r11, col_r12, col_nds1, col_nds2, col_t1, col_t2)
  sat_family_class = get_family_class(E1sat, E2sat,
                        col_r11, col_r12, col_nds1, col_nds2, col_t1, col_t2)
  family_classes <- list("unsat" = unsat_family_class, "sat" = sat_family_class)

  return(family_classes)
}

all_instances <- function(file_labels, solver, timelimit) {
  file_labels_num = length(file_labels)
  filenames = vector()
  for(i in 1:file_labels_num) {
    filenames = append(filenames, paste(file_labels[i], solver, timelimit, sep = "_"))
  }
  print(filenames)
  tbls = lapply(filenames, function(x){
    read.table(x, header=TRUE, sep=" ", row.names=NULL, stringsAsFactors=FALSE)[ ,c('file', 'sat', 't', 'nds', 'r1')]})
  E = do.call(rbind, tbls)
  E$sat = as.factor(E$sat)
  return(E)
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
unsat_solved_families_better_r1_nds_solver2 = vector()
unsat_solved_families_better_t_solver2 = vector()
sat_solved_families_better_r1_nds_solver2 = vector()
sat_solved_families_better_t_solver2 = vector()

families_table = read.table(families, header=TRUE, stringsAsFactors=FALSE)
families_num = nrow(families_table)
cat("total number of families:", families_num, "\n\n", sep=" ")
eq_or_gr_unsat_solver2 = 0
eq_or_gr_sat_solver2 = 0
file_labels = vector()
for(i in 1:families_num) {
  file_labels = append(file_labels, families_table[i,]$label)
  E_merged = merge_solvers_results_on_family(families_table[i,]$label, families_table[i,]$mask, solver1, solver2, timelimit)
  # Find subtable of the merged table where at least one solver coped:
  E_merged_solved = E_merged[(E_merged$sat.x != 2) & (E_merged$sat.y != 2),]
  E_merged_solved = rename_columns(E_merged_solved, solver1, solver2)
  if(nrow(E_merged_solved) > 0) {
    family_name = get_family_name(families_table[i,]$label, families_table[i,]$mask)
    solved_families = append(solved_families, family_name)
    if(nrow(E_merged_solved) > 4) {
      plot_comparison_two_solvers(E_merged_solved, family_name, solver1, solver2, timelimit)
    }
    family_classes = family_stats(family_name, nrow(E_merged), E_merged_solved, solver1, solver2)
    # If family is interesting from solver2 point of view:
    if(family_classes$unsat > 0) {
      eq_or_gr_unsat_solver2 = eq_or_gr_unsat_solver2 + 1
      if(family_classes$unsat > 1) {
        unsat_solved_families_better_r1_nds_solver2 = append(unsat_solved_families_better_r1_nds_solver2, family_name)
      }
      if(family_classes$unsat == 3) {
        unsat_solved_families_better_t_solver2 = append(unsat_solved_families_better_t_solver2, family_name)
      }
    }
    if(family_classes$sat > 0) {
      eq_or_gr_sat_solver2 = eq_or_gr_sat_solver2 + 1
      if(family_classes$sat > 1) {
        sat_solved_families_better_r1_nds_solver2 = append(sat_solved_families_better_r1_nds_solver2, family_name)
      }
      if(family_classes$sat == 3) {
        sat_solved_families_better_t_solver2 = append(sat_solved_families_better_t_solver2, family_name)
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
cat("***", eq_or_gr_unsat_solver2, "families where solver", solver2, "solved >= unsat instances than solver", solver1, "\n", sep=" ")
cat("  ** out of them", length(unsat_solved_families_better_r1_nds_solver2), "families where on solver", solver2, "mean r1 and nds are lower \n", sep=" ")
for(i in 1:length(unsat_solved_families_better_r1_nds_solver2)){
  print(unsat_solved_families_better_r1_nds_solver2[i])
}
cat("    * out of them", length(unsat_solved_families_better_t_solver2), "families where on solver", solver2, "mean t is lower \n", sep=" ")
for(i in 1:length(unsat_solved_families_better_t_solver2)){
  print(unsat_solved_families_better_t_solver2[i])
}

cat("\n\n")
cat("***", eq_or_gr_sat_solver2, "families where solver", solver2, "solved >= sat instances than solver", solver1, "\n", sep=" ")
cat("  ** out of them", length(sat_solved_families_better_r1_nds_solver2), "families where on solver", solver2, "mean r1 and nds are lower \n", sep=" ")
for(i in 1:length(sat_solved_families_better_r1_nds_solver2)){
  print(sat_solved_families_better_r1_nds_solver2[i])
}
cat("    * out of them", length(sat_solved_families_better_t_solver2), "families where on solver", solver2, "mean t is lower \n", sep=" ")
for(i in 1:length(sat_solved_families_better_t_solver2)){
  print(sat_solved_families_better_t_solver2[i])
}

file_labels = unique(file_labels)
cat("\nfile labels:")
print(file_labels)
E1 = all_instances(file_labels, solver1, timelimit)
cat("Total instances from solver1 : ", nrow(E1), "\n", sep="")
E2 = all_instances(file_labels, solver2, timelimit)
cat("Total instances from solver2 : ", nrow(E2), "\n", sep="")
E_merged = merge_solvers_results(E1, E2)
E_merged = E_merged[(E_merged$sat.x != 2) & (E_merged$sat.y != 2),]
E_merged = rename_columns(E_merged, solver1, solver2)
cat("Total instances in merged table : ", nrow(E_merged), "\n", sep="")
plot_comparison_two_solvers(E_merged , "ALL_sc11-20", solver1, solver2, timelimit)

col_sat1 = paste("sat_", solver1, sep="")
col_sat2 = paste("sat_", solver2, sep="")
E_merged_unsat = E_merged[(E_merged[[col_sat1]] == 0) & (E_merged[[col_sat2]] == 0),]
#print(E_merged_unsat)
plot_comparison_two_solvers(E_merged_unsat, "UNSAT_sc11-20", solver1, solver2, timelimit)

E_merged_unsat_random = E_merged_unsat[which(grepl("unif-", E_merged_unsat$file) | grepl("fla-", E_merged_unsat$file)),]
print(E_merged_unsat_random)
plot_comparison_two_solvers(E_merged_unsat_random, "UNSAT_random_sc11-20", solver1, solver2, timelimit)

E_merged_unsat_nonrandom = E_merged_unsat[-which(grepl("unif-", E_merged_unsat$file) | grepl("fla-", E_merged_unsat$file)),]
print(E_merged_unsat_nonrandom)
plot_comparison_two_solvers(E_merged_unsat_nonrandom, "UNSAT_nonrandom_sc11-20", solver1, solver2, timelimit)

E_merged_sat = E_merged[(E_merged[[col_sat1]] == 1) & (E_merged[[col_sat2]] == 1),]
#print(E_merged_sat)
plot_comparison_two_solvers(E_merged_sat, "SAT_sc11-20", solver1, solver2, timelimit)

E_merged_sat_random = E_merged_sat[which(grepl("unif-", E_merged_sat$file) | grepl("fla-", E_merged_sat$file)),]
print(E_merged_sat_random)
plot_comparison_two_solvers(E_merged_sat_random, "SAT_random_sc11-20", solver1, solver2, timelimit)

E_merged_sat_nonrandom = E_merged_sat[-which(grepl("unif-", E_merged_sat$file) | grepl("fla-", E_merged_sat$file)),]
print(E_merged_sat_nonrandom)
plot_comparison_two_solvers(E_merged_sat_nonrandom, "SAT_nonrandom_sc11-20", solver1, solver2, timelimit)

cat("\n")

cat("Total unsatisfiable instances solved by both solvers : ", nrow(E_merged_unsat), "\n", sep="")
cat("Total random unsatisfiable instances solved by both solvers : ", nrow(E_merged_unsat_random), "\n", sep="")
cat("Total non-random unsatisfiable instances solved by both solvers : ", nrow(E_merged_unsat_nonrandom), "\n", sep="")
col_nds1 = paste("nds_", solver1, sep="")
col_nds2 = paste("nds_", solver2, sep="")
E_merged_unsat_better_solver2 = E_merged_unsat[E_merged_unsat[[col_nds2]] < E_merged_unsat[[col_nds1]],]
E_merged_unsat_random_better_solver2 = E_merged_unsat_random[E_merged_unsat_random[[col_nds2]] < E_merged_unsat_random[[col_nds1]],]
E_merged_unsat_nonrandom_better_solver2 = E_merged_unsat_nonrandom[E_merged_unsat_nonrandom[[col_nds2]] < E_merged_unsat_nonrandom[[col_nds1]],]
cat(nrow(E_merged_unsat_better_solver2), " on all unsat where solver2 is node-wise better.", "\n", sep="")
cat(nrow(E_merged_unsat_random_better_solver2), " on random unsat where solver2 is node-wise better.", "\n", sep="")
cat(nrow(E_merged_unsat_nonrandom_better_solver2), " on non-random unsat where solver2 is node-wise better.", "\n", sep="")

cat("\n")
cat("Total satisfiable instances solved by both solvers : ", nrow(E_merged_sat), "\n", sep="")
cat("Total random satisfiable instances solved by both solvers : ", nrow(E_merged_sat_random), "\n", sep="")
cat("Total non-random satisfiable instances solved by both solvers : ", nrow(E_merged_sat_nonrandom), "\n", sep="")
E_merged_sat_better_solver2 = E_merged_sat[E_merged_sat[[col_nds2]] < E_merged_sat[[col_nds1]],]
E_merged_sat_random_better_solver2 = E_merged_sat_random[E_merged_sat_random[[col_nds2]] < E_merged_sat_random[[col_nds1]],]
E_merged_sat_nonrandom_better_solver2 = E_merged_sat_nonrandom[E_merged_sat_nonrandom[[col_nds2]] < E_merged_sat_nonrandom[[col_nds1]],]
cat(nrow(E_merged_sat_better_solver2), " on all sat where solver2 is node-wise better.", "\n", sep="")
cat(nrow(E_merged_sat_random_better_solver2), " on random sat where solver2 is node-wise better.", "\n", sep="")
cat(nrow(E_merged_sat_nonrandom_better_solver2), " on non-random sat where solver2 is node-wise better.", "\n", sep="")
