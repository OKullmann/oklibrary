SHELL = /bin/sh
.SUFFIXES :

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

Directories := $(includedir)

makefiles := $(addprefix $(includedir)/, $(notdir $(shell ls $(srcdir)/makefile_*[!~])))
makefile_oksystem := $(prefix)/OKsystem/makefile
doxy_file := $(includedir)/Doxyfile

all $(test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) : 

prebuild : $(Directories) $(makefiles) $(makefile_oksystem) $(doxy_file)

$(Directories) : % :
	mkdir $@

$(makefiles) $(doxy_file) : $(includedir)/% : $(srcdir)/%
	cp $< $@

$(makefile_oksystem) : $(srcdir)/makefile_OKsystem
	cp $< $@

maintainer_clean :
	rm $(makefiles)
