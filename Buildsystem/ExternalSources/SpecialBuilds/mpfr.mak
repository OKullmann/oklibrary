# Matthew Henderson, 25.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

mpfr-base-directory := $(prefix)/mpfr
mpfr-directories := $(mpfr-base-directory)

.PHONY : mpfr $(mpfr_targets) create_mpfr_dirs

# #################################
# Main mpfr targets
# #################################

$(mpfr-directories) : % : 
	mkdir -p $@

mpfr : $(mpfr_recommended)

$(mpfr_targets) : $(mpfr-directories)
	$(call unarchive,$@,$(mpfr-base-directory))
	cd $(mpfr-base-directory)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallmpfr : 
	-rm -rf $(mpfr-base-directory)
