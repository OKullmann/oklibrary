# Creating an index in $(system_directories)/doc to all the OKplatform
# documentation, including external resources. This index also contains
# links to the web pages of Boost, Doxygen, Gcc etc

# ##################################################
# Local Documentation Locations
# ##################################################

OKlibrary_html_documentation_index_location := $(doc_dir)/html/index.html
doxygen_html_documentation_index_location := $(OKplatform)/ExternalSources/doc/Doxygen/$(doxygen_recommended)/html/index.html
boost_html_documentation_index_location := $(OKplatform)/ExternalSources/doc/Boost/$(boost_recommended)/index.htm
pgsql_html_documentation_index_location := $(OKplatform)/ExternalSources/doc/Postgresql/doc/postgresql/html/index.html
gcc_man_page_location := $(OKplatform)/ExternalSources/doc/Gcc/$(gcc_recommended)/man/man1/gcc.1

# ##################################################
# Local Documentation Link Elements
# ##################################################

OKlibrary_html_index_link_element:= "<a href=\"$(OKlibrary_html_documentation_index_location)\">OKlibrary</a>"
doxygen_html_index_link_element := "<a href=\"$(doxygen_html_documentation_index_location)\">Doxygen</a>"
boost_html_index_link_element := "<a href=\"$(boost_html_documentation_index_location)\">Boost</a>"
gcc_man_page_link_element := "<a href=\"$(gcc_man_page_location)\">Gcc</a>"
pgsql_html_index_link_element := "<a href=\"$(pgsql_html_documentation_index_location)\">Postgresql</a>"

list_of_local_index_link_elements := $(OKlibrary_html_index_link_element) $(boost_html_index_link_element) $(doxygen_html_index_link_element) $(gcc_man_page_link_element) $(pgsql_html_index_link)

# ##################################################
# WWW Homepage URLs
# ##################################################

boost_homepage_url := http://www.boost.org/
doxygen_homepage_url := http://www.stack.nl/~dimitri/doxygen/
gcc_homepage_url := http://gcc.gnu.org/
pgsql_homepage_url := http://www.postgresql.org/
mhash_homepage_url := http://mhash.sourceforge.net/

# ##################################################
# WWW Homepage Link Elements
# ##################################################

boost_homepage_link_element := "<a href=\"$(boost_homepage_url)\">Boost</a>"
doxygen_homepage_link_element := "<a href=\"$(doxygen_homepage_url)\">Doxygen</a>"
gcc_homepage_link_element := "<a href=\"$(gcc_homepage_url)\">Gcc</a>"
pgsql_homepage_link_element := "<a href=\"$(pgsql_homepage_url)\">Postgresql</a>"
mhash_homepage_link_element := "<a href=\"$(mhash_homepage_url)\">Mhash</a>"

list_of_homepage_link_elements := $(boost_homepage_link_element) $(doxygen_homepage_link_element) $(gcc_homepage_link_element) $(pgsql_homepage_link_element) $(mhash_homepage_link_element)

# ##################################################
# Head
# ##################################################

title_element := "<title>OKlibrary Documentation</title>"
stylesheet_location := $(OKbuildsystem)/makefile_generic_include/documentation/documentation_index_resources/documentation_index_style.css
style_element := "<link rel=\"stylesheet\" type=\"text.css\" href=\"$(stylesheet_location)\""
documentation_index_head := $(title_element) $(style_element)

# ##################################################
# Local Documentation Index Division
# ##################################################

local_index_division := "<div id=\"local_index\"> <h2>Local Documentation</h2> "$(list_of_local_index_link_elements)"</div>"

# ##################################################
# WWW Homepage Index Division
# ##################################################

homepage_division := "<div id=\"homepages\"><h2>WWW</h2> "$(list_of_homepage_link_elements)" </div>"

# ##################################################
# Logo Division
# ##################################################

logo_image_location := $(OKbuildsystem)/makefile_generic_include/documentation/documentation_index_resources/logo.png
logo_image := "<img src=\"$(logo_image_location)\"/>"
logo_division := "<div id=\"logo\"> "$(logo_image)" </div>"

# ##################################################
# Version Number Division
# ##################################################

version_number_element := "v0.0.1"
version_number_division := "<div id=\"vn\"> "$(version_number_element)" </div> "
# ##################################################
# Body
# ##################################################

documentation_index_body := $(logo_division) $(version_number_division) $(local_index_division) $(homepage_division)

# ##################################################
# Main target for creating index
# ##################################################

documentation_index_file := $(doc_dir)/index.html
documentation_index_html := "<html><head>"$(documentation_index_head)"</head><body>"$(documentation_index_body)"</body></html>"

documentation_index : | $(doc_dir)
	@touch $(documentation_index_file)
	@echo $(documentation_index_html) > $(documentation_index_file) 
