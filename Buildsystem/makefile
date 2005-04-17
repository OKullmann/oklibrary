.PHONY : prebuild all unoptimised optimised check test testop clean cleanall force maintainer-clean

include makefile.definitions

# Definitions required from makefile.definitions:
# Root

srcdir := $(shell pwd)

prefix := $(shell cd $(Root); pwd)
includedir := $(prefix)/include

Directories := $(includedir)

makefiles := $(addprefix $(includedir)/, makefile_generic makefile_recursive)

prebuild : $(Directories) $(makefiles)

$(Directories) : % :
	mkdir $@

$(makefiles) : $(includedir)/% : %
	cp $< $@


all unoptimised optimised check test testop clean cleanall force : 

maintainer-clean :
	rm $(makefiles)
