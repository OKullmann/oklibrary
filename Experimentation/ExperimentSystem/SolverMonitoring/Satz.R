# Oliver Kullmann, 2.9.2012 (Swansea)
# Copyright 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


# #######
# Input #
# #######

# Reading the output of satz215 solver from an R-like file, as produced
# by ExtractSatz (with header-line, and every line starting with the
# running number), and putting it into a dataframe.
# Can be also used when to the front (after the running number) various
# integer-fields are added (for example as produced by the CRun-scripts).

read_satz = function(filename, ...) {
  E = read.table(file = filename,
        header = T,
        colClasses = c(rep("integer",lengthfirstline(filename)-11),"numeric","integer",rep("numeric",4),"character",rep("numeric",5)),
        ...)
  E
}
