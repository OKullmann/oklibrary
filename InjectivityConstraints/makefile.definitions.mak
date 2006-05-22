# Matthew Henderson, 24.4.2006 (Paderborn)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestInjectivityConstraints

programs :=

source_libraries = $(Boost)

link_libraries := 

Root := $(wildcard ../../..)
