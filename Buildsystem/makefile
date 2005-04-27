SHELL = /bin/sh
.SUFFIXES :

srcdir := $(CURDIR)
# PROBLEM: This should be the directory, where this makefile is located!
# But it is the directory, where this makefile has been called.

include $(srcdir)/makefile_standardgoals
# PROBLEM: Does not work with remote calls ("i.e., make -f xxx").

include $(srcdir)/makefile.definitions
# SAME PROBLEM.

# Definitions required from makefile.definitions:
# Root

prefix := $(shell cd $(Root); pwd)
includedir := $(prefix)/include

Directories := $(includedir)

makefiles := $(addprefix $(includedir)/, $(shell ls makefile_*[!~]))
makefile_oksystem := $(prefix)/OKsystem/makefile

all $(test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) : 

prebuild : $(Directories) $(makefiles) $(makefile_oksystem)

$(Directories) : % :
	mkdir $@

$(makefiles) : $(includedir)/% : %
	cp $< $@

$(makefile_oksystem) : makefile_OKsystem
	cp $< $@

maintainer_clean :
	rm $(makefiles)
