html : $(doxygen_html_dir) documentation_index
	echo "Doxygen version: $$(doxygen --version)" && rm -r $(doxygen_html_dir)/* && $(preprocessing_call) $(doxy_file_template) > $(doxy_file) && doxygen $(doxy_file) $(Doxygen_modifier) 
