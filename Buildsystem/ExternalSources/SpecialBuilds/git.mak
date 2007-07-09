# Oliver Kullmann, 6.5.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

git-base-directory := $(prefix)/Git
git_doc_dir := $(external_sources_doc_base_dir)/Git
git-directories := $(git-base-directory) $(git_doc_dir)

.PHONY : git $(git_targets) create_git_dirs

# #################################
# Main Git targets
# #################################

$(git-directories) : % : 
	mkdir -p $@

create_git_dirs : $(git-directories)

git : $(git_recommended)

$(git_targets) : create_git_dirs
	$(call unarchive,$@,$(git-base-directory)) $(postcondition) \
	cd $(git-base-directory)/$@; $(postcondition) \
	make configure; $(postcondition) \
	sh ./configure --prefix=/usr/local; $(postcondition) \
	make all doc; $(postcondition) \
	cp -r $(git-base-directory)/$@/Documentation $(git_doc_dir); $(postcondition) \
	sudo make install install-doc; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallgit : 
	-rm -rf $(git-base-directory)
