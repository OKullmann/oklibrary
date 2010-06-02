# Mladen Nikolic, 15.5.2010 (Belgrade)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


gwtest = function(rts, ids, bootstraps) {
  ps=-0.5*gehanw(rts,ids)+0.5
  pooledsample=list(ranks=rank(rts),inds=ids)
  rv=rcor(pooledsample$ranks,pooledsample$inds)
  if(bootstraps==0)
    vr=jackknifevariance(pooledsample)
  else
    vr=bootstrapvariance(pooledsample,bootstraps)
  return(list(rvalue=rv, variance=vr, superiority=ps))
}

gehanw = function(rts,ids) {
  w=0
  rts1=rts[ids==1]
  rts2=rts[ids==-1]
  for(x in rts1) {
    w=w-sum(x<rts2)
    w=w+sum(x>rts2)
  }
  return(w/(length(rts1)*length(rts2)))
}

rcor = function(ranks,inds) {
  rv=cor(ranks,inds)
  if(is.na(rv))
    return(0)
  if(abs(rv-1)<0.00001)
    rv=rv-1/sqrt(var(ranks)*var(inds))
  if(abs(rv+1)<0.00001)
    rv=rv+1/sqrt(var(ranks)*var(inds))
  return(rv)
}

jackknifevariance = function(sample) {
  rhos=c()
  for(i in 1:length(sample$ranks))
    rhos=c(rhos,rcor(sample$ranks[-i],sample$inds[-i]))
  return(var(rhos)*(length(rhos)-1)^2/length(rhos))
}

bootstrapvariance = function(sample,bootstraps) {
  indices=1:length(sample$ranks)
  rhos=c()
  for(i in 1:bootstraps) {
    sel=sample(indices,length(indices),replace=TRUE)
    rhos=c(rhos,rcor(sample$ranks[sel],sample$inds[sel]))  
  }
  return(var(rhos))
}


# Determining the "probability of superiority", comparing 2 solvers
# (see convenience-function probsup_solvcomp_files below, which takes
# as input two filenames):
probsup_solvcomp = function(runtimes1, runtimes2, cutoff, discard, bootstraps) {

  # Checking that both tables have equal number of rows 
  # (that is, both solvers have been tested on same number of formulae).
  # Tables may have different number of columns (that is, different numbers
  # of shufflings of the same formula).

  if(dim(runtimes1)[[1]]!=dim(runtimes2)[[1]])
    stop("Dimensions of input tables do not agree!\n")

  # Forming indicator matrices --- data from the first sampel
  # have indicator 1, and data from the second sample have
  # indicator -1.

  inds1=matrix(1,dim(runtimes1)[1],dim(runtimes1)[2])
  inds2=matrix(-1,dim(runtimes2)[1],dim(runtimes2)[2])

  # Runtime samples are merged to form pooled samples

  runtimes=cbind(runtimes1,runtimes2)
  inds=cbind(inds1,inds2)

  num=0   # Number of used formulae
  ravg=0  # Average of r values (point biserial correlation)
  zsum=0  # Sum of z values (number of standard deviations that the datum 
          # deviates from the mean)
  zvar=0  # Sum of variances of estimates of z values
  savg=0  # Average of probabilities of superiority (P(S1<S2))

  ## Statistics calculation:

  for(i in 1:dim(runtimes1)[1]) {
    # If all the runtimes are greater than cutoff time, the data bear no
    # information, so the row is skipped. Also, the row is skipped if all the
    # runtimes are less then some small value "discard".

    if((sum(runtimes[i,]<cutoff)==0) || (max(runtimes1[i,])<discard))
      next

    num=num+1
    cat(".")

    # Gehan-Wilcoxon test is performed, and the statistics are calculated

    stats=gwtest(runtimes[i,],inds[i,],bootstraps)
    ravg=ravg+stats$rvalue
  
    # r values are transformed using the Fisher transformation

    zsum=zsum+0.5*log((1+stats$rvalue)/(1-stats$rvalue))

    # Variance of transformed r values is computed
  
    zvar=zvar+stats$variance/(1-stats$rvalue^2)^2
    savg=savg+stats$superiority
  }

  ravg=ravg/num
  savg=savg/num

  # Output results

  cat("\n")

  cat("Number of used formulae: ",num,"\n",sep="")
  cat("Average of z values: ",zsum/sqrt(zvar),"\n",sep="")
  cat("Average of r values: ",ravg,"\n",sep="")
  cat("p-value of the test: ",2-2*pnorm(abs(zsum/sqrt(zvar))),"\n",sep="")
  cat("Average of probabilites of superiority: ",savg,"\n",sep="")
}

# Reading two files, each containing a simple data-table, without heading; the
# files must have the same number of lines, but the number of columns can
# vary between the two files (though not internally):
read_probsup_solvcomp = function(file1, file2) {
  t1=read.table(file1);
  t2=read.table(file2);
  return(list(table1=t1, table2=t2));
}


probsup_solvcomp_files= function(file1, file2, cutoff, discard, bootstraps) {
  tables=read_probsup_solvcomp(file1,file2);
  return(probsup_solvcomp(tables$table1,tables$table2,cutoff,discard,bootstraps));
}
