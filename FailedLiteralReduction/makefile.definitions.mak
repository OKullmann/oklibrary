# Oliver Kullmann, 13.4.2005 (Swansea)

OKTestsystem := -I$(OKSystem)/Transitional/Testsystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestFailedLiteralReduction

programs :=

source_libraries =  $(Boost) $(OKTestsystem)

link_libraries := 

Root := $(wildcard ../../..)
