# Oliver Kullman, 20.12.2019 (Swansea
# Copyright 2019, 2020, 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Mols Pandiagonal CPandiagonal CspPandiagonal Sort ExpandQueensCubes ExactCoverQueensCubes ECSAT0_QueensCubes ECSAT1_QueensCubes
debug_progs = Mols_debug Pandiagonal_debug CPandiagonal_debug CspPandiagonal_debug Sort_debug ExpandQueensCubes_debug ExactCoverQueensCubes_debug ECSAT0_QueensCubes_debug ECSAT1_QueensCubes_debug

popt_progs = $(addprefix p-, $(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

Mols : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp AloAmo.hpp Commandline.hpp
Mols_debug : Mols.cpp $(programmingloc)/ProgramOptions/Environment.hpp ProgramInfo.hpp Errors.hpp Options.hpp Statistics.hpp Encoding.hpp AloAmo.hpp Commandline.hpp

Pandiagonal : Pandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp
Pandiagonal_debug : Pandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp

CPandiagonal : CPandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp
CPandiagonal_debug : CPandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp

CspPandiagonal : CspPandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp
CspPandiagonal_debug : CspPandiagonal.cpp PQOptions.hpp AloAmo.hpp PQEncoding.hpp Statistics.hpp Commandline.hpp Errors.hpp

Sort : Sort.cpp
Sort_debug : Sort.cpp

ExpandQueensCubes : ExpandQueensCubes.cpp Algorithms.hpp EQOptions.hpp
ExpandQueensCubes_debug : ExpandQueensCubes.cpp Algorithms.hpp EQOptions.hpp

ExactCoverQueensCubes : ExactCoverQueensCubes.cpp Algorithms.hpp
ExactCoverQueensCubes_debug : ExactCoverQueensCubes.cpp Algorithms.hpp

ECSAT0_QueensCubes : ECSAT0_QueensCubes.cpp Algorithms.hpp ECEncoding.hpp PQOptions.hpp
ECSAT0_QueensCubes_debug : ECSAT0_QueensCubes.cpp Algorithms.hpp ECEncoding.hpp PQOptions.hpp

ECSAT1_QueensCubes : ECSAT1_QueensCubes.cpp Algorithms.hpp ECEncoding.hpp PQOptions.hpp
ECSAT1_QueensCubes_debug : ECSAT1_QueensCubes.cpp Algorithms.hpp ECEncoding.hpp PQOptions.hpp
