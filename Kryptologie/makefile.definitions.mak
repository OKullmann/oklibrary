# Oliver Kullmann, 13.4.2005 (Swansea)

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif
ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif
ifdef XERCES_H
  Xerces_h := -I$(XERCES_H)
else
  Xerces_h :=
endif
ifdef XERCES_SO
  Xerces_so := -I$(XERCES_SO)
else
  Xerces_so :=
endif

General_options :=
Optimisation_options := -O3

test_program := TestKryptologie

programs := HashMD5

source_libraries =

link_libraries := -lmhash

Root := $(wildcard ../../..)
