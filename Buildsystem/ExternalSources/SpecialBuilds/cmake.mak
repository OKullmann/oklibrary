# Oliver Kullmann, 5.6.2007 (Swansea)

# ##################################
# Directory Structure
# ################################## 

cmake-base-directory := $(prefix)/Cmake
cmake_doc_dir := $(external_sources_doc_base_dir)/Cmake
cmake-directories := $(cmake-base-directory) $(cmake_doc_dir)

.PHONY : cmake $(cmake_targets) create_cmake_dirs

# #################################
# Main Cmake targets
# #################################

$(cmake-directories) : % : 
	mkdir -p $@

create_cmake_dirs : $(cmake-directories)

cmake : $(cmake_recommended)

$(cmake_targets) : create_cmake_dirs
	$(call unarchive,$@,$(cmake-base-directory)) $(postcondition) \
	cd $(cmake-base-directory)/$@; $(postcondition) \
	./bootstrap; $(postcondition) \
	make; $(postcondition) \
	cp -r Docs Example $(cmake_doc_dir); $(postcondition) \
	sudo make install; $(postcondition)

# #################################
# Cleaning
# #################################

cleanallcmake : 
	-rm -rf $(cmake-base-directory)
