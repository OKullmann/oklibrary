# Oliver Kullmann, 9.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Sub-makefile defining rules to create local html documentation

temporary_makefile := $(aux_dir)/temporary.mak
# The point of the temporary makefile is, that $(rel_path_tool) must
# be available before $(OKconfiguration)/Html/include.mak is read.

include $(temporary_makefile)

.PHONY : clean_up
$(temporary_makefile) : $(rel_path_tool)
	cp $(OKconfiguration)/Html/include.mak $(temporary_makefile)
$(rel_path_tool) : $(tool_dir)
	cp -f $(rel_path_tool_origin) $(rel_path_tool)
	strip --strip-unneeded $(rel_path_tool)

export

html : documentation_index doxygen_documentation

clean_up :
	rm $(temporary_makefile)

documentation_index : clean_up $(local_html_dir)
	$(preprocessing_call) $(local_home_page_template) > $(local_home_page_output)
	$(preprocessing_call) $(aims_page_template) > $(aims_page_output)
	$(preprocessing_call) $(languages_page_template) > $(languages_page_output)
	$(preprocessing_call) $(history_page_template) > $(history_page_output)
	$(preprocessing_call) $(tutorial_page_template) > $(tutorial_page_output)
	$(preprocessing_call) $(faq_page_template) > $(faq_page_output)
	$(preprocessing_call) $(externalsources_page_template) > $(externalsources_page_output)
	cp $(logo_template) $(logo_output)
	cp $(developers_template) $(developers_output)
	$(preprocessing_call) $(sage_html_template) > $(sage_html_output)
	$(preprocessing_call) $(coq_html_template) > $(coq_html_output)
	cp $(release_history_template) $(release_history_output)
	cp -r --force $(documents_dir) $(doc_dir)


doxygen_documentation : clean_up $(doxygen_html_dir) $(log_dir)
	echo "Doxygen version: $$(doxygen --version)" && $(preprocessing_call) $(doxy_file_template) > $(doxy_file) && $(doxygen_call) $(doxy_file) $(Doxygen_modifier) 



