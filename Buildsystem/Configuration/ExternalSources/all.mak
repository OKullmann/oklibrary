# Matthew Henderson, 30.10.2006 (Swansea)

# Configuration variables regarding installation and documentation of external sources

ExternalSources_doc = $(ExternalSources)/doc

include $(OKconfiguration)/ExternalSources/doxygen.mak
include $(OKconfiguration)/ExternalSources/gcc.mak
include $(OKconfiguration)/ExternalSources/r.mak
include $(OKconfiguration)/ExternalSources/ocaml.mak
include $(OKconfiguration)/ExternalSources/coq.mak

# #################################
# Boost
# #################################

boost_recommended_version_number = 1_34_1
boost_supported_not_recommended_version_numbers = 1_34_0
boost_supported_version_numbers = $(boost_supported_not_recommended_version_numbers) $(boost_recommended_version_number)

boost_prefix = boost
boost_recommended_package_name = $(boost_recommended_version_number)
# OK: The variable boost_recommended_package_name seems wrong, and seems
# to be used only for setting boost_html_documentation_index_location ??
boost_html_documentation_index_location = $(ExternalSources_doc)/Boost/$(boost_recommended_package_name)/index.htm

boost_homepage_url := http://www.boost.org/
boost_documentation_url := http://www.boost.org/libs/libraries.htm

boost_targets_prefix := $(boost_prefix)-
boost_prefix_underscore := $(boost_prefix)_
# OK: where do we use boost_prefix_underscore ??
boost_targets := $(addprefix $(boost_targets_prefix), $(boost_supported_version_numbers))
boost_recommended := $(boost_targets_prefix)$(boost_recommended_version_number)

ifndef Boost
  ifdef BOOST
    Boost = -I$(BOOST)
  else
    Boost = -I/usr/local/$(boost_recommended)
  endif
endif
# TODO: We must update and systematise the use of such variables!

# #################################
# Mhash
# #################################

mhash_recommended_version_number = 0.9.9
mhash_supported_not_recommended_version_numbers = 0.9.7.1
mhash_supported_version_numbers = $(mhash_supported_not_recommended_version_numbers) $(mhash_recommended_version_number)

mhash_prefix = mhash

mhash_homepage_url := http://mhash.sourceforge.net/
mhash_documentation_url := http://mhash.sourceforge.net/mhash.3.html

mhash_targets_prefix := $(mhash_prefix)-
mhash_targets := $(addprefix $(mhash_targets_prefix), $(mhash_supported_version_numbers))
# for example "mhash-0.9.7.1 mhash-0.9.9"
mhash_recommended := $(mhash_targets_prefix)$(mhash_recommended_version_number)
# for example "mhash-0.9.9"

ifndef Mhash
  ifdef MHASH
    Mhash_dir := $(MHASH)
  else
    Mhash_dir := $(ExternalSources)/Mhash
  endif
  Mhash := $(Mhash_dir)/$(mhash_recommended_version_number)
endif
# TODO: We must update and systematise the use of such variables!

# #################################
# Postgresql
# #################################

postgresql_recommended_version_number = 8.2.4
postgresql_supported_not_recommended_version_numbers = 
postgresql_supported_version_numbers = $(postgresql_supported_not_recommended_version_numbers) $(postgresql_recommended_version_number)

postgresql_prefix = postgresql
pgsql_html_documentation_index_location = $(ExternalSources_doc)/Postgresql/doc/postgresql/html/index.html

pgsql_homepage_url := http://www.postgresql.org/
pgsql_documentation_url := http://www.postgresql.org/docs/

postgresql_targets_prefix := $(postgresql_prefix)-
postgresql_targets := $(addprefix $(postgresql_targets_prefix), $(postgresql_supported_version_numbers))
postgresql_recommended := $(postgresql_targets_prefix)$(postgresql_recommended_version_number)

# #################################
# Valgrind
# #################################

valgrind_recommended_version_number = 3.2.3
valgrind_supported_not_recommended_version_numbers = 3.2.1
# OK: shouldn't this be 3.2.2 ? Or perhaps just empty?
valgrind_supported_version_numbers = $(valgrind_supported_not_recommended_version_numbers) $(valgrind_recommended_version_number)

valgrind_prefix = valgrind

valgrind_homepage_url := http://valgrind.org/
valgrind_documentation_url := http://valgrind.org/docs/manual/index.html

valgrind_targets_prefix := $(valgrind_prefix)-
valgrind_targets := $(addprefix $(valgrind_targets_prefix),$(valgrind_supported_version_numbers))
valgrind_recommended := $(valgrind_targets_prefix)$(valgrind_recommended_version_number)

# ################################
# UBCSAT
# ###############################

ubcsat_recommended_version_number = 1-0-0
ubcsat_supported_but_not_recommended_version_numbers = 
ubcsat_supported_version_numbers = $(ubcsat_recommended_version_number) $(ubcsat_supported_but_not_recommended_version_numbers)

ubcsat_prefix = ubcsat

ubcsat_homepage_url := http://www.satlib.org/ubcsat/
ubcsat_documentation_url := http://www.satlib.org/ubcsat/\#documentation

ubcsat_targets_prefix := $(ubcsat_prefix)-
ubcsat_targets := $(addprefix $(ubcsat_targets_prefix), $(ubcsat_supported_version_numbers))
ubcsat_recommended := $(ubcsat_targets_prefix)$(ubcsat_recommended_version_number)

ifndef Ubcsat
  ifdef UBCSAT
    Ubcsat_dir = $(UBCSAT)
  else
    Ubcsat_dir = $(ExternalSources)/Ubcsat
  endif
  Ubcsat = -I$(Ubcsat_dir)/$(ubcsat_recommended_version_number)/src
