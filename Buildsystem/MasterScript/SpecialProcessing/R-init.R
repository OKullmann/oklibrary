# Oliver Kullmann, 4.6.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

OKplatform = "m4_SHELL(OKplatform)"
OKsystem = "m4_SHELL(OKsystem)"
OKlib = "m4_SHELL(OKlib)"

oklib_R_directories = c("OKlib/Experimentation/ExperimentSystem", "OKlib/Statistics")

sconcat = function(...) { paste(..., sep = "") }

oklib_session_name = sconcat("R_session_", "m4_SHELL(build_identification_okl)")

oklib_plain_load = function(filename) {
 source(filename, echo = FALSE, encoding = "utf8")
}

oklib_load = function(filename) {
 oklib_plain_load(file.path(OKsystem, filename))
}

oklib_load_dir = function(path, base = OKsystem, trace = TRUE, ...) {
  for (p in file.path(base,path)) 
    for (filename in list.files(p, pattern = "\\.R$", recursive = TRUE)) {
    if (trace) cat(filename)
    source(file.path(p,filename), ...)
    if (trace) cat("\n")
  }
}

oklib_load_all = function() {
  oklib_load_dir(path = oklib_R_directories)
}

