# Oliver Kullmann, 16.1.2008 (Swansea)
# Copyright 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Links to be established

oksolver2002_okl ?= $(bin_dir)/OKsolver_2002-O3-DNDEBUG

ramsey_generator_okl ?= $(bin_dir)/Ramsey-O3-DNDEBUG

extdimacs2dimacs_okl ?= $(bin_dir)/ExtendedToStrictDimacs-O3-DNDEBUG

public_link_list_okl ?= $(oksolver2002_okl) $(ramsey_generator_okl) $(extdimacs2dimacs_okl) $(oklib_call)

