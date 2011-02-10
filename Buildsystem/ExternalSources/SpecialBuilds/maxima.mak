# Oliver Kullmann, 24.12.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# ##################################
# Directory Structure
# ################################## 

maxima_directories_okl := $(maxima_base_installation_dir_okl) $(maxima_base_build_dir_okl) $(maxima_base_doc_dir_okl) $(maxima_doc_dir_okl) $(maxima_eis_base_installation_dir_okl)

.PHONY : maxima maxima_core maxima_eis allmaxima cleanmaxima cleanallmaxima cleanallallmaxima

$(maxima_directories_okl) : % : 
	mkdir -p $@

# #################################
# Main maxima targets
# #################################


ifeq ($(maxima_lisp_name_okl),clisp)
allmaxima : clispall gnuplot maxima
else
allmaxima : ecl gnuplot maxima
endif

maxima : maxima_core maxima_eis

maxima_core : $(maxima_directories_okl)
	$(call unarchive,$(maxima_source_okl),$(maxima_base_build_dir_okl))
	cd $(maxima_build_dir_okl); $(postcondition) \
	LANG=C ./configure --prefix=${maxima_installation_dir_okl} $(maxima_lisp_configuration_okl); $(postcondition) \
	LANG=C make; $(postcondition) \
	make check; $(postcondition) \
	make install; $(postcondition) \
	sed -e "s|--remember maxima|--remember ${maxima_installation_dir_okl}/bin/maxima|" ${maxima_installation_dir_okl}/bin/rmaxima > ${maxima_installation_dir_okl}/bin/temp; mv  ${maxima_installation_dir_okl}/bin/temp  ${maxima_installation_dir_okl}/bin/rmaxima; chmod u+x ${maxima_installation_dir_okl}/bin/rmaxima; $(postcondition) \
	cp -rf $(maxima_base_share_dir_okl)/doc/* $(maxima_doc_dir_okl); $(postcondition) \
	cp -f $(maxima_share_dir_okl)/contrib/gf/gf_manual.pdf $(maxima_doc_dir_okl); $(postcondition) \
	cd $(maxima_base_doc_dir_okl); tar -xzf $(maxima_source_woollettbook_okl); $(postcondition) \
	cp -f $(maxima_book_source_okl) $(maxima_base_doc_dir_okl); $(postcondition)
	cd $(maxima_homedir_okl); $(postcondition) \
	ln -s ~/.Xauthority; $(postcondition)


# For access to the "encyclopedia of integer sequences":
maxima_eis : $(maxima_eis_base_installation_dir_okl)
	$(call unarchive,$(maxima_source_dir_okl)/$(maxima_eis_package_name_okl),$(maxima_eis_base_installation_dir_okl))
	$(call unarchive,$(maxima_source_dir_okl)/$(maxima_eis_data_name_okl),$(maxima_eis_base_installation_dir_okl))
	cd $(maxima_eis_installation_dir_okl); cat eis.lisp | awk '{if ($$1 == "(defparameter" && $$2 == "*path-base*") print "(defparameter *path-base* \"$(maxima_eis_base_installation_dir_okl)\")"; else print}' > eistemp; mv eistemp eis.lisp; $(postcondition)


# #################################
# Cleaning
# #################################

cleanmaxima :
	-rm -rf $(maxima_base_build_dir_okl)

cleanallmaxima : cleanmaxima
	-rm -rf $(maxima_base_installation_dir_okl) $(maxima_base_doc_dir_okl)

cleanallallmaxima : cleanallecl cleanallgnuplot cleanallmaxima


# #################################
# Tool gnuplot
###################################

gnuplot_directories_okl := $(gnuplot_base_build_dir_okl) $(gnuplot_base_installation_dir_okl)

.PHONY : gnuplot cleangnuplot cleanallgnuplot

$(gnuplot_directories_okl) : % : 
	mkdir -p $@

gnuplot : $(gnuplot_directories_okl) $(maxima_base_doc_dir_okl)
	$(call unarchive,$(gnuplot_source_okl),$(gnuplot_base_build_dir_okl))
	cd $(gnuplot_build_dir_okl); $(postcondition) \
	LDFLAGS="$(gcc_linking_okl)" ./configure --prefix=$(gnuplot_installation_dir_okl); $(postcondition) \
	make; $(postcondition) \
	make install; $(postcondition)
	ln -s --force $(gnuplot_call_okl) $(public_bin_dir_okl)/gnuplot
	cp -f $(gnuplot_pdf_source_okl) $(maxima_base_doc_dir_okl)


cleangnuplot : 
	-rm -rf $(gnuplot_base_build_dir_okl)

cleanallgnuplot : 
	-rm -rf $(gnuplot_base_installation_dir_okl)
