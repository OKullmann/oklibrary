# Oliver Kullmann, 4.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = VanderWaerdenCNF PdVanderWaerdenCNF

debug_progs = $(addsuffix _debug,$(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

VanderWaerdenCNF : VanderWaerdenCNF.cpp VanderWaerdenCNF.hpp
VanderWaerdenCNF : include_options += -I $(oklibloc)/..
VanderWaerdenCNF_debug : VanderWaerdenCNF.cpp VanderWaerdenCNF.hpp
VanderWaerdenCNF_debug : include_options += -I $(oklibloc)/..

PdVanderWaerdenCNF : PdVanderWaerdenCNF.cpp VanderWaerdenCNF.hpp
PdVanderWaerdenCNF : include_options += -I $(oklibloc)/..
PdVanderWaerdenCNF_debug : PdVanderWaerdenCNF.cpp VanderWaerdenCNF.hpp
PdVanderWaerdenCNF_debug : include_options += -I $(oklibloc)/..


at_list = $(addprefix at_,$(opt_progs)) $(addprefix at_,$(debug_progs))

.PHONY : $(at_list)

regtest :

apptest : $(at_list)

$(at_list) : at_% : %
#	./DirMatching ./$* tests/app_tests/$*
