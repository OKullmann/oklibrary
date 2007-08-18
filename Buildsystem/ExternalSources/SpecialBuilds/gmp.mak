# Matthew Henderson, 25.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

gmp_directories := $(gmp_base_directory)

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
	./configure; $(postcondition) \
	make; $(postcondition) \
	make check; $(postcondition) \
	cd doc	&& make gmp.dvi ; $(postcondition) \
#	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallgmp : 
	-rm -rf $(gmp_base_directory)
