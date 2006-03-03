# Oliver Kullmann, 13.4.2005 (Swansea)


General_options :=
Optimisation_options := -O3 -DNDEBUG

test_program := TestKryptologie

programs := HashMD5

source_libraries = 

link_libraries := -lmhash

Root := $(wildcard ../../..)
