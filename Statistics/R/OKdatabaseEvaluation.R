# Oliver Kullmann, 26.5.2002 (Swansea)
# Copyright 2002-2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

end <- function(x) { length(x) + 1 }
# as in the STL

isgezero <- function(x) {
  return(x >= - 1e-12)
}

nround <- function(x) {
  s <- sign(x);
  x <- abs(x);
  z <- trunc(x);
  posp1 <- isgezero((x - z) - 0.5);
  z <- z + posp1;
  return(s * z)
}


invinterpolation <- function(y, x1, y1, x2, y2) { return((y - y1) * (x2 - x1) / (y2 - y1) + x1) }

invextinterpolation <- function(x, y, y0) {
  if (length(x) <= 1) return(NA);
  i <- max(which(y >= y0));
  if (i <= 0) return(NA);
  if (y[i] == y0) return(x[i]);
  if (i == length(x)) return(NA);
  return(c(invinterpolation(y0, x[i], y[i], x[i+1], y[i+1]), i))
}

invitinterpolationsat <- function(A, p) {
# Returns a list with components n, t, c, pc, where n, t and pc are vectors,
# and c is a list, and n,t,c,pc are of the same length;
# n contains the number of variables (ordered), 
# t is the (linearly approximated) density with sat-probability p,
# pc is the probability that the number of clauses at the left density has
#    sat-probability > 0.5, while for the number of clauses at the right
#    density the sat-probability is < 0.5,
# and c contains lists of length 12 with elements:
# 1. difference (>0) of neighbour densities;
# 2. difference (>0) of sat-probabilities at these densities;
# 3. number of samples at left density;
# 4. number of samples at right density;
# 5. left density;
# 6. right density;
# 7. number of clauses at left density
# 8. number of clauses at right density
# 9. left side of the 95% confidence interval for the left sat-probability
# 10. right side of  the 95% confidence interval for the left sat-probability
# 11. left side of the 95% confidence interval for the right sat-probability
# 12. right side of the 95% confidence interval for the right sat-probability
  n <- orderedset(A$n);
  nr <- c(); tr <- c(); cr <- list(); pcr <- c();
  i <- 1; j <- 0;
  while (i != end(n)) {
    sel <- A$n == n[i]
    x <- A$d1[sel]; y <- A$sat[sel]; count <- A$c[sel];
    r <- invextinterpolation(x, y, p);
    rv <- r[1]; ri <- r[2];
    if (! is.na(rv) & ! is.na(ri)) {
      j <- j+1;
      nr[j] <- n[i]; tr[j] <- rv;
      cr <- c(cr, list(c(x[ri+1] - x[ri], y[ri] - y[ri+1], count[ri], count[ri+1], x[ri], x[ri+1], nround(x[ri]*n[i]), nround(x[ri+1]*n[i]), confprop(95, y[ri], count[ri]), confprop(95, y[ri+1], count[ri+1]))));
      pcr[j] <- (1 - pt((0.5-y[ri])*sqrt(count[ri]-1)/y[ri]/(1-y[ri]), count[ri]-1)) * pt((0.5-y[ri+1])*sqrt(count[ri+1]-1)/y[ri+1]/(1-y[ri+1]), count[ri+1]-1);
    }
    i <- i + 1;
  }
  return(list ( n = nr, t = tr, c = cr, pc = pcr ))
}

invitinterpolationpl <- function(A, p) {
  n <- orderedset(A$n);
  nr <- c(); tr <- c(); cr <- list();
  i <- 1;
  while (i != end(n)) {
    sel <- A$n == n[i]
    x <- A$d1[sel]; y <- A$trv[sel]; count <- A$c[sel];
    r <- invextinterpolation(x, y, p);
    rv <- r[1]; ri <- r[2];
    if (! is.na(rv) & ! is.na(ri)) {
      nr <- append(nr, n[i]); tr <- append(tr, rv);
      cr <- c(cr, list(c(x[ri+1] - x[ri], y[ri] - y[ri+1], count[ri], count[ri+1])));
    }
    i <- i + 1;
  }
  return(list ( n = nr, t = tr, c = cr ))
}


# 15.7.2002
rho <- function(n, p, A) {
# n : number of variables
# p : probability
# A : data frame with A$sat (probability of sat, A$d1 (density) and A$n
# output: linearly approximated rho, where probability is approx. p;
# NA if not computable
  pos <- A$sat > 0 & A$sat < 1 & A$n == n;
  if (length(pos[pos]) <= 1) return(NA);
  f <- approxfun(A$sat[pos], A$d1[pos], rule = 1);
  return(f(p))
}

