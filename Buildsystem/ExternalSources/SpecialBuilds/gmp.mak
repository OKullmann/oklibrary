# Matthew Henderson, 25.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

gmp_directories := $(gmp_base_directory) $(gmp_documentation_dir)

.PHONY : gmp $(gmp_targets)

# #################################
# Main gmp targets
# #################################

$(gmp_directories) : % : 
	mkdir -p $@

gmp : $(gmp_recommended)

$(gmp_targets) : $(gmp_directories)
	$(call unarchive,sources/Gmp/$@,$(gmp_base_directory)) $(postcondition) \
	cd $(gmp_base_directory)/$@; $(postcondition) \
	./configure --prefix=$(gmp_install_directory); $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	make html; $(postcondition) \
	cp -r doc/gmp.html $(gmp_documentation_dir); $(postcondition) \
	cd doc && make gmp.dvi; $(postcondition) \
	cp gmp.dvi $(gmp_documentation_dir); $(postcondition) \
	cd ..; $(postcondition) \
	$(gmp_install_command)

# #################################
# Cleaning
# #################################

cleanallgmp : 
	-rm -rf $(gmp_base_directory)
