# Oliver Kullmann, 20.10.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Fitting the number of arithmetic progressions

# For the length k of arithmetic progressions and the maximal number N
# of primes considered, return a function f(n) which approximates
# the number of hyperedges in arithprog_greentao_ohg(k,n):
fit_greentao = function(k, N, monitor=FALSE) {
  fit_greentao_eval(fit_greentao_create(k, N, monitor), monitor)
}
# If using "monitor=TRUE", information on the initial linear regression and 
# the sequencing non-linear regression is printed.

# Helper function for fit_greentao, which creates the dataframe:
fit_greentao_create = function(k, N, monitor=FALSE) {
  filename = paste("GreenTaoStat_", k, "_", N, sep="")
  statistics_program = "CountProgressions_GreenTao-O3-DNDEBUG"
  command = paste(statistics_program, k, N, ">", filename)
  if (monitor) cat("Command: ", command, "\n")
  system(command)
  E = read.table(filename, colClasses=c("integer","integer"),header=TRUE)
  system(paste("rm", filename))
  E
}

# Helper function for fit_greentao, which evaluates the dataframe:
fit_greentao_eval = function(E, monitor=FALSE) {
  X = E$n
  Y = E$nhyp
  cat("Max nhyp = ", Y[length(Y)], "\n")

  x = log(X)
  y = log(Y)
  L = lm(y ~ x)
  if (monitor) {
    cat("\nThe linear model (log-log regression) nhyp = a0 * n^b0:\n")
    print(summary(L))
  }
  Cl = coefficients(L)
  a0 = exp(Cl[1])
  b0 = Cl[2]
  if (monitor) {
    f = function(n) { a0 * n^b0 }
    cat("a0 = ", a0, ", b0 = ", b0, "\n")
    cat("Residual range: ", range(f(E$n) - E$nhyp), "\n\n")
  }

  start_values = list(a = a0, b = b0)
  NL = nls(Y ~ a*(X^b), start = start_values)
  cat("Non-linear model nhyp = a * n^b:\n")
  if (monitor) print(summary(NL))
  Cnl = coefficients(NL)
  print(Cnl)
  f2 = function(n) { Cnl[1] * n^(Cnl[2]) }
  cat("Residual range: ", range(f2(E$n) - E$nhyp), "\n")
  return(f2)
}

