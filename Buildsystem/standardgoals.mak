# Oliver Kullmann, 10.4.2005 (Swansea)
# Copyright 2005-2007, 2008, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Targets for building the OKlibrary (excluding external sources)

special_goals := prebuild

local_html_goals := html doxygen_documentation documentation_index
internet_goals := internet internet_home_page internet_local internet_global internet_packages
html_goals := $(local_html_goals) $(internet_goals)

test_files := test testop
new_test_phonies := new_test new_testop # the notion of "phony" seems inappropriate here (since these are files!) ?!
test_targets := check
new_test_targets := new_check maxima_test
test_goals :=$(test_files) $(test_targets)
new_test_goals := $(new_test_phonies) $(new_test_targets)

normal_goals := all unoptimised optimised app_tests timing documents

cleaning_goals := clean cleanobj cleantestobj cleandep cleanprograms cleanall cleantest cleantestop cleanalltests new_cleantest new_cleantestop new_cleanalltests

forced_goals := force

all_goals := $(special_goals) $(html_goals) $(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals)

.PHONY : $(special_goals) $(html_goals) $(test_targets) $(new_test_targets) $(normal_goals) $(cleaning_goals) $(forced_goals)
