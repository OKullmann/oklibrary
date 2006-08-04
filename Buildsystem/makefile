SHELL = /bin/sh
.SUFFIXES :

# ToDo:
# Instead of copying use linking for the placement of the other makefiles

# Special makefile for the buildsystem

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

srcdir := $(OKsystem)/Transitional/Buildsystem

include $(srcdir)/makefile_standardgoals.mak

externalsourcesdir := $(OKplatform)/ExternalSources

Directories :=  $(externalsourcesdir)

makefile_oksystem := $(OKsystem)/makefile
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
