# Oliver Kullmann, 6.5.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

git-base-directory := $(prefix)/Git
git-directories := $(git-base-directory)

.PHONY : git $(git_targets) create_git_dirs

# #################################
# Main Git targets
# #################################

$(git-directories) : % : 
	mkdir $@

create_git_dirs : $(git-directories)

git : $(git_recommended)

$(git_targets) : create_git_dirs
	$(call unarchive,$@,$(git-base-directory))
	cd $(git-base-directory)/$@; $(postcondition) \
	make configure; $(postcondition) \
	sh ./configure --prefix=$(git-base-directory); $(postcondition) \
	make all install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallgit : 
	-rm -rf $(git-base-directory)
