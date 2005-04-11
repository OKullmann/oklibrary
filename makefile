# Oliver Kullmann, 10.4.2005 (Swansea)

SHELL = /bin/sh
.SUFFIXES :

goals := all unoptimised optimised check test testop allandcheck clean cleanall
sub_makes := $(dir $(shell find * -mindepth 1 -maxdepth 1 -name "makefile"))

.PHONY : $(goals) $(sub_makes)

all : $(sub_makes)
$(goals) : $(sub_makes)

$(sub_makes) :
	$(MAKE) -C $@ $(MAKECMDGOALS)

