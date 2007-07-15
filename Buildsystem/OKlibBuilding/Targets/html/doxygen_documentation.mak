html : $(doxygen_html_dir) documentation_index
	- rm -r $(doxygen_html_dir)/*
	echo "Doxygen version: $$(doxygen --version)" && $(preprocessing_call) $(doxy_file_template) > $(doxy_file) && doxygen $(doxy_file) $(Doxygen_modifier) 
