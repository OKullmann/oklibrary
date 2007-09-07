# Creating an index in $(system_directories)/doc to all the OKplatform
# documentation, including external resources. This index also contains
# links to the web pages of Boost, Doxygen, Gcc etc

# ##################################################
# OKlibrary Documentation Location
# ##################################################

OKlibrary_full_documentation_dvi_location := $(doc_dir)/dvi/FullDocumentation.dvi


# ##################################################
# Main target for creating index
# ##################################################

documentation_index_file := $(doc_dir)/index.html

# temporary (OK):
.PHONY : new_documentation_index
documentation_index : new_documentation_index | $(doc_dir)

new_documentation_index : $(local_html_dir)
	$(preprocessing_call) $(local_home_page_template) > $(local_home_page_output)
	$(preprocessing_call) $(aims_page_template) > $(aims_page_output)
	$(preprocessing_call) $(history_page_template) > $(history_page_output)
	$(preprocessing_call) $(externalsources_page_template) > $(externalsources_page_output)
	cp $(logo_template) $(logo_output)
	cp $(developers_template) $(developers_output)
	$(preprocessing_call) $(sage_html_template) > $(sage_html_output)
	$(preprocessing_call) $(coq_html_template) > $(coq_html_output)
