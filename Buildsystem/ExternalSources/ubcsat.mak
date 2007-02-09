# Matthew Lewsey, 9.11.2006 (Swansea)
# filename : BuildSystem/ExternalSources/ubcsat.mak

# ##################################
# Functions
# ##################################

define unarchivefolder
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip $(3); elif [ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2 $(3); else exit 1; fi;
endef

# ##################################
# Targets
# ##################################

include $(OKbuildsystem)/external_sources_versions.mak

# ##################################
# Directory Structure
# ##################################

ubcsat-base-directory := $(ExternalSources)/Ubcsat
ubcsat-extract-directory := $(ubcsat-base-directory)/1-0-0
ubcsat-lib-directory := $(ubcsat-base-directory)/1-0-0/lib
ubcsat-bin-directory := $(ubcsat-base-directory)/1-0-0/bin
ubcsat-src-directory := $(ubcsat-base-directory)/1-0-0/src
ubcsat-mod-extract-directory := $(ubcsat-base-directory)/1-0-0
ubcsat-mod-src-directory := $(ubcsat-base-directory)/1-0-0/mod-src
ubcsat-mod-lib-directory := $(ubcsat-base-directory)/1-0-0/mod-lib


ubcsat-directories := $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-lib-directory) $(ubcsat-mod-src-directory) $(ubcsat-mod-lib-directory) $(ubcsat-bin-directory)

ubcsat_names := adaptnovelty algorithms gsat gsat-tabu gwsat hsat hwsat irots mt19937ar mylocal novelty parameters reports rnovelty rots samd saps ubcsat ubcsat-help ubcsat-internal ubcsat-io ubcsat-mem ubcsat-reports ubcsat-time ubcsat-triggers walksat walksat-tabu

ubcsat_c_files := $(addsuffix .c, $(ubcsat_names))
ubcsat_o_files := $(addsuffix .o, $(ubcsat_names))

$(ubcsat-directories) : % : 
	mkdir $@

paths := $(addprefix $(ubcsat-lib-directory)/, $(ubcsat_o_files))
mod-paths := $(addprefix $(ubcsat-mod-lib-directory)/, $(ubcsat_o_files))

# #################################
# The Targets
# ################################

$(ubcsat-extract-directory)/tag : | $(ubcsat-base-directory) $(ubcsat-extract-directory)
	$(call unarchivefolder,ubcsat-1-0-0,$(ubcsat-extract-directory),src)
	touch $@

$(ubcsat-mod-extract-directory)/tag_ : | $(ubcsat-base-directory) $(ubcsat-extract-directory)
	$(call unarchivefolder,ubcsat-1-0-0,$(ubcsat-mod-extract-directory),src)
	mv $(ubcsat-extract-directory)/src $(ubcsat-mod-src-directory)
	touch $@

ubcsat : $(ubcsat-mod-extract-directory)/tag_ $(ubcsat-extract-directory)/tag $(ubcsat-bin-directory)/ubcsat  alter dos2unix $(ubcsat-mod-lib-directory)/libubcsat.a

$(paths) : $(ubcsat-lib-directory)/%.o : $(ubcsat-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-lib-directory)
	gcc -c $< -o $@

$(mod-paths) : $(ubcsat-mod-lib-directory)/%.o : $(ubcsat-mod-src-directory)/%.c | $(ubcsat-mod-lib-directory) 
	gcc -c $< -o $@

$(ubcsat-bin-directory)/ubcsat : $(paths) | $(ubcsat-bin-directory)
	gcc -O3 -lm -o $(ubcsat-bin-directory)/ubcsat $(paths)

$(ubcsat-mod-lib-directory)/libubcsat.a : $(mod-paths)
	$(AR) $(ARFLAGS) $@ $^

.PHONY : alter

alter : 
	echo this alters mylocal.h
	chmod 7 $(ubcsat-mod-src-directory)/mylocal.h
	rm $(ubcsat-mod-src-directory)/mylocal.h -f
	echo '#define ALTERNATEMAIN' > $(ubcsat-mod-src-directory)/mylocal.h

.PHONY : dos2unix

dos2unix : 
	echo will run dos2unix on src
