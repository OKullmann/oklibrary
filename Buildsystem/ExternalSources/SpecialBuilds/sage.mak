# Matthew Henderson, 21.7.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 


sage_directories := $(sage_base_directory)

.PHONY : sage $(sage_targets)

# #################################
# Main sage targets
# #################################

$(sage_directories) : % : 
	mkdir -p $@

sage : $(sage_recommended)

$(sage_targets) : $(sage_directories)
	$(call unarchive_uncompressed,$@,$(sage_base_directory))
	cd $(sage_base_directory)/$@; $(postcondition) \
	make; $(postcondition) \
	cd devel/doc; $(postcondition) \
	make pdf; $(postcondition) \
	cd $(sage_base_directory)/$@; $(postcondition) \
	make test

# #################################
# Cleaning
# #################################

cleanallsage : 
	-rm -rf $(sage-base-directory)
