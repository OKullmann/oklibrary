# Oliver Kullmann, 7.5.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

asciidoc-base-directory := $(prefix)/Asciidoc
asciidoc-directories := $(asciidoc-base-directory)

.PHONY : asciidoc $(asciidoc_targets) create_asciidoc_dirs

# #################################
# Main Asciidoc targets
# #################################

$(asciidoc-directories) : % : 
	mkdir $@

create_asciidoc_dirs : $(asciidoc-directories)

asciidoc : $(asciidoc_recommended)

$(asciidoc_targets) : create_asciidoc_dirs
	$(call unarchive,$@,$(asciidoc-base-directory))
	cd $(asciidoc-base-directory)/$@; $(postcondition) \
	sudo ./install.sh; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallasciidoc : 
	-rm -rf $(asciidoc-base-directory)
