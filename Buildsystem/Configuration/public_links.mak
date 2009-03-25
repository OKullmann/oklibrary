# Oliver Kullmann, 16.1.2008 (Swansea)
# Copyright 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# The list of links to be established to programs from the OKlibrary.

oksolver2002_okl ?= $(bin_dir)/OKsolver_2002-O3-DNDEBUG

ramsey_generator_okl ?= $(bin_dir)/Ramsey-O3-DNDEBUG
vanderwaerden_generator_okl ?= $(bin_dir)/VanderWaerden-O3-DNDEBUG
lininequal_trans_okl ?= $(bin_dir)/LinInequal-O3-DNDEBUG
boundones_okl ?=  $(OKlib)/Satisfiability/Transformers/Generators/BoundOnes
vdwtransversals_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversals
vdwtransversalspb_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsPB
vdwtransversalsinc_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsInc
vdwtransversalsincpb_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsIncPB

extdimacs2dimacs_okl ?= $(bin_dir)/ExtendedToStrictDimacs-O3-DNDEBUG


public_link_list_okl ?= $(oksolver2002_okl) $(ramsey_generator_okl) $(vanderwaerden_generator_okl) $(lininequal_trans_okl) $(boundones_okl) $(vdwtransversals_okl) $(vdwtransversalspb_okl) $(vdwtransversalsinc_okl) $(vdwtransversalsincpb_okl) $(extdimacs2dimacs_okl) $(oklib_call)

