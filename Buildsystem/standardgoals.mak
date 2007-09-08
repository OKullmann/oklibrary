# Oliver Kullmann, 10.4.2005 (Swansea)

special_goals := prebuild
preparation_goals := createdirs 

html_goals := html internet documentation_index

test_files := test testop
new_test_phonies := new_test new_testop # the notion of "phony" seems inappropriate here (since these are files!) ?!
test_targets := check
new_test_targets := new_check
test_goals :=$(test_files) $(test_targets)
new_test_goals := $(new_test_phonies) $(new_test_targets)

normal_goals := all $(preparation_goals) unoptimised optimised app_tests documents

cleaning_goals := clean cleanobj cleantestobj cleandep cleanprograms cleanall cleantest cleantestop cleanalltests new_cleantest new_cleantestop new_cleanalltests

forced_goals := force

all_goals := $(special_goals) $(html_goals) $(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals)

.PHONY : $(special_goals) $(html_goals) $(test_targets)  $(new_test_targets) $(normal_goals) $(cleaning_goals) $(forced_goals)
