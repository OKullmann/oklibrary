# Oliver Kullmann, 20.10.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Fitting the number of arithmetic progressions

# For the length k of arithmetic progressions and the maximal number n
# of primes considered, return a function f(n) which approximates
# the number of hyperedges in arithprog_greentao_hg(k,n):
# Prerequisite: N >= 1.
fit_greentao = function(k, n, N=3, monitor=FALSE) {
  fit_greentao_eval(fit_greentao_create(k, n, monitor), k, N, monitor)
}
# If using "monitor=TRUE", information on the linear regression is printed.

# As model we use the asymptotic formula (7) from [Grosswald, Hagis, 1979,
# Arithmetic Progressions Consisting Only of Primes], where x is replaced
# by n*log(n), using that according to the Prime Number theorem asymptotically
# the n-th prime number is n*log(n). The formula (7) for the number
# of arithmetic progresssions of length k with members up to x, but consisting
# only of prime numbers, then yields:
# C_k/(2*(k-1)) * n^2/(log(n))^(k-2) * (1 + sum_{i=1}^N a_i/(log(n))^i
# This is proven now for k<=4, while in general it follows from the
# Hardy-Littlewood m-tuples conjecture, that for all N>=0 this formula is
# asymptotically correct (the quotient with the correct value goes to 1
# with n going to infinity).
# In the paper an explicit formula for C_k is given, and implicit formulas
# for the a_i (also depending on k). For C_k we use the numerical values
# in GH_coeff, while for the a_i we use linear regression; additionally we
# use also an absolute additive constant.

# The coefficients from [Grosswald, Hagis, 1979], beginning with k=2:
GH_coeff = c(1, 1.320323632, 2.8582486, 4.1511809, 10.1317950, 17.2986123, 53.9719484, 148.551629, 336.034327,
 511.422283, 1312.31971, 2364.59897, 7820.60006, 22938.9087, 55651.463, 91555.112, 256474.861, 510992.01, 1900972.6
)

# Helper function for fit_greentao, which creates the dataframe:
fit_greentao_create = function(k, n, monitor=FALSE) {
  N_as_string = format(n,scientific=FALSE)
  filename = paste("GreenTaoStat_", k, "_", N_as_string, sep="")
  statistics_program = "CountProgressions_GreenTao-O3-DNDEBUG"
  command = paste(statistics_program, k, N_as_string, ">", filename)
  if (monitor) cat("Command: ", command, "\n")
  system(command)
  E = read.table(filename, colClasses=c("integer","integer"),header=TRUE)
  system(paste("rm", filename))
  E
}

# Helper function for fit_greentao, which evaluates the dataframe:
fit_greentao_eval = function(E, k, N, monitor=FALSE) {
  X = E$n
  Y = E$nhyp
  cat("Number of observations (changes) = ", length(X), "\n")
  cat("Max nhyp = ", Y[length(Y)], "\n")

  Dk = GH_coeff[k-1]/2/(k-1)
  X0 = Dk*X^2/(log(X))^(k-2)
  A = array(dim=c(N,length(X)))
  for (i in 1:N)
    if (i==1) A[i,] = X0/log(X)
    else A[i,] = A[i-1,]/log(X)
  YD = Y - X0
  if (N == 1)
    GHL = lm(YD ~ A[1,])
  else if (N == 2)
    GHL = lm(YD ~ A[1,] + A[2,])
  else if (N == 3)
    GHL = lm(YD ~ A[1,] + A[2,] + A[3,])
  else if (N == 4)
    GHL = lm(YD ~ A[1,] + A[2,] + A[3,] + A[4,])
  else return()

  if (monitor) {
    cat("\nThe pure Grosswald-Hagis model:")
    print(summary(GHL))
  }

  Chl = coefficients(GHL)
  c = Chl[1]
  a = Chl[-1]

  f = function(n) {
   x0 = Dk*n^2/(log(n))^(k-2)
   b = array(dim=c(N,length(n)))
   for (i in 1:N)
    if (i==1) b[i,] = x0/log(n)
    else b[i,] = b[i-1,]/log(n)
   c + x0 + rowSums(t(b) %*% a)
  }
  cat("Coefficients:", c, a, "; ", Dk, "\n")
  cat("Residual range:", range(E$nhyp - f(E$n)), "\n")
  plot(E$n,E$nhyp)
  lines(E$n,f(E$n),col="red")
  return(f)
}

