documentation_index_file := $(doc_dir)/index.html

OKlibrary_html_documentation_index := $(doc_dir)/html/index.html
doxygen_html_documentation_index_location := $(OKplatform)/ExternalSources/Doxygen/doxygen-$$(doxygen --version)/html/index.html
boost_html_documentation_index_location := $(OKplatform)/ExternalSources/Boost/boost_$(alternative_boost)/index.htm
pgsql_html_documentation_index_html := $(OKplatform)/ExternalSources/Postgresql/doc/postgresql/html/index.html

gcc_man_page_location := $(OKplatform)/ExternalSources/Gcc/$(alternative_gcc)/man/man1/gcc.1

boost_homepage_url := http://www.boost.org/
doxygen_homepage_url := http://www.stack.nl/~dimitri/doxygen/
gcc_homepage_url := http://gcc.gnu.org/
pgsql_homepage_url := http://www.postgresql.org/
mhash_homepage_url := http://mhash.sourceforge.net/

boost_homepage_link := "<a href=\"$(boost_homepage_url)\">Boost</a>"
doxygen_homepage_link := "<a href=\"$(doxygen_homepage_url)\">Doxygen</a>"
gcc_homepage_link := "<a href=\"$(gcc_homepage_url)\">Gcc</a>"
pgsql_homepage_link := "<a href=\"$(pgsql_homepage_url)\">Postgresql</a>"
mhash_homepage_link := "<a href=\"$(mhash_homepage_url)\">Mhash</a>"

OKlibrary_html_index_link:= "<a href=\"$(OKlibrary_html_documentation_index)\">OKlibrary</a>"
doxygen_html_index_link := "<a href=\"$(doxygen_html_documentation_index_location)\">Doxygen</a>"
boost_html_index_link := "<a href=\"$(boost_html_documentation_index_location)\">Boost</a>"
gcc_man_page_link := "<a href=\"$(gcc_man_page_location)\">Gcc</a>"
pgsql_html_index_link := "<a href=\"$(pgsql_html_documentation_index_html)\">Postgresql</a>"

title_element := "<title>OKlibrary Documentation</title>"
stylesheet_location := $(OKBuildsystem)/makefile_generic_include/documentation/documentation_index_resources/documentation_index_style.css
style_element := "<link rel=\"stylesheet\" type=\"text.css\" href=\"$(stylesheet_location)\""
documentation_index_head := $(title_element) $(style_element)

local_index_list := $(OKlibrary_html_index_link) $(boost_html_index_link) $(doxygen_html_index_link) $(gcc_man_page_link) $(pgsql_html_index_link)
local_index_division := "<div id=\"local_index\"> <h2>Local Documentation</h2> "$(local_index_list)"</div>"

homepage_list := $(boost_homepage_link) $(doxygen_homepage_link) $(gcc_homepage_link) $(pgsql_homepage_link) $(mhash_homepage_link)
homepage_division := "<div id=\"homepages\"><h2>WWW</h2> "$(homepage_list)" </div>"

logo_image_location := $(OKBuildsystem)/makefile_generic_include/documentation/documentation_index_resources/logo.png
logo_image := "<img src=\"$(logo_image_location)\"/>"
logo_division := "<div id=\"logo\"> "$(logo_image)" </div>"

version_number_element := "v0.0.1"
version_number_division := "<div id=\"vn\"> "$(version_number_element)" </div> "

documentation_index_body := " "$(logo_division)" "$(version_number_division)" "$(local_index_division)" "$(homepage_division)"  "

documentation_index_html := "<html><head>"$(documentation_index_head)"</head><body>"$(documentation_index_body)"</body></html>"

documentation_index : | $(doc_dir)
	@touch $(documentation_index_file)
	@echo $(documentation_index_html) > $(documentation_index_file) 
