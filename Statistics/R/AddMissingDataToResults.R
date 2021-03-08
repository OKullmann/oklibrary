# Oleg Zaikin, 7.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######################################################
# Add missing data to file with solvers' results        #
# #######################################################

# Usage:
# AddMissingDataToResults.R oldFilename newFileName Version Options Weights Solver
#
# Example:
# Rscript ./AddMissingDataToResults.R ./mcc20_cttawSolver_2.8.2_3600 ./mcc20_cttawSolver_2.11.1_3600 "2.8.2" "TA19" "7,1,0.31,0.19,0.111765;1.7" "cttawSolver"

version = "0.1.1"

# Set wide terminal to see results with no line breaks:
options(width=400)
options(scipen=999)

args = commandArgs(trailingOnly = TRUE)
print(paste("AddMissingDataToResults, version=", version, sep=""))
print("Command line parameters :")
print(args)

if(length(args) < 6) {
  print(paste("Usage: script oldResult newResult oldVersion oldOptions oldWeights oldSolver"))
  quit("yes")
}

oldResult = args[1]
newResult = args[2]
oldVersion = args[3]
oldOptions = args[4]
oldWeights = args[5]
oldSolver = args[6]

print(paste("oldResult :", oldResult, sep=" "))
print(paste("newResult :", newResult, sep=" "))

E_old = read.table(oldResult, header=TRUE, sep=" ", row.names=NULL)
E_new = read.table(newResult, header=TRUE, sep=" ", row.names=NULL)

print("start merging")

E_merged = merge(x = E_new, y = E_old, by = "file")

E_merged$vers = paste("\"", oldVersion, "\"", sep="")
E_merged$opt = paste("\"\"", oldOptions, "\"\"", sep="")
E_merged$wghts = paste("\"", oldWeights, "\"", sep="")
E_merged$prog = paste("\"", oldSolver, "\"", sep="")

E_merged$file <- paste("\"", E_merged$file, "\"", sep="")

for(i in 1:nrow(E_merged)) {
  E_merged[i,]$t.x = E_merged[i,]$t.y
  E_merged[i,]$sat.x = E_merged[i,]$sat.y
  E_merged[i,]$nds.x = E_merged[i,]$nds.y
  E_merged[i,]$bnds.x = E_merged[i,]$bnds.y
  E_merged[i,]$r1.x = E_merged[i,]$r1.y
  E_merged[i,]$pls.x = E_merged[i,]$pls.y
  E_merged[i,]$ptime.x = E_merged[i,]$ptime.y
}

# Drop columns with old names and unusefule (by now) data:
drops <- c("n", "rn", "rc", "mcl.y", "ptime.y", "nsol",
           "t.y", "sat.y", "nds.y", "bnds.y", "r1.y", "pls.y")
E_merged = E_merged[ , !(names(E_merged) %in% drops)]

names(E_merged)[names(E_merged) == "mcl.x"] = "mcl"
names(E_merged)[names(E_merged) == "t.x"] = "t"
names(E_merged)[names(E_merged) == "sat.x"] = "sat"
names(E_merged)[names(E_merged) == "nds.x"] = "nds"
names(E_merged)[names(E_merged) == "bnds.x"] = "bnds"
names(E_merged)[names(E_merged) == "r1.x"] = "r1"
names(E_merged)[names(E_merged) == "pls.x"] = "pls"
names(E_merged)[names(E_merged) == "ptime.x"] = "ptime"

# set correct column order:
E_merged = E_merged[c("maxn", "c", "l", "mcl", "t", "sat", "nds", "bnds",
                      "r1", "pls", "sol", "ptime", "pn", "pc", "taut",
                      "file", "opt", "prog", "vers", "wghts")]

print(E_merged[1:3,])

write.table(E_merged, file = oldResult, append = FALSE, quote = FALSE,
            sep = " ", eol = "\n", na = "NA", dec = ".", row.names = FALSE,
            col.names = TRUE, qmethod = c("escape", "double"),
            fileEncoding = "")
