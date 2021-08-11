# Oleg Zaikin, 9.8.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Calculate probabilities of an orthogonal mate existance for a random Euler square
# completion problem.

# Usage:
# CalculateProbabilities.R solver_results

# Example:
# CalculateProbabilities.R Results_N5

version = "0.0.3"

args = commandArgs(trailingOnly = TRUE)
cat("CalculateProbabilities, version=", version, "\n", sep="")

if(length(args) > 0) {
    cat("Command line parameters :\n")
    print(args)
}

if(length(args) < 1) {
  cat("Usage: script solver_results\n")
  quit("yes")
}

solver_results = args[1]

E = read.table(solver_results, header=TRUE, stringsAsFactors=FALSE, sep=" ")

cat("total rows :", nrow(E), "\n\n", sep=" ")

N = min(E$N)
cat("N :", N, "\n", sep=" ")

minm = min(E$m)
maxm = max(E$m)
cat("minimal m :", minm, "\n", sep=" ")
cat("maximal m :", maxm, "\n", sep=" ")
cat("\n")

probabilites = vector()
mvalues = vector()

for(m in minm:maxm) {
    mvalues = append(mvalues, m)
    Em = E[E$m == m,]
    solver_runs = nrow(Em)
    if (solver_runs == 0) {
        cat("0 solver runs for m ", m, "\n", sep="")
        quit("yes")
    }
    Em_sat = Em[Em$sat == 1,]
    solutions = nrow(Em_sat)
    addit_symbol = ""
    if (m < 10) {
        addit_symbol = "0";
    }
    prob = solutions / solver_runs
    probabilites = append(probabilites, prob)
    cat("m : ", addit_symbol, m, "  solver_runs : ", solver_runs, "  solutions: ", solutions, "  prob : ", prob, "\n", sep="")
}

cat("\n")
cat("m prob", "\n", sep="")
for(i in 1:length(mvalues)) {
    cat(mvalues[i], " ", probabilites[i], "\n", sep="")
}
minprob = min(probabilites)

library(ggplot2)
title = paste("N=", N, sep="")
# xlim=c(minm,maxm), ylim=c(0,1),
qplot(x=mvalues, y=probabilites, xlab="m", ylab="probability", main=title) +
scale_y_continuous(breaks=seq(0, 1, 0.1)) + scale_x_continuous(breaks=seq(minm, maxm, 1)) +
theme_linedraw()
