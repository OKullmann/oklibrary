# Tony H. Bao, 13.04.2005 (Swansea)

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestMessages

programs := Messages_Testapplication

source_libraries = $(Boost) $(OKSystem)

link_libraries := 

Root := $(wildcard ../../..)
