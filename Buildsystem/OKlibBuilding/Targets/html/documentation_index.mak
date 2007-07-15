# Creating an index in $(system_directories)/doc to all the OKplatform
# documentation, including external resources. This index also contains
# links to the web pages of Boost, Doxygen, Gcc etc

# ##################################################
# OKlibrary Documentation Location
# ##################################################

OKlibrary_full_documentation_dvi_location := $(doc_dir)/dvi/FullDocumentation.dvi

# ##################################################
# OKlibrary Documentation Link Elements
# ##################################################

OKlibrary_html_index_link_element:= "<a href=\"$(OKlibrary_html_documentation_index_location)\" target=\"_blank\">Reference (html)</a>"

OKlibrary_full_documentation_dvi_link_element:= "<a href=\"$(OKlibrary_full_documentation_dvi_location)\">Manual (dvi)</a>"

list_of_OKlibrary_documentation_link_elements:= $(OKlibrary_html_index_link_element) \
                                                $(OKlibrary_full_documentation_dvi_link_element)

# ##################################################
# Local Documentation Link Elements
# ##################################################

boost_html_index_link_element := "<a href=\"$(boost_html_documentation_index_location)\" target=\"_blank\">Boost</a>"
doxygen_html_index_link_element := "<a href=\"$(doxygen_html_documentation_index_location)\" target=\"_blank\">Doxygen</a>"
gcc_page_link_element := "<a href=\"$(gcc_html_documentation_index_location)\" target=\"_blank\">Gcc</a>"
git_html_index_link_element := "<a href=\"$(git_html_documentation_index_location)\" target=\"_blank\">Git</a>"
pgsql_html_index_link_element := "<a href=\"$(pgsql_html_documentation_index_location)\" target=\"_blank\">Postgresql</a>"

list_of_local_index_link_elements :=  $(boost_html_index_link_element) \
                                      $(doxygen_html_index_link_element) \
                                      $(gcc_page_link_element) \
                                      $(git_html_index_link_element) \
                                      $(pgsql_html_index_link_element)


# ##################################################
# WWW Homepage Link Elements
# ##################################################

boost_homepage_link_element := "<a href=\"$(boost_homepage_url)\" target=\"_blank\">Boost</a>"
doxygen_homepage_link_element := "<a href=\"$(doxygen_homepage_url)\" target=\"_blank\">Doxygen</a>"
gcc_homepage_link_element := "<a href=\"$(gcc_homepage_url)\" target=\"_blank\">Gcc</a>"
git_homepage_link_element := "<a href=\"$(git_homepage_url)\" target=\"_blank\">Git</a>"
pgsql_homepage_link_element := "<a href=\"$(pgsql_homepage_url)\" target=\"_blank\">Postgresql</a>"
mhash_homepage_link_element := "<a href=\"$(mhash_homepage_url)\" target=\"_blank\">Mhash</a>"

list_of_homepage_link_elements := $(boost_homepage_link_element) \
                                  $(doxygen_homepage_link_element) \
                                  $(gcc_homepage_link_element) \
                                  $(git_homepage_link_element) \
                                  $(pgsql_homepage_link_element) \
                                  $(mhash_homepage_link_element)

# ##################################################
# Head
# ##################################################

title_element := "<title>OKlibrary Documentation</title>"
stylesheet_location := $(OKbuildsystem)/Html/Local/documentation_index_style.css
style_element := "<link rel=\"stylesheet\" type=\"text.css\" href=\"$(stylesheet_location)\""
documentation_index_head := $(title_element) $(style_element)

# ##################################################
# OKlibrary Documentation Division
# ##################################################

OKlibrary_documentation_division := "<div id=\"OKlibrary_doc\"><h4>OKlibrary Documentation</h4>"$(list_of_OKlibrary_documentation_link_elements)"</div>"

# ##################################################
# Local Documentation Index Division
# ##################################################

local_index_division := "<div id=\"local_index\"><h4>Local Packages Documentation</h4> "$(list_of_local_index_link_elements)"</div>"

# ##################################################
# WWW Homepage Index Division
# ##################################################

homepage_division := "<div id=\"homepages\"><h4>WWW Packages Homepages</h4> "$(list_of_homepage_link_elements)" </div>"

# ##################################################
# Wrapper division for local and WWW documentation
# ##################################################

external_documentation_division := "<div id=\"external_doc\">"$(local_index_division) $(homepage_division)"</div>"

# ##################################################
# Logo Division
# ##################################################

logo_image_location := $(OKbuildsystem)/Html/Local/logo.png
logo_division := "<div id=\"logo\"><img src=\"$(logo_image_location)\"/> </div>"

# ##################################################
# Subtitle
# ##################################################

subtitle := "<h3>An algorithmic platform for efficient satisfiability-based problem solving</h3>"

# ##################################################
# Footer division
# ##################################################

footer_division := "<div id=\"footer\"></div>"

# ##################################################
# Documentation Wrapper Division
# ##################################################


documentation_navigation := "<div id=\"doc_navigation\"> "$(OKlibrary_documentation_division)"  "$(external_documentation_division)"  </div>"

# ##################################################
# Body
# ##################################################

documentation_index_body := "<div id=\"content\"> "$(logo_division)" \
                            "$(subtitle)" \
                            "$(documentation_navigation)" \
                            "$(footer_division)" </div>"

# ##################################################
# Full document
# ##################################################

documentation_index_html := "<html><head>"$(documentation_index_head)"</head><body>"$(documentation_index_body)"</body></html>"

# ##################################################
# Main target for creating index
# ##################################################

documentation_index_file := $(doc_dir)/index.html

# temporary (OK):
.PHONY : new_documentation_index
documentation_index : new_documentation_index | $(doc_dir)
	@touch $(documentation_index_file)
	@echo $(documentation_index_html) > $(documentation_index_file) 

new_documentation_index : $(local_html_dir)
	$(preprocessing_call) $(local_home_page_template) > $(local_home_page_output)
	$(preprocessing_call) $(aims_page_template) > $(aims_page_output)
	$(preprocessing_call) $(history_page_template) > $(history_page_output)
	cp $(logo_template) $(logo_output)
	cp $(developers_template) $(developers_output)
