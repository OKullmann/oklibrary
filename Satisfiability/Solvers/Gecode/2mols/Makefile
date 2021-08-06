# Oleg Zaikin, 6.8.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

atps       = 2mols0 2mols1 2mols2 2mols3 2mols4

atps       := $(addprefix at_,$(atps))

.PHONY : $(atps)

apptest : $(atps)

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
