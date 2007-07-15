# Matthew Henderson, 30.10.2006 (Swansea)

# Configuration variables regarding installation and documentation of external sources

ExternalSources_doc = $(ExternalSources)/doc

# #################################
# GCC
# #################################

gcc_recommended_version_number = 4.1.2
gcc_supported_not_recommended_version_numbers = 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.2.0
gcc_old_installation = 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.1.2
gcc_new_installation = 4.2.0
gcc_supported_version_numbers = $(gcc_supported_not_recommended_version_numbers) $(gcc_recommended_version_number)

gcc_prefix = gcc
gcc_targets_prefix = $(gcc_prefix)-
gcc_html_documentation_index_location = $(ExternalSources_doc)/Gcc/$(gcc_recommended_version_number)/html/gcc/index.html

gcc_homepage_url := http://gcc.gnu.org/

gcc_targets := $(addprefix $(gcc_targets_prefix), $(gcc_supported_version_numbers))
gcc_targets_old := $(addprefix $(gcc_targets_prefix), $(gcc_old_installation))
gcc_targets_new := $(addprefix $(gcc_targets_prefix), $(gcc_new_installation))
# OK: temporary hack! The system must be rethought.
gcc_recommended := $(gcc_targets_prefix)$(gcc_recommended_version_number)

# #################################
# Boost
# #################################

boost_recommended_version_number = 1_34_0
boost_supported_not_recommended_version_numbers =
boost_supported_version_numbers = $(boost_supported_not_recommended_version_numbers) $(boost_recommended_version_number)

boost_prefix = boost
boost_html_documentation_index_location = $(ExternalSources_doc)/Boost/$(boost_recommended_package_name)/index.htm

boost_homepage_url := http://www.boost.org/

boost_targets_prefix := $(boost_prefix)-
boost_prefix_underscore := $(boost_prefix)_

boost_targets := $(addprefix $(boost_targets_prefix), $(boost_supported_version_numbers))
boost_recommended := $(boost_targets_prefix)$(boost_recommended_version_number)
boost_recommended_package_name := $(boost_recommended_version_number)

# #################################
# Doxygen
# #################################

doxygen_recommended_version_number = 1.5.2
doxygen_supported_not_recommended_version_numbers = 1.5.1
doxygen_supported_version_numbers = $(doxygen_supported_not_recommended_version_numbers) $(doxygen_recommended_version_number)

# remarks: 
#  doxygen-1.4.6 broken
#  doxygen-1.5.1 correction of 1.5.0

doxygen_prefix = doxygen
doxygen_html_documentation_index_location = $(ExternalSources_doc)/Doxygen/$(doxygen_recommended)/html/index.html

doxygen_homepage_url := http://www.stack.nl/~dimitri/doxygen/

doxygen_targets_prefix := $(doxygen_prefix)-
doxygen_targets := $(addprefix $(doxygen_targets_prefix), $(doxygen_supported_version_numbers))
doxygen_recommended := $(doxygen_targets_prefix)$(doxygen_recommended_version_number)
doxygen_recommended_package_name := $(doxygen_targets_prefix)$(doxygen_recommended_version_number).src

# #################################
# Mhash
# #################################

mhash_recommended_version_number = 0.9.9
mhash_supported_not_recommended_version_numbers = 0.9.7.1
mhash_supported_version_numbers = $(mhash_supported_not_recommended_version_numbers) $(mhash_recommended_version_number)

mhash_prefix = mhash

mhash_homepage_url := http://mhash.sourceforge.net/

mhash_targets_prefix := $(mhash_prefix)-
mhash_targets := $(addprefix $(mhash_targets_prefix), $(mhash_supported_version_numbers))
# for example "mhash-0.9.7.1 mhash-0.9.9"
mhash_recommended := $(mhash_targets_prefix)$(mhash_recommended_version_number)
# for example "mhash-0.9.9"

# #################################
# Postgresql
# #################################

postgresql_recommended_version_number = 8.2.4
postgresql_supported_not_recommended_version_numbers = 
postgresql_supported_version_numbers = $(postgresql_supported_not_recommended_version_numbers) $(postgresql_recommended_version_number)

postgresql_prefix = postgresql
pgsql_html_documentation_index_location = $(ExternalSources_doc)/Postgresql/doc/postgresql/html/index.html

pgsql_homepage_url := http://www.postgresql.org/

postgresql_targets_prefix := $(postgresql_prefix)-
postgresql_targets := $(addprefix $(postgresql_targets_prefix), $(postgresql_supported_version_numbers))
postgresql_recommended := $(postgresql_targets_prefix)$(postgresql_recommended_version_number)

# #################################
# Valgrind
# #################################

valgrind_recommended_version_number = 3.2.3
valgrind_supported_not_recommended_version_numbers = 3.2.1
valgrind_supported_version_numbers = $(valgrind_supported_not_recommended_version_numbers) $(valgrind_recommended_version_number)

valgrind_prefix = valgrind

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

ubcsat_targets_prefix := $(ubcsat_prefix)-
ubcsat_targets := $(addprefix $(ubcsat_targets_prefix), $(ubcsat_supported_version_numbers))
ubcsat_recommended := $(ubcsat_targets_prefix)$(ubcsat_recommended_version_number)

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

git_targets_prefix := $(git_prefix)-
git_targets := $(addprefix $(git_targets_prefix), $(git_supported_version_numbers))
git_recommended := $(git_targets_prefix)$(git_recommended_version_number)

# ################################
# Cmake
# ###############################

cmake_recommended_version_number = 2.4.6
cmake_supported_version_numbers = $(cmake_recommended_version_number)

cmake_prefix = cmake

cmake_targets_prefix := $(cmake_prefix)-
cmake_targets := $(addprefix $(cmake_targets_prefix), $(cmake_supported_version_numbers))
cmake_recommended := $(cmake_targets_prefix)$(cmake_recommended_version_number)

# ################################
# R
# ###############################

R_recommended_version_number = 2.5.0
R_supported_version_numbers = $(R_recommended_version_number)

R_prefix = R

R_targets_prefix := $(R_prefix)-
R_targets := $(addprefix $(R_targets_prefix), $(R_supported_version_numbers))
R_recommended := $(R_targets_prefix)$(R_recommended_version_number)

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

gmp_targets_prefix := $(gmp_prefix)-
gmp_targets := $(addprefix $(gmp_targets_prefix), $(gmp_supported_version_numbers))
gmp_recommended := $(gmp_targets_prefix)$(gmp_recommended_version_number)

