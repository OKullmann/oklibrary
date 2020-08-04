# Oliver Kullman, 20.12.2019 (Swansea
# Copyright 2019, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Mols
debug_progs = Mols_debug

popt_progs = $(addprefix p-, $(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

Mols : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp
Mols_debug : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp
