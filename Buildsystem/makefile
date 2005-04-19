.PHONY : prebuild all unoptimised optimised check test testop clean cleanall force maintainer-clean

include makefile.definitions

# Definitions required from makefile.definitions:
# Root

srcdir := $(shell pwd)

prefix := $(shell cd $(Root); pwd)
includedir := $(prefix)/include

Directories := $(includedir)

makefiles := $(addprefix $(includedir)/, $(shell ls makefile_*[!~]))

all unoptimised optimised check test testop clean cleanall force : 

prebuild : $(Directories) $(makefiles)

$(Directories) : % :
	mkdir $@

$(makefiles) : $(includedir)/% : %
	cp $< $@


maintainer-clean :
	rm $(makefiles)