endif
# TODO: We must update and systematise the use of such variables.

# ################################
# Asciidoc
# ###############################

asciidoc_recommended_version_number = 8.2.1
asciidoc_supported_version_numbers = $(asciidoc_recommended_version_number)

asciidoc_prefix = asciidoc

asciidoc_targets_prefix := $(asciidoc_prefix)-
asciidoc_targets := $(addprefix $(asciidoc_targets_prefix), $(asciidoc_supported_version_numbers))
asciidoc_recommended := $(asciidoc_targets_prefix)$(asciidoc_recommended_version_number)

# ################################
# Git
# ###############################

git_recommended_version_number = 1.5.2.4
git_supported_version_numbers = $(git_recommended_version_number) 1.5.2.3

git_prefix = git
git_html_documentation_index_location = $(ExternalSources_doc)/Git/Documentation/git.html

git_homepage_url := http://git.or.cz/
git_documentation_url := http://git.or.cz/\#documentation

git_targets_prefix := $(git_prefix)-
git_targets := $(addprefix $(git_targets_prefix), $(git_supported_version_numbers))
git_recommended := $(git_targets_prefix)$(git_recommended_version_number)

# ################################
# Cmake
# ###############################

cmake_recommended_version_number = 2.4.7
cmake_supported_version_numbers = $(cmake_recommended_version_number)

cmake_prefix = cmake

cmake_homepage_url := http://www.cmake.org/HTML/Index.html
cmake_documentation_url := http://www.cmake.org/HTML/Documentation.html

cmake_targets_prefix := $(cmake_prefix)-
cmake_targets := $(addprefix $(cmake_targets_prefix), $(cmake_supported_version_numbers))
cmake_recommended := $(cmake_targets_prefix)$(cmake_recommended_version_number)

# ################################
# mpfr
# ###############################

mpfr_recommended_version_number = 2.2.1
mpfr_supported_version_numbers = $(mpfr_recommended_version_number)

mpfr_prefix = mpfr

mpfr_targets_prefix := $(mpfr_prefix)-
mpfr_targets := $(addprefix $(mpfr_targets_prefix), $(mpfr_supported_version_numbers))
mpfr_recommended := $(mpfr_targets_prefix)$(mpfr_recommended_version_number)

# ################################
# gmp
# ###############################

gmp_recommended_version_number = 4.2.1
gmp_supported_version_numbers = $(gmp_recommended_version_number)

gmp_prefix = gmp

gmp_homepage_url := http://gmplib.org/
gmp_documentation_url := http://gmplib.org/\#DOC

gmp_targets_prefix := $(gmp_prefix)-
gmp_targets := $(addprefix $(gmp_targets_prefix), $(gmp_supported_version_numbers))
gmp_recommended := $(gmp_targets_prefix)$(gmp_recommended_version_number)

# ################################
# Sage
# ###############################

sage_recommended_version_number = 2.6
sage_supported_not_recommended_version_numbers = 2.7
sage_supported_version_numbers = $(sage_supported_not_recommended_version_numbers) $(sage_recommended_version_number)

sage_prefix = sage
sage_recommended_package_name = $(sage_prefix)-$(sage_recommended_version_number)
sage_html_template = $(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation/Sage.html
sage_html_output = $(local_html_dir)/Sage.html
sage_html_documentation_index_location = Sage.html

sage_base_directory = $(ExternalSources)/Sage
sage_installation_dir = $(sage_base_directory)/$(sage_recommended_package_name)
sage_main_index = $(sage_installation_dir)/doc/index.html
sage_devel_doc = $(sage_installation_dir)/devel/doc-$(sage_recommended_version_number)/paper-letter

sage_maxima_dir = $(sage_installation_dir)/local/share/maxima
sage_maxima_version = $(notdir $(wildcard $(sage_installation_dir)/local/share/maxima/*))
sage_maxima_html = $(sage_maxima_dir)/$(sage_maxima_version)/doc/html
sage_xmaxima_html = $(sage_maxima_dir)/$(sage_maxima_version)/xmaxima/html
sage_maxima_share = $(sage_maxima_dir)/$(sage_maxima_version)/share

sage_gap_version = $(subst gap-,,$(notdir $(wildcard $(sage_installation_dir)/local/lib/gap-*)))
sage_guava_version = $(subst guava,,$(notdir $(wildcard $(sage_installation_dir)/local/lib/gap-$(sage_gap_version)/pkg/guava*)))
sage_guave_html = $(sage_installation_dir)/local/lib/gap-$(sage_gap_version)/pkg/guava$(sage_guava_version)/htm

sage_clisp_dir = $(sage_installation_dir)/local/share/doc/clisp
sage_clisp_html = $(sage_clisp_dir)/doc

sage_ipython_version = $(subst ipython-,,$(notdir $(wildcard $(sage_installation_dir)/local/share/doc/ipython-*)))
sage_ipython_dir = $(sage_installation_dir)/local/share/doc/ipython-$(sage_ipython_version)

sage_qd_dir = $(sage_installation_dir)/local/share/doc/qd

sage_bzip2_version = $(subst bzip2-,,$(notdir $(wildcard $(sage_installation_dir)/spkg/build/bzip2-*)))
sage_bzip2_dir = $(sage_installation_dir)/spkg/build/bzip2-$(sage_bzip2_version)

sage_homepage_url := http://modular.math.washington.edu/sage/
sage_documentation_url := http://modular.math.washington.edu/sage/documentation.html

sage_targets_prefix := $(sage_prefix)-
sage_targets := $(addprefix $(sage_targets_prefix), $(sage_supported_version_numbers))
sage_recommended := $(sage_targets_prefix)$(sage_recommended_version_number)

