html : $(doxygen_html_dir) documentation_index $(log_dir)
	- rm -r $(doxygen_html_dir)/*
	echo "Doxygen version: $$(doxygen --version)" && $(preprocessing_call) $(doxy_file_template) > $(doxy_file) && $(doxygen_call) $(doxy_file) $(Doxygen_modifier) 
