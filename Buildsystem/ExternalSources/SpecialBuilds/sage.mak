# Matthew Henderson, 21.7.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

sage-base-directory := $(prefix)/sage
sage-directories := $(sage-base-directory)

.PHONY : sage $(sage_targets) create_sage_dirs

# #################################
# Main sage targets
# #################################

$(sage-directories) : % : 
	mkdir -p $@

sage : $(sage_recommended)

$(sage_targets) : $(sage-directories)
	$(call unarchive_uncompressed,$@,$(sage-base-directory))
	cd $(sage-base-directory)/$@; $(postcondition) \
	make; $(postcondition) 

# #################################
# Cleaning
# #################################

cleanallsage : 
	-rm -rf $(sage-base-directory)
 
