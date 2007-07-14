html : $(doxygen_html_dir) documentation_index
	echo "Doxygen version: $$(doxygen --version)"; rm -r $(doxygen_html_dir)/*; ( cat $(doxy_file); echo $(doxygen-parameters) ) | doxygen - $(Doxygen_modifier) 
