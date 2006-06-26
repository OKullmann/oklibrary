SHELL = /bin/sh
.SUFFIXES :

# ToDo:
# Instead of copying use linking for the placement of the other makefiles

# Special makefile for the buildsystem

ifndef OKPlatform
  ifdef OKPLATFORM
    OKPlatform := $(OKPLATFORM)
  else
    $(error Either OKPlatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKSystem
  ifdef OKSYSTEM
    OKSystem := $(OKSYSTEM)
  else
    OKSystem := $(OKPlatform)/OKsystem
  endif
endif

srcdir := $(OKSystem)/Transitional/Buildsystem

include $(srcdir)/makefile_standardgoals.mak

externalsourcesdir := $(OKPlatform)/ExternalSources

Directories :=  $(externalsourcesdir)

makefile_oksystem := $(OKSystem)/makefile
makefile_externalsources := $(externalsourcesdir)/makefile

all $(test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) : 

prebuild : $(Directories) $(makefiles) $(makefile_oksystem) $(makefile_externalsources) $(doxy_file)

$(Directories) : % :
	mkdir $@

$(makefile_oksystem) : $(srcdir)/makefile_OKsystem
	cp $< $@

$(makefile_externalsources) : $(srcdir)/makefile_ExternalSources.mak
	ln -s $< $@


maintainer_clean :
	rm $(makefiles)
