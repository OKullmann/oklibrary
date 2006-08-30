# Oliver Kullmann, 13.4.2005 (Swansea)

OKTestSystem := -I$(OKSystem)/Transitional/TestSystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestFailedLiteralReduction

programs :=

source_libraries =  $(Boost) $(OKTestSystem)

link_libraries := 

Root := $(wildcard ../../..)
