# Oliver Kullmann, 22.8.2007 (Swansea)
# Copyright 2007, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Sub-makefile defining rules to create the internet web pages

include $(OKconfiguration)/Html/include.mak
export

internet : internet_local internet_global internet_packages

internet_local : $(internet_html_dir) 
	env --ignore-environment HOME=$$HOME PATH=$$PATH TERM=$$TERM LANG=$$LANG oklib doc_dir=$(internet_html_dir)/doc ExternalSources_doc=$(internet_html_dir)/doc/doc internet_home_page_output_dir=$(internet_html_dir) extsrc_relative=on html internet_home_page
	cp -r $(ExternalSources_doc) $(internet_html_dir)/doc/

internet_home_page : $(internet_home_page_output_dir)
	$(preprocessing_call) $(internet_home_page_template) > $(internet_home_page_output)

internet_global :
	cp -r --force $(internet_html_dir) $(internet_dir)

internet_packages :
	mv $(packages_dir)/* $(internet_dir)/packages

