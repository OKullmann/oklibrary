# Oliver Kullmann, 5.5.2005 (Swansea)

choice_n = function(A,id,l,r,v) { return(subset(A, solverid == id & p == l & result == r & n == v, select = c(time))) }
mean_n = function(A,id,l,r,v) { A = choice_n(A,id,l,r,v)$time; return(c(length(A),mean(A))) }
vec_n = function(A,l) { return(orderedset(subset(A, p == l)$n)) } 
char = function(A,id,l,r) { v = vec_n(A,l); return(sapply(v, mean_n, A = A, id = id, l = l, r = r)) }
charp = function(A,id,l,r) { ch = char(A,id,l,r); if (max(ch[1,]) == 0) return(NA) else return(list(id, char(A,id,l,r))) }

overview = function(A,l,r) { ids = levels(A$solverid); L = lapply(ids, charp, A = A, l = l, r = r); return(L[!is.na(L)]) }

print.overview = function(O) { for (i in 1:length(O)) { print(O[[i]][[1]]); print(O[[i]][[2]]) }  }

write.overview = function(O, file, append = FALSE) {
  l = length(O);
  if (l == 0) { return() }
  d = dim(O[[1]][[2]]);
  c = d[2];
  write(c(l, c), file = file, append = append); write("", file = file, append = TRUE);
  for (o in O) {
    write(o[[1]], file = file, append = TRUE);
    write_matrix(o[[2]], file = file, append = TRUE); write("", file = file, append = TRUE);
  }
}

read.overview = function(file) {
  A <- read.csv(file,row.names=NULL);
  return(transform(A, p=factor(p,ordered=TRUE),benchid=factor(benchid),solverid=factor(solverid,ordered=TRUE),result=factor(result),timeout=factor(timeout)))
}

