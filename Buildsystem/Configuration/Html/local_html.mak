# Oliver Kullmann, 15.7.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Configuration data regarding the local html-pages (mostly not doxygen)

# OK: According to the name of this file, the Internet-related content
# should be put into another file (internet_html.mak?) ?

# The directory where to put files to be put on ok-sat-library.org
# is $(internet_dir); if this variable is needed, then it is specified
# by the override-file.

local_home_page_template ?= $(OKbuildsystem)/Html/Local/HomePage.html
local_home_page_output ?= $(local_html_dir)/index.html
local_home_page_css_template ?= $(OKbuildsystem)/Html/Local/HomePage.css
local_home_page_css_output ?= $(local_html_dir)/HomePage.css


aims_page_template ?= $(OKbuildsystem)/Html/Local/Aims.html
aims_page_output ?= $(local_html_dir)/Aims.html

languages_page_template ?= $(OKbuildsystem)/Html/Local/ProgrammingLanguages.html
languages_page_output ?= $(local_html_dir)/ProgrammingLanguages.html

history_page_template ?= $(OKbuildsystem)/Html/Local/History.html
history_page_output ?= $(local_html_dir)/History.html

packagehistory_page_template ?= $(OKbuildsystem)/Html/Local/PackageHistory.html
packagehistory_page_output ?= $(local_html_dir)/PackageHistory.html

licence_page_template ?= $(OKbuildsystem)/Html/Local/Licence.html
licence_page_output ?= $(local_html_dir)/Licence.html

tutorial_page_template ?= $(OKbuildsystem)/Html/Local/Tutorial.html
tutorial_page_output ?= $(local_html_dir)/Tutorial.html

faq_page_template ?= $(OKbuildsystem)/Html/Local/FAQ.html
faq_page_output ?= $(local_html_dir)/FAQ.html

externalsources_page_template ?= $(OKbuildsystem)/Html/Local/ExternalSources.html
externalsources_page_output ?= $(local_html_dir)/ExternalSources.html

logo_template ?= $(OKbuildsystem)/Html/Local/logo.png
logo_output ?= $(local_html_dir)/logo.png

developers_template ?= $(OKconfiguration)/Developers.html
developers_output ?= $(local_html_dir)/Developers.html

tenchallenges_page_template ?= $(OKbuildsystem)/Html/Local/TenChallenges.html
tenchallenges_page_output ?= $(local_html_dir)/TenChallenges.html
ramseytheory_page_template ?= $(OKbuildsystem)/Html/Local/RamseyTheory.html
ramseytheory_page_output ?= $(local_html_dir)/RamseyTheory.html


tenchallenges_doxygen_directory ?= $(doc_dir)/doxygen_html/dir_1ea18a24b3992f33b9d5066b2a2464fc.html


current_date ?= $(shell date '+%a, %d %b %Y, %H:%M:%S %z')


# Internet home page:

internet_home_page_template ?= $(OKbuildsystem)/Html/Internet/HomePage.html
internet_home_page_output_dir ?= $(internet_html_dir)
internet_home_page_output ?= $(internet_home_page_output_dir)/index.html

# For Internet-domains see Configuration/public_links.mak.


# Doxygen pages of global interest:

main_docu_page ?= $(doc_dir)/doxygen_html/d5/d70/OKlib_2docus_2general_8hpp.html
computeralgebra_docu_page_okl ?= $(doc_dir)/doxygen_html/d4/de2/OKlib_2ComputerAlgebra_2docus_2general_8hpp.html
usermaxima_docu_page_okl ?= $(doc_dir)/doxygen_html/d0/d69/ComputerAlgebra_2docus_2Maxima_8hpp.html


main_demo_page ?= $(doc_dir)/doxygen_html/de/d71/OKlib_2demos_2general_8hpp.html

licence_docu_page ?= $(doc_dir)/doxygen_html/d2/d59/OKlib_2System_2LegalIssues_2docus_2general_8hpp.html


sat2011competition_okl ?= $(doc_dir)/doxygen_html/d3/d83/SAT2011_8hpp.html
