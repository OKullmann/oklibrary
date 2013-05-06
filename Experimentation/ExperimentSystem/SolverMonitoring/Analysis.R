# Oliver Kullmann, 6.5.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Tools for evaluating SAT-solver statistics


oklib_load("OKlib/Statistics/R/Utilities.R")


# Generic function for reading a statistics-file created by our
# extraction-tools:
read_satstat = function(file, ...) {
  E=read.table(file,header=TRUE)
  E
}
