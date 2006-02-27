# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestAutarkySearch

programs :=

source_libraries =  $(OKSystem) $(Boost)

link_libraries := 

Root := $(wildcard ../../..)
