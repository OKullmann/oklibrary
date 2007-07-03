# #################################
# GCC Versions
# #################################

gcc_recommended_version_number := 4.1.2
gcc_supported_not_recommended_version_numbers := 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.2.0
gcc_targets_old := 4.0.1 4.0.2 4.0.3 4.1.0 4.1.1 4.1.2
gcc_targets_new := 4.2.0
gcc_supported_version_numbers := $(gcc_supported_not_recommended_version_numbers) $(gcc_recommended_version_number)

# ########################################
# GCC Version-dependent System Variables
# ########################################

gcc_prefix := gcc
gcc_targets_prefix := $(gcc_prefix)-

gcc_targets := $(addprefix $(gcc_targets_prefix), $(gcc_supported_version_numbers))
gcc_targets_old := $(addprefix $(gcc_targets_prefix), $(gcc_targets_old))
gcc_targets_new := $(addprefix $(gcc_targets_prefix), $(gcc_targets_new))
# OK: temporary hack! The system must be rethought.
gcc_recommended := $(gcc_targets_prefix)$(gcc_recommended_version_number)

# #################################
# Boost Versions
# #################################

boost_recommended_version_number := 1_34_0
boost_supported_not_recommended_version_numbers :=
boost_supported_version_numbers := $(boost_supported_not_recommended_version_numbers) $(boost_recommended_version_number)

# #########################################
# Boost Version-dependent System Variables
# #########################################

boost_prefix := boost
boost_targets_prefix := $(boost_prefix)-
boost_prefix_underscore := $(boost_prefix)_
boost_targets := $(addprefix $(boost_targets_prefix), $(boost_supported_version_numbers))
boost_recommended := $(boost_targets_prefix)$(boost_recommended_version_number)
boost_recommended_package_name := $(boost_recommended_version_number)

# #################################
# Doxygen Versions
# #################################

doxygen_recommended_version_number := 1.5.2
doxygen_supported_not_recommended_version_numbers := 1.5.1
doxygen_supported_version_numbers := $(doxygen_supported_not_recommended_version_numbers) $(doxygen_recommended_version_number)

# ###########################################
# Doxygen Version-dependent System Variables
# ###########################################

doxygen_prefix := doxygen
doxygen_targets_prefix := $(doxygen_prefix)-
doxygen_targets := $(addprefix $(doxygen_targets_prefix), $(doxygen_supported_version_numbers))
doxygen_recommended := $(doxygen_targets_prefix)$(doxygen_recommended_version_number)
doxygen_recommended_package_name := $(doxygen_targets_prefix)$(doxygen_recommended_version_number).src

# remarks: 
#  doxygen-1.4.6 broken
#  doxygen-1.5.1 correction of 1.5.0

# #################################
# Mhash Versions
# #################################

mhash_recommended_version_number := 0.9.9
mhash_supported_not_recommended_version_numbers := 0.9.7.1
mhash_supported_version_numbers := $(mhash_supported_not_recommended_version_numbers) $(mhash_recommended_version_number)

# ###########################################
# Mhash Version-dependent System Variables
# ###########################################

mhash_prefix := mhash
mhash_targets_prefix := $(mhash_prefix)-
mhash_targets := $(addprefix $(mhash_targets_prefix), $(mhash_supported_version_numbers))
# for example "mhash-0.9.7.1 mhash-0.9.9"
mhash_recommended := $(mhash_targets_prefix)$(mhash_recommended_version_number)
# for example "mhash-0.9.9"

# #################################
# Postgresql Versions
# #################################

postgresql_recommended_version_number := 8.2.4
postgresql_supported_not_recommended_version_numbers := 
postgresql_supported_version_numbers := $(postgresql_supported_not_recommended_version_numbers) $(postgresql_recommended_version_number)

# ##############################################
# Postgresql Version-dependent System Variables
# ##############################################

postgresql_prefix := postgresql
postgresql_targets_prefix := $(postgresql_prefix)-
postgresql_targets := $(addprefix $(postgresql_targets_prefix), $(postgresql_supported_version_numbers))
postgresql_recommended := $(postgresql_targets_prefix)$(postgresql_recommended_version_number)

# #################################
# Valgrind Versions
# #################################

valgrind_recommended_version_number := 3.2.3
valgrind_supported_not_recommended_version_numbers := 3.2.1
valgrind_supported_version_numbers := $(valgrind_supported_not_recommended_version_numbers) $(valgrind_recommended_version_number)

