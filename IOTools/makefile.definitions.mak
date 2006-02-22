# Oliver Kullmann, 13.4.2005 (Swansea)


ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

OKTestsystem := -I$(OKSYSTEM)/Transitional/Testsystem

General_options := -g
Optimisation_options := -O3

test_program := TestIOTools

programs :=

source_libraries = $(OKTestsystem) $(Bibliothek) $(Loki) $(Boost) $(OKSystem)

link_libraries :=

Root := $(wildcard ../../..)
