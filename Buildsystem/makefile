SHELL = /bin/sh
.SUFFIXES :

# ToDo:
# Instead of copying use linking for the placement of the other makefiles

# Special makefile for the buildsystem

define last-element
$(word $(words $1),$1)
endef
srcdir := $(shell cd $(dir $(call last-element, $(MAKEFILE_LIST))); pwd)

include $(srcdir)/makefile_standardgoals

include $(srcdir)/makefile.definitions.mak

# Definitions required from makefile.definitions:
# Root

prefix := $(shell cd $(srcdir); cd $(Root); pwd)
externalsourcesdir := $(prefix)/ExternalSources

Directories :=  $(externalsourcesdir)

makefile_oksystem := $(prefix)/OKsystem/makefile
makefile_externalsources := $(externalsourcesdir)/makefile

all $(test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) : 

prebuild : $(Directories) $(makefiles) $(makefile_oksystem) $(makefile_externalsources) $(doxy_file)

$(Directories) : % :
	mkdir $@

$(makefile_oksystem) : $(srcdir)/makefile_OKsystem
	cp $< $@

$(makefile_externalsources) : $(srcdir)/makefile_ExternalSources.mak
	cp $< $@


maintainer_clean :
	rm $(makefiles)