# 15.7.2002
critical <- function(e, A, printreg = TRUE) {
# Evaluation of rho at e and 1 - e for the critical exponent
  n <- orderedset(A$n);
  diff <- c(); nr <- c();
  i <- 1;
  while (i != end(n)) {
    rho2 <- rho(n[i], e, A);
    rho1 <- rho(n[i], 1 - e, A);
    if (! is.na(rho2) & ! is.na(rho1)) {
      nr <- append(nr, n[i]);
      diff <- append(diff, rho2 - rho1);
    }
    i <- i + 1;
  }
  if (length(nr) >= 3) {
    LinM <- lm(log(diff) ~ +1 + I(log(nr)));
    if (printreg) {
      plot(log(nr), log(diff), xlab = "", ylab = "", main = paste("p=", e, ", r^2=", round(summary(LinM)$adj.r.squared, 3), ", b=", round(coef(LinM)[2], 3)), cex.main=1.5);
      lines(log(nr), predict(LinM));
    }
    critic <- -1 / coef(LinM)[2]
  }
  else {
    LinM <- NA; critic <- NA;
  }
  return(list(n = nr, d = diff, LM = LinM, crit = critic));
}

extcrit <- function(x) {
  return(x$crit)
}
extlm <- function(x) {
  return(x$LM)
}
extn <- function(x) {
  return(x$n)
}
extd <- function(x) {
  return(x$d)
}
extfactor <- function(x) {
  return(exp(coef(x$LM)[1]))
}


# 17.7.2002
rhopl <- function(n, p, A) {
# n : number of variables
# p : probability
# A : data frame with A$trv (probability of satisfiability by pure literals (alone), A$d1 (density) and A$n
# output: linearly approximated rho, where probability is approx. p;
# NA if not computable
  pos <- A$trv > 0 & A$trv < 1 & A$n == n;
  if (length(pos[pos]) <= 1) return(NA);
  f <- approxfun(A$trv[pos], A$d1[pos], rule = 1);
  return(f(p))
}

# 17.7.2002
criticalpl <- function(e, A, printreg = TRUE) {
# Evaluation of rhopl at e and 1 - e for the critical exponent
  n <- orderedset(A$n);
  diff <- c(); nr <- c();
  i <- 1;
  while (i != end(n)) {
    rho2 <- rhopl(n[i], e, A);
    rho1 <- rhopl(n[i], 1 - e, A);
    if (! is.na(rho2) & ! is.na(rho1)) {
      nr <- append(nr, n[i]);
      diff <- append(diff, rho2 - rho1);
    }
    i <- i + 1;
  }
  if (length(nr) >= 3) {
    LinM <- lm(log(diff) ~ +1 + I(log(nr)));
    if (printreg) {
      plot(log(nr), log(diff), xlab = "", ylab = "", main = paste("p=", e, ", r^2=", round(summary(LinM)$adj.r.squared, 3), ", b=", round(coef(LinM)[2], 3)), cex.main=1.5);
      lines(log(nr), predict(LinM));
    }
    critic <- -1 / coef(LinM)[2]
  }
  else {
    LinM <- NA; critic <- NA;
  }
  return(list(n = nr, d = diff, LM = LinM, crit = critic));
}

chipl <- function(p, n, A, rhoc, nu) {
# p probability
# n testvector
# A datamatrix
# rhoc critical rho-value
# nu critical exponent
  test <- sapply(n, rhopl, A = A, p = p);
  return(mean((test - rhoc) * n^(1/nu)))
}


# 19.7.2002
ModelListUnsat <- function(n, A) {
# n vector of relevant n-values
# A datamatrix
  c1 <- c(); c2 <- c(); c3 <- c();
  for (i in n) {
    pos <- A$n == i;
    LM <- lm(log(A$nds[pos]) ~ +1 + I(A$d1[pos]^-1) + I(A$d1[pos]^-2));
    c1 <- append(c1, coef(LM)[1]);
    c2 <- append(c2, coef(LM)[2]);
    c3 <- append(c3, coef(LM)[3]);
  }
  return(list(c1 = c1, c2 = c2, c3 = c3))
}

ModelListUnsatsmall <- function(n, A, rho) {
# n vector of relevant n-values
# A datamatrix
# cut off value for rho
  c1 <- c(); c2 <- c(); c3 <- c();
  for (i in n) {
    pos <- A$n == i & A$d1 <= rho;
    LM <- lm(log(A$nds[pos]) ~ +1 + I(A$d1[pos]^1) + I(A$d1[pos]^2));
    c1 <- append(c1, coef(LM)[1]);
    c2 <- append(c2, coef(LM)[2]);
    c3 <- append(c3, coef(LM)[3]);
  }
  return(list(c1 = c1, c2 = c2, c3 = c3))
}
