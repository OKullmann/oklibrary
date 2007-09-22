# Matthew Henderson, 25.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

R-base-directory := $(prefix)/R
R-directories := $(R-base-directory)

.PHONY : R $(R_targets) create_R_dirs

# #################################
# Main R targets
# #################################

$(R-directories) : % : 
	mkdir -p $@

R : $(R_recommended)

$(R_targets) : $(R-directories)
	$(call unarchive,sources/R/$@,$(R-base-directory))
	cd $(R-base-directory)/$@; $(postcondition) \
	./configure; $(postcondition) \
	make; $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallR : 
	-rm -rf $(R-base-directory)
