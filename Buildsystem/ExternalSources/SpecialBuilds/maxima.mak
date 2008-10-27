# Oliver Kullmann, 24.12.2007 (Swansea)
# Copyright 2007, 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

maxima_directories_okl := $(maxima_base_installation_dir_okl) $(maxima_base_build_dir_okl) $(maxima_base_doc_dir_okl) $(maxima_doc_dir_okl)

.PHONY : maxima allmaxima cleanmaxima cleanallmaxima cleanallallmaxima

$(maxima_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main maxima targets
# #################################


allmaxima : libsigsegv libffcall clisp gnuplot maxima

# Temporary overwrite to repair six bugs in version 5.16.3
ifeq ($(maxima_recommended_version_number_okl),5.16.3)
maxima : $(maxima_directories_okl)
	$(call unarchive,$(maxima_source_okl),$(maxima_base_build_dir_okl))
	$(call unarchive,$(ExternalSources)/sources/Maxima/rand-mt19937.lisp,$(maxima_build_dir_okl)/src)
	$(call unarchive,$(ExternalSources)/sources/Maxima/nset.lisp,$(maxima_build_dir_okl)/src)
	$(call unarchive,$(ExternalSources)/sources/Maxima/macsys.lisp,$(maxima_build_dir_okl)/src)
	$(call unarchive,$(ExternalSources)/sources/Maxima/grind.lisp,$(maxima_build_dir_okl)/src)
	$(call unarchive,$(ExternalSources)/sources/Maxima/gf.mac,$(maxima_build_dir_okl)/share/contrib/gf)
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl} $(maxima_lisp_configuration_okl); $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	sed -e "s|--remember maxima|--remember ${maxima_installation_dir_okl}/bin/maxima|" ${maxima_installation_dir_okl}/bin/rmaxima > ${maxima_installation_dir_okl}/bin/temp; mv  ${maxima_installation_dir_okl}/bin/temp  ${maxima_installation_dir_okl}/bin/rmaxima; chmod u+x ${maxima_installation_dir_okl}/bin/rmaxima; $(postcondition) \
	cp -rf $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/doc/* $(maxima_doc_dir_okl); $(postcondition) \
	cp -f $(maxima_build_dir_okl)/share/contrib/gf/gf_manual.pdf $(maxima_doc_dir_okl); $(postcondition) \
	cd $(maxima_base_doc_dir_okl); tar -xzf $(maxima_source_woollettbook_okl); $(postcondition) \
	cp -f $(maxima_book_source_okl) $(maxima_base_doc_dir_okl)
else
maxima : $(maxima_directories_okl)
	$(call unarchive,$(maxima_source_okl),$(maxima_base_build_dir_okl))
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl} $(maxima_lisp_configuration_okl); $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	cp -rf $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/doc/* $(maxima_doc_dir_okl); $(postcondition) \
	cp -f $(maxima_build_dir_okl)/share/contrib/gf/gf_manual.pdf $(maxima_doc_dir_okl); $(postcondition) \
	cd $(maxima_base_doc_dir_okl); tar -xzf $(maxima_source_woollettbook_okl); $(postcondition) \
	cp -f $(maxima_book_source_okl) $(maxima_base_doc_dir_okl)
endif


# #################################
# Cleaning
# #################################

cleanmaxima :
	-rm -rf $(maxima_base_build_dir_okl)

cleanallmaxima : cleanmaxima
	-rm -rf $(maxima_base_installation_dir_okl) $(maxima_base_doc_dir_okl)

cleanallallmaxima : cleanalllibsigsegv cleanalllibffcall cleanallclisp cleanallgnuplot cleanallmaxima


# #################################
# Tool gnuplot
###################################

gnuplot_directories_okl := $(gnuplot_base_build_dir_okl) $(gnuplot_base_installation_dir_okl) $(maxima_base_doc_dir_okl)

.PHONY : gnuplot cleangnuplot cleanallgnuplot

$(gnuplot_directories_okl) : % : 
	mkdir -p $@

gnuplot : $(gnuplot_directories_okl)
	$(call unarchive,$(gnuplot_source_okl),$(gnuplot_base_build_dir_okl))
	cd $(gnuplot_build_dir_okl); $(postcondition) \
	./configure --prefix=$(gnuplot_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)
	ln -s --force $(gnuplot_call_okl) $(public_bin_dir_okl)/gnuplot
	cp -f $(gnuplot_pdf_source_okl) $(maxima_base_doc_dir_okl)


cleangnuplot : 
	-rm -rf $(gnuplot_base_build_dir_okl)

cleanallgnuplot : 
	-rm -rf $(gnuplot_base_installation_dir_okl)
