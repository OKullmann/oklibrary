# Oliver Kullmann, 10.4.2005 (Swansea)

special_goals := prebuild
preparation_goals := createdirs 

html_goals := html

test_files := test testop
new_test_phonies := new_test new_testop
test_targets := check
new_test_targets := new_check
test_goals :=$(test_files) $(test_targets)
new_test_goals := $(new_test_phonies) $(new_test_targets)

normal_goals := all $(preparation_goals) unoptimised optimised

cleaning_goals := clean cleanobj cleantestobj cleandep cleanprograms cleanall

forced_goals := force

all_goals := $(special_goals) $(html_goals) $(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals)

.PHONY : $(special_goals) $(html_goals) $(test_targets)  $(new_test_targets) $(normal_goals) $(cleaning_goals) $(forced_goals)
