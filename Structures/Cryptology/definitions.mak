# Oliver Kullmann, 13.4.2005 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program :=

programs := # HashMD5

source_libraries = $(mhash_include_option_okl)

