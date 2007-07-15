# Oliver Kullmann, 15.7.2007 (Swansea)

# Configuration data regarding the local html-pages (not doxygen)

local_html_dir = $(doc_dir)/local_html

internet_html_homepage = unknown

local_home_page_template = $(OKbuildsystem)/Html/Local/HomePage.html
local_home_page_output = $(local_html_dir)/index.html

aims_page_template = $(OKbuildsystem)/Html/Local/Aims.html
aims_page_output = $(local_html_dir)/Aims.html

history_page_template = $(OKbuildsystem)/Html/Local/History.html
history_page_output = $(local_html_dir)/History.html

logo_template = $(OKbuildsystem)/Html/Local/logo.png
logo_output = $(local_html_dir)/logo.png

developers_template = $(OKconfiguration)/Developers.html
developers_output = $(local_html_dir)/Developers.html

current_date = $(shell date --rfc-2822)

