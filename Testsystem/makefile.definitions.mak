# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef OKTESTSYSTEM
  OKTestsystem := -I$(OKTESTSYSTEM)
else
  OKTestsystem :=
endif

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestTestsystem

programs := TestProgram_DesignStudy

source_libraries = $(Boost) $(OKTestsystem)

link_libraries := 

Root := $(wildcard ../../..)
