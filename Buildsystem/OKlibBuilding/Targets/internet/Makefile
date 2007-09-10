# Oliver Kullmann, 22.8.2007 (Swansea)

# Sub-makefile defining rules to create the internet web pages

include $(OKconfiguration)/Html/include.mak
export

internet : internet_local internet_global

internet_local : $(internet_html_dir) 
	env --ignore-environment PATH=$$PATH oklib doc_dir=$(internet_html_dir)/doc ExternalSources_doc=$(internet_html_dir)/doc/doc internet_home_page_output_dir=$(internet_html_dir) html internet_home_page
	cp -r $(ExternalSources_doc) $(internet_html_dir)/doc/

internet_home_page : $(internet_home_page_output_dir)
	$(preprocessing_call) $(internet_home_page_template) > $(internet_home_page_output)

internet_global :
	cp -r --force $(internet_html_dir) $(internet_dir)

