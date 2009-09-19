# Matthew Gwynne, 19.9.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading picosat output (stdout) and returning a data.frame
read_picosat_output = function(filename, ...) {
  S = system(paste("cat ", filename," | grep \"^c\""), intern=TRUE)
  result = list()
  for (line in S) {
    line = gsub("^c *", "",line)
    if (length(grep("^parsing *",line)) > 0) {
      result = c(result,list(filename=gsub("^parsing *","",line)))
    } else if (length(grep("^initialized", line)) > 0) {
      result = c(result,
        list(num_variables=gsub("[^0-9]","",line)))
    } else if (length(grep("trivial clauses",line)) > 0) {
      result = c(result,
        list(num_clauses=gsub("[^0-9]","",line)))
    } else if (length(grep("^[0-9\\.]+[ %]+[a-zA-z]",line)) > 0) {
      line = unlist(strsplit(line, " "))
      name = gsub("[^a-zA-Z_\\-]","_",
        gsub(" +"," ",do.call(paste,as.list(line[2:length(line)]))))
      if (length(grep("%",line[1])) > 0) {
        line[1] = gsub("\\%","",line[1])
        name = paste(name,"_pc",sep="")
      }
      result = c(result,
        eval(parse(text=paste("list(",name,"=",as.double(line[1]),")"))))
    }
  }
  data.frame(result)
}

# Reading a set of minisat2 outputs into a data.frame (given as a list of
# filenames)
read_picosat_outputs = function(filenames) {
 for(file in filenames) {
   if(exists("result_df")) {
     result_df = rbind(result_df,read_picosat_output(file))
   } else {
     result_df = read_oksolver_output(file)
   }
 }
 result_df
}
# Note one can use:
# read_minisat2_outputs(dir(pattern=glob2rx("*.result")))
# to read all files with ".result" as the end.