# ##############################################
# Valgrind Version-dependent System Variables
# ##############################################

valgrind_prefix := valgrind
valgrind_targets_prefix := $(valgrind_prefix)-
valgrind_targets := $(addprefix $(valgrind_targets_prefix),$(valgrind_supported_version_numbers))
valgrind_recommended := $(valgrind_targets_prefix)$(valgrind_recommended_version_number)

# ################################
# UBCSAT Versions
# ###############################

ubcsat_recommended_version_number := 1-0-0
ubcsat_supported_but_not_recommended_version_numbers := 
ubcsat_supported_version_numbers := $(ubcsat_recommended_version_number) $(ubcsat_supported_but_not_recommended_version_numbers)

# ###########################################
# UBCSAT Version-dependent System Variables
# ###########################################

ubcsat_prefix := ubcsat
ubcsat_targets_prefix := $(ubcsat_prefix)-
ubcsat_targets := $(addprefix $(ubcsat_targets_prefix), $(ubcsat_supported_version_numbers))
ubcsat_recommended := $(ubcsat_targets_prefix)$(ubcsat_recommended_version_number)

# ################################
# Asciidoc Versions
# ###############################

asciidoc_recommended_version_number := 8.2.1
asciidoc_supported_version_numbers := $(asciidoc_recommended_version_number)

# ###########################################
# Asciidoc Version-dependent System Variables
# ###########################################

asciidoc_prefix := asciidoc
asciidoc_targets_prefix := $(asciidoc_prefix)-
asciidoc_targets := $(addprefix $(asciidoc_targets_prefix), $(asciidoc_supported_version_numbers))
asciidoc_recommended := $(asciidoc_targets_prefix)$(asciidoc_recommended_version_number)

# ################################
# Git Versions
# ###############################

git_recommended_version_number := 1.5.2.3
git_supported_version_numbers := $(git_recommended_version_number) 1.5.2.2

# ###########################################
# Git Version-dependent System Variables
# ###########################################

git_prefix := git
git_targets_prefix := $(git_prefix)-
git_targets := $(addprefix $(git_targets_prefix), $(git_supported_version_numbers))
git_recommended := $(git_targets_prefix)$(git_recommended_version_number)

# ################################
# Cmake Versions
# ###############################

cmake_recommended_version_number := 2.4.6
cmake_supported_version_numbers := $(cmake_recommended_version_number)

# ###########################################
# Cmake Version-dependent System Variables
# ###########################################

cmake_prefix := cmake
cmake_targets_prefix := $(cmake_prefix)-
cmake_targets := $(addprefix $(cmake_targets_prefix), $(cmake_supported_version_numbers))
cmake_recommended := $(cmake_targets_prefix)$(cmake_recommended_version_number)

# ################################
# R Versions
# ###############################

R_recommended_version_number := 2.5.0
R_supported_version_numbers := $(R_recommended_version_number)

# ###########################################
# R Version-dependent System Variables
# ###########################################

R_prefix := R
R_targets_prefix := $(R_prefix)-
R_targets := $(addprefix $(R_targets_prefix), $(R_supported_version_numbers))
R_recommended := $(R_targets_prefix)$(R_recommended_version_number)

# ################################
# mpfr Versions
# ###############################

mpfr_recommended_version_number := 2.2.1
mpfr_supported_version_numbers := $(mpfr_recommended_version_number)

# ###########################################
# mpfr Version-dependent System Variables
# ###########################################

mpfr_prefix := mpfr
mpfr_targets_prefix := $(mpfr_prefix)-
mpfr_targets := $(addprefix $(mpfr_targets_prefix), $(mpfr_supported_version_numbers))
mpfr_recommended := $(mpfr_targets_prefix)$(mpfr_recommended_version_number)

# ################################
# gmp Versions
# ###############################

gmp_recommended_version_number := 4.2.1
gmp_supported_version_numbers := $(gmp_recommended_version_number)

# ###########################################
# gmp Version-dependent System Variables
# ###########################################

gmp_prefix := gmp
gmp_targets_prefix := $(gmp_prefix)-
gmp_targets := $(addprefix $(gmp_targets_prefix), $(gmp_supported_version_numbers))
gmp_recommended := $(gmp_targets_prefix)$(gmp_recommended_version_number)