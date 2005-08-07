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
includedir := $(prefix)/include
externalsourcesdir := $(prefix)/ExternalSources

Directories := $(includedir) $(externalsourcesdir)

makefiles := $(addprefix $(includedir)/, $(notdir $(shell ls $(srcdir)/makefile_*[!~])))
makefile_oksystem := $(prefix)/OKsystem/makefile
makefile_externalsources := $(externalsourcesdir)/makefile

doxy_file := $(includedir)/Doxyfile

all $(test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) : 


prebuild : $(Directories) $(makefiles) $(makefile_oksystem) $(makefile_externalsources) $(doxy_file)

$(Directories) : % :
	mkdir $@

$(makefiles) $(doxy_file) : $(includedir)/% : $(srcdir)/%
	cp $< $@

$(makefile_oksystem) : $(srcdir)/makefile_OKsystem
	cp $< $@

$(makefile_externalsources) : $(srcdir)/makefile_ExternalSources
	cp $< $@


maintainer_clean :
	rm $(makefiles)
