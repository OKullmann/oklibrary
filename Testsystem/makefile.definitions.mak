# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

OKTestsystem := -I$(OKSYSTEM)/Transitional/Testsystem

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestTestsystem

programs := TestProgram_DesignStudy

source_libraries = $(Boost) $(OKTestsystem) $(OKSystem)

link_libraries := 

Root := $(wildcard ../../..)
