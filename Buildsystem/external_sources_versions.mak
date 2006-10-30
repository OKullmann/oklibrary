# #################################
# Gcc Versions
# #################################

gcc_targets := gcc-3.4.3 gcc-3.4.4 gcc-3.4.5 gcc-3.4.6 gcc-4.0.0 gcc-4.0.1 gcc-4.0.2 gcc-4.0.3 gcc-4.1.0 gcc-4.1.1
# remark: gcc-3.4.6 creates linking errors on cs-wsok when compiling
# optimised new test programs (it seems that code for message objects has
# been optimised away); one needs to find out for which other gcc-versions
# we also have this problem (at least with gcc-4.0.3 the problem seems
# to be solved).
gcc_recommended := gcc-4.1.1
gcc_recommended_version_number := 4.1.1
 
# #################################
# Boost Versions
# #################################

boost_targets := boost-1_33_1
boost_recommended := boost-1_33_1
boost_recommended_version_number := 1_33_1

# #################################
# Doxygen Versions
# #################################

doxygen_targets := doxygen-1.4.7 doxygen-1.5.0
doxygen_recommended := doxygen-1.5.0
# remark: doxygen-1.4.6 broken

# #################################
# Mhash Versions
# #################################

mhash_targets := mhash-0.9.7.1
mhash_recommended := mhash-0.9.7.1

# #################################
# Postgresql Versions
# #################################

postgresql_targets := postgresql-8.0.3
postgresql_recommended := postgresql-8.0.3

# #################################
# Valgrind Versions
# #################################

valgrind_targets := valgrind-3.1.0 valgrind-3.2.0
valgrind_recommended := valgrind-3.2.0