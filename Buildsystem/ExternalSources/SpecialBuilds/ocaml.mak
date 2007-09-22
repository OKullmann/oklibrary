# Oliver Kullmann, 23.7.2007 (Swansea)

# Building Ocaml

# ##################################
# Directory Structure
# ################################## 


ocaml_directories := $(ocaml_base_directory)

.PHONY : ocaml $(ocaml_targets)

# #################################
# Main ocaml targets
# #################################

$(ocaml_directories) : % : 
	mkdir -p $@

ocaml : $(ocaml_recommended)

$(ocaml_targets) : ocaml-% : $(ocaml_directories)
	$(call unarchive,sources/Ocaml/$@,$(ocaml_base_directory))
	cd $(ocaml_base_directory)/$@; $(postcondition) \
	./configure -prefix $(ocaml_base_directory)/$*; $(postcondition) \
	make world && make bootstrap && make opt && make opt.opt && make install && make clean

# #################################
# Cleaning
# #################################

cleanallocaml : 
	-rm -rf $(ocaml-base-directory)

