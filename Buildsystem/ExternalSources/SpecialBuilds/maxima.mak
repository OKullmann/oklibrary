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

.PHONY : maxima allmaxima cleanallmaxima

$(maxima_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main maxima targets
# #################################


allmaxima : libsigsegv libffcall clisp gnuplot maxima

ifneq ($(maxima_recommended_version_number_okl),5.15.0)
maxima : $(maxima_directories_okl)
	$(call unarchive,$(maxima_source_okl),$(maxima_base_build_dir_okl))
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl} --with-clisp=$(clisp_call_okl) --with-clisp-runtime=$(clisp_lib_okl); $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	cp -r $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/doc/* $(maxima_doc_dir_okl); $(postcondition) \
	cp $(maxima_book_source_okl) $(maxima_base_doc_dir_okl)
else
# Temporary overwrite to repair a bug in version 5.15.0
maxima : $(maxima_directories_okl)
	$(call unarchive,$(maxima_source_okl),$(maxima_base_build_dir_okl))
	rm -r $(maxima_build_dir_okl)/share/contrib/graphs
	$(call unarchive,$(ExternalSources)/sources/Maxima/graphs,$(maxima_build_dir_okl)/share/contrib)
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl} --with-clisp=$(clisp_call_okl) --with-clisp-runtime=$(clisp_lib_okl); $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	cp $(maxima_build_dir_okl)/share/contrib/graphs/dijkstra.lisp $(maxima_installation_dir_okl)/share/maxima/5.15.0/share/contrib/graphs
	cp -r $(maxima_installation_dir_okl)/share/maxima/$(maxima_recommended_version_number_okl)/doc/* $(maxima_doc_dir_okl); $(postcondition) \
	cp $(maxima_book_source_okl) $(maxima_base_doc_dir_okl)
endif


# #################################
# Cleaning
# #################################

cleanallmaxima : 
	-rm -rf $(maxima_base_installation_dir_okl) $(maxima_base_build_dir_okl) $(maxima_base_doc_dir_okl)

cleanallallmaxima : cleanalllibsigsegv cleanalllibffcall cleanallclisp cleanallgnuplot cleanallmaxima


# #################################
# Tool gnuplot
###################################

gnuplot_directories_okl := $(gnuplot_base_build_dir_okl) $(gnuplot_base_installation_dir_okl)

.PHONY : gnuplot cleanallgnuplot

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


cleanallgnuplot : 
	-rm -rf $(gnuplot_base_build_dir_okl) $(gnuplot_base_installation_dir_okl) 
