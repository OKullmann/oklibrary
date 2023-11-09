# Oliver Kullmann, 9.9.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Sub-makefile defining rules to create local html documentation

temporary_makefile := $(aux_dir)/temporary.mak
# The point of the temporary makefile is, that $(rel_path_tool) must
# be available before $(OKconfiguration)/Html/include.mak is read
# (and this can only be achieved by attaching the action of creating/updating
# $(rel_path_tool) to the action of creating a makefile).

include $(temporary_makefile)

.PHONY : clean_up

$(temporary_makefile) : $(tool_dir)
	cp $(OKconfiguration)/Html/include.mak $(temporary_makefile)
	if [[ ! -f $(rel_path_tool) ]]; then \
	  cp $(rel_path_tool_origin) $(rel_path_tool); $(postcondition) \
	  strip --strip-unneeded $(rel_path_tool); $(postcondition) \
	elif [[ -e $(rel_path_tool_origin) &&  $(rel_path_tool_origin) -nt $(rel_path_tool) ]]; then \
	  cp -f $(rel_path_tool_origin) $(rel_path_tool); $(postcondition) \
	  strip --strip-unneeded $(rel_path_tool); $(postcondition) \
	fi; if [ $$? != 0 ]; then rm $(temporary_makefile); exit 1; fi

export

html : documentation_index doxygen_documentation

clean_up :
	rm $(temporary_makefile)

documentation_index : clean_up $(local_html_dir)
	$(preprocessing_call) $(local_home_page_template) > $(local_home_page_output)
	$(preprocessing_call) $(local_home_page_css_template) > $(local_home_page_css_output)
	$(preprocessing_call) $(aims_page_template) > $(aims_page_output)
	$(preprocessing_call) $(languages_page_template) > $(languages_page_output)
	$(preprocessing_call) $(history_page_template) > $(history_page_output)
	$(preprocessing_call) $(tenchallenges_page_template) > $(tenchallenges_page_output)
	$(preprocessing_call) $(ramseytheory_page_template) > $(ramseytheory_page_output)
	$(preprocessing_call) $(hardness_page_template) > $(hardness_page_output)
	$(preprocessing_call) $(packagehistory_page_template) > $(packagehistory_page_output)
	$(preprocessing_call) $(licence_page_template) > $(licence_page_output)
	$(preprocessing_call) $(tutorial_page_template) > $(tutorial_page_output)
	$(preprocessing_call) $(faq_page_template) > $(faq_page_output)
	$(preprocessing_call) $(externalsources_page_template) > $(externalsources_page_output)
	cp $(logo_template) $(logo_output)
	cp $(developers_template) $(developers_output)
	$(preprocessing_call) $(maxima_html_template_okl) > $(maxima_html_output_okl)
	$(preprocessing_call) $(coq_html_template) > $(coq_html_output)
	$(preprocessing_call) $(clisp_html_template_okl) > $(clisp_html_output_okl)
	$(preprocessing_call) $(gcc_html_template_okl) > $(gcc_html_output_okl)
	$(preprocessing_call) $(gap_html_template_okl) > $(gap_html_output_okl)
	cp $(release_history_template) $(release_history_output)
	cp -r --force $(documents_dir) $(doc_dir)


doxygen_documentation : clean_up $(doxygen_html_dir) $(log_dir)
	echo "Doxygen version: $$($(doxygen_call_okl) --version)"
	$(preprocessing_call) $(doxy_file_template) > $(doxy_file)
	$(doxygen_call_okl) $(doxy_file) $(Doxygen_modifier) 

