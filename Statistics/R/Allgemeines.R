# Oliver Kullmann, 11.8.2002 (Swansea)

# range difference for vector x
ranged <- function(x) {
  r <- range(x);
  return(r[2]-r[1])
}

# find element in vector
find.e <- function(e, vec) {
  match(e, vec);
}

# select element of vectors which are in a list of values
select.v <- function(vec, val) {
  vec[vec %in% val]
}

orderedset <- function(x) {
  l = length(x);
  if (l <= 1) { return(x) }
  x = sort(x, method="quick");
  sel = vector("logical", l);
  sel[1] = TRUE;
  for (i in 2:l) {
    sel[i] = x[i] > x[i-1]
  }
  return(x[sel])
}

# replace substrings
replace.substrings <- function(x, a, b) {
  gsub(a,b,x)
}
# single substitutions with sub(a,b,x)


# radius of the f%-confidence interval with center p for the observed
# proportion p using N observations
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

# In a string s representing a number, add surrounding $'s, and replace "e" by a tex-representation
texexponent = function(s) {
  if (regexpr("e", s) != -1) { # s contains e
    return(paste("$", sub("e","\\\\cdot 10^{",s), "}", "$", sep=""))
  }
  else {
    return(paste("$", s, "$"))
  }
}

# formatting small numbers;
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

# formatting long natural numbers
longintegers = function(n) {
  return(formatC(n,format="d",big.mark=","))
}


# the residual standard error for objects returned by summary(nlsModel)
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

