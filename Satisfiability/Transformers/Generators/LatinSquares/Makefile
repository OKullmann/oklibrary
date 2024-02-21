# Oliver Kullman, 20.12.2019 (Swansea
# Copyright 2019, 2020, 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Mols Pandiagonal
debug_progs = Mols_debug Pandiagonal_debug

popt_progs = $(addprefix p-, $(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

Mols : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp AloAmo.hpp Commandline.hpp
Mols_debug : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp AloAmo.hpp Commandline.hpp

Pandiagonal : Pandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp
Pandiagonal_debug : Pandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp
