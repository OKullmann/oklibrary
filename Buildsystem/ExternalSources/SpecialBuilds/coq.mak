# Oliver Kullmann, 23.7.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 


coq_directories := $(coq_base_directory)

.PHONY : coq $(coq_targets)

# #################################
# Main coq targets
# #################################

$(coq_directories) : % : 
	mkdir -p $@

coq : $(coq_recommended)

$(coq_targets) : coq-% : $(coq_directories)
	$(call unarchive,sources/Coq/$@,$(coq_base_directory))
	cd $(coq_base_directory)/$@; $(postcondition) \
	export PATH=$(ocaml_bin_dir):${PATH}; $(postcondition) \
	./configure -opt --prefix $(coq_base_directory)/$* -fsets all -reals all; $(postcondition) \
	make world && make install; $(postcondition) \

# #################################
# Cleaning
# #################################

cleanallcoq : 
	-rm -rf $(coq-base-directory)

