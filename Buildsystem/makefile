.PHONY : all unoptimised optimised check clean cleanall

include makefile.definitions

# Definitions required from makefile.definitions:
# Root

srcdir := $(shell pwd)

prefix := $(shell cd $(Root); pwd)
includedir := $(prefix)/include

Directories := $(includedir)

all : $(Directories) makefile_generic

makefile_generic : $(includedir)/makefile_generic

$(includedir)/makefile_generic : $(srcdir)/makefile_generic
		cp $< $@

unoptimised optimised check clean cleanall : 

