# Oliver Kullmann, 15.7.2007 (Swansea)

# Configuration data regarding the local html-pages (not doxygen)

# OK: According to the name of this file, the Internet-related content
# should be put into another file (internet_html.mak?) ?

# The directory where to put files to be put on ok-sat-library.org
# is $(internet_dir); if this variable is needed, then it is specified
# by the override-file.

local_home_page_template ?= $(OKbuildsystem)/Html/Local/HomePage.html
local_home_page_output ?= $(local_html_dir)/index.html

aims_page_template ?= $(OKbuildsystem)/Html/Local/Aims.html
aims_page_output ?= $(local_html_dir)/Aims.html

languages_page_template ?= $(OKbuildsystem)/Html/Local/ProgrammingLanguages.html
languages_page_output ?= $(local_html_dir)/ProgrammingLanguages.html

history_page_template ?= $(OKbuildsystem)/Html/Local/History.html
history_page_output ?= $(local_html_dir)/History.html

externalsources_page_template ?= $(OKbuildsystem)/Html/Local/ExternalSources.html
externalsources_page_output ?= $(local_html_dir)/ExternalSources.html

logo_template ?= $(OKbuildsystem)/Html/Local/logo.png
logo_output ?= $(local_html_dir)/logo.png

developers_template ?= $(OKconfiguration)/Developers.html
developers_output ?= $(local_html_dir)/Developers.html

current_date ?= $(shell date --rfc-2822)

internet_home_page_template ?= $(OKbuildsystem)/Html/Internet/HomePage.html
internet_home_page_output_dir ?= $(internet_html_dir)
internet_home_page_output ?= $(internet_home_page_output_dir)/index.html

oklibrary_domain := http://www.ok-sat-library.org

main_docu_page ?= $(doc_dir)/doxygen_html/d0/dfd/docus_2general_8hpp.html

