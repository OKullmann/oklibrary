# Oliver Kullmann, 13.4.2005 (Swansea)

ifndef Mhash
  ifdef MHASH	
    Mhash := $(MHASH)
  else
    Mhash :=
  endif
endif

Mhash_include := -I$(Mhash)/include

General_options := 
Optimisation_options := -O3 -DNDEBUG

test_program := TestKryptologie

programs := HashMD5

source_libraries = $(Mhash_include)

link_libraries := -L$(Mhash)/lib -lmhash


