# Matthew Henderson, 25.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

gmp-base-directory := $(prefix)/gmp
gmp-directories := $(gmp-base-directory)

.PHONY : gmp $(gmp_targets) create_gmp_dirs

# #################################
# Main gmp targets
# #################################

$(gmp-directories) : % : 
	mkdir -p $@

gmp : $(gmp_recommended)

$(gmp_targets) : $(gmp-directories)
	$(call unarchive,$@,$(gmp-base-directory))
	cd $(gmp-base-directory)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallgmp : 
	-rm -rf $(gmp-base-directory)
