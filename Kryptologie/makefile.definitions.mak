# Oliver Kullmann, 13.4.2005 (Swansea)


ifdef OKSYSTEM
  OKSystem := -I$(OKSYSTEM)
else
  OKSystem := 
endif

General_options :=
Optimisation_options := -O3 -DNDEBUG

test_program := TestKryptologie

programs := HashMD5

source_libraries = $(OKSystem)

link_libraries := -lmhash

Root := $(wildcard ../../..)
