doxy_file := $(OKbuildsystem)/makefile_generic_include/documentation/documentation_resources/Doxyfile

doxygen-parameters := 
Doxygen_modifier := 2> $(aux_dir)/DoxygenErrorMessages

html : $(html_dir) documentation_index
	echo "Doxygen version: $$(doxygen --version)"; rm -r $(html_dir)/*; cd $(OKplatform); ( cat $(doxy_file); echo $(doxygen-parameters) "OUTPUT_DIRECTORY=$(doc_dir)" ) | doxygen - $(Doxygen_modifier) 
