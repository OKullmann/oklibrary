# Oliver Kullmann, 11.8.2002 (Swansea)
# Copyright 2002-2009, 2011, 2016 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


# Find element in vector:
find.e <- function(e, vec) {
  match(e, vec);
}

# Select element of vectors which are in a list of values:
select.v <- function(vec, val) {
  vec[vec %in% val]
}

# Replace substrings:
replace.substrings <- function(x, a, b) {
  gsub(a,b,x)
}
# single substitutions with sub(a,b,x)


# Radius of the f%-confidence interval with center p for the observed
# proportion p using N observations.
# Usage: radiusconfprop(f, p, N) for
# 0 <= f <= 100, 0 <= p <= 1, N >= 4
# f, p, N can be vectors (if the lengths are not the same, then the values
# are recycled to fill up according to the maximal length)
# Example: radiusconfprop(95, 0.413, 20000) = 0.006824397
radiusconfprop <- function(f, p, N) {
  return(qt(1 - 1/2 * (1 - f/100), N-1) * sqrt(p * (1-p) / (N-1)))
}

# f%-confidence interval for the observed proportion p using N observations
# based on the normal distribution
confpropnormal <- function(f, p, N) {
  r <- radiusconfprop(f, p, N);
  return(c(p - r, p + r))
}
# f%-confidence interval for the observed proportion p using N observations
# based on the binomial distribution
confprop <- function(f, p, N) {
  return(as.vector(binom.test(round(p * N), N, p=0, conf = f/100)$conf.int))
}

# p-value for the observed number suc of success using N observations, where the exact propability is p
# (0 < p-value <= 1; if for example the p-value is less than 0.05, then with significance level 5% the observations are inconsistent with the theory) (the smaller the significance level is chosen, the stronger is the case if the theoretical value is rejected; thus we seek for p-value as large as possible, and typically a p-value smaller than 0.05 means closer investigations are necessary, i.e., either the experimental or the theoretical probability might be doubtfull)
# The problem with looking only at the p-value is, that the "quality" of the experiment (which here is just given by N) is ignored. This quality is better reflected in the size of the confidence interval computed with confprop, but having one "quality index" would be better ???
# (Alternatively, prop.test uses the normal approximation.)
pvalprop = function(suc, N, p) {
  return(binom.test(suc,N,p)$p.value)
}

# Append definition of tex-macro to file
definetexmacro <- function(val, macro, file) {
  out = paste("\\providecommand{\\", macro, "}{", val, "}", sep="");
  write(noquote(out), file = file, ncolumns = 1, append = TRUE)
}

# In a string s representing a number, add surrounding $'s, and replace "e" by a tex-representation:
texexponent = function(s) {
  if (regexpr("e", s) != -1) { # s contains e
    return(paste("$", sub("e","\\\\cdot 10^{",s), "}", "$", sep=""))
  }
  else {
    return(paste("$", s, "$"))
  }
}

# Formatting small numbers:
smallastex = function(val, dig=-1) {
  if (val < 2*10e-16) {
    return("$<2 \\cdot 10^{-16}$")
  }
  else {
    if (dig == -1) {
      return(texexponent(val))
    }
    else {
      return(texexponent(paste(signif(val, dig))))
    }
  }
}

# Formatting long natural numbers:
longintegers = function(n) {
  return(formatC(n,format="f",big.mark=",",digits=0))
}


# The residual standard error for objects returned by summary(nlsModel):
resstderr = function(M) {
  return ((sum(M$residual^2)/M$df[2])^(1/2))
}


# 6.6.2004

hammingdistance = function(x,y) {
  return(sum(x != y))
}

randomdistanceuniform = function(x, m) {
# x = 0-1-vector
# m = number of vectors, over which the minimal Hamming distance is computed
  n = length(x);
  mindistance = n;
  for (i in 1:m) {
    mindistance = min(mindistance, hammingdistance(x, sample(c(0,1), n, replace = TRUE)));
  }
  return(mindistance)
}
randomdistanceBE = function(x, m) {
  n = length(x);
  mindistance = n;
  for (i in 1:m) {
    p = runif(1);
    mindistance = min(mindistance, hammingdistance(x, sample(c(0,1), n, replace = TRUE, prob = c(1-p, p))));
  }
  return(mindistance)
}

experimentrandomdistances = function(q, n, m, N) {
# q = Bernoulli success probability for the "solution vector" x
# n = number of bits in x
# m = number of "guesses" for x
  unif = rep(0,n+1);
  BE = rep(0,n+1)
  for (i in 1:N) {
    x = sample(c(0,1), n, replace = TRUE, prob = c(1-q, q));
    d = randomdistanceuniform(x, m) + 1;
    unif[d] = unif[d] + 1;
    d = randomdistanceBE(x, m) + 1;
    BE[d] = BE[d] + 1;
  }
  dist = 0:n;
  return(list(freq = data.frame(unif, BE, dist), meanunif = sum(unif * dist) / N, meanBE = sum(BE * dist) / N));
}

predictx = function(model, x) {
  return(predict(model, data.frame(x=x)))
# assumes model has one independent variable, which is called "x"
}

write_matrix = function(m, file, append = FALSE) {
  write(t(m), file = file, ncolumns = dim(m)[2], append = append)
}

# Post processing dataframes

# Given a dataframe with with field field1, return the dataframe with
# an additional field field2 to the data.frame where the value of field2
# in each row is the result of running applying "conversion" to the matching
# text (given pattern) in field1 for that row.
# If pattern does not match some substring of field1's value in a particular
# row, then the result of field2 in that row is conversion("").
# MG: This should be removed and replicated somehow as a shell script.
#     NO string processing should be done at the R level.
extract_new_column = function(df,field1,field2,pattern, conversion) {
  temp_df = data.frame(do.call(c,
    lapply(df[,field1],
      function(s) {
        match = regexpr(pattern,s,perl=TRUE)
        if (match != -1) {
          conversion(substr(s,match[1],match[1]+attr(match,"match.length")-1))
        } else {
          conversion("")
        }
      } )))
  colnames(temp_df) = c(field2)
  data.frame(df,temp_df)
}
# Example:
# Given a data.frame df with a column file_name, with entries of the form
# "AES_R1_P0_K0_CX_KN89.cnf", the following:
#
#
# df2 <- extract_new_column(df,"file_name","unknown_key_bits",
#   "(?<=AES_R1_P0_K0_CX_KN).*(?=.cnf)",as.integer)
#
# produces a new data.frame df2 with a column "unknown_key_bits" which
# contains the integer in the string (89 in the example).

# Set screen-width (without arguments, to the given width):
wide_screen = function(wide=Sys.getenv("COLUMNS")) {
  options(width=as.integer(wide))
}
