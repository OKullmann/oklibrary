# Matthew Lewsey, 9.11.2006 (Swansea)
# filename : BuildSystem/ExternalSources/ubcsat.mak

# ##################################
# Targets
# ##################################

include $(OKbuildsystem)/external_sources_versions.mak

# ##################################
# Directory Structure
# ##################################

ubcsat-base-directory := $(ExternalSources)/Ubcsat
ubcsat-extract-directory := $(ubcsat-base-directory)/ubcsat-1-0-0
ubcsat-mod-extract-directory := $(ubcsat-base-directory)/mod-ubcsat-1-0-0
ubcsat-installation-directory := $(ubcsat-base-directory)/1-0-0
ubcsat-lib-directory := $(ubcsat-base-directory)/1-0-0/lib
ubcsat-mod-lib-directory := $(ubcsat-base-directory)/1-0-0/mod-lib
ubcsat-bin-directory := $(ubcsat-base-directory)/1-0-0/bin
ubcsat-mod-bin-directory := $(ubcsat-base-directory)/1-0-0/mod-bin
ubcsat-src-directory := $(ubcsat-base-directory)/ubcsat-1-0-0/src
ubcsat-mod-src-directory := $(ubcsat-base-directory)/mod-ubcsat-1-0-0/src

ubcsat-directories := $(ubcsat-base-directory) $(ubcsat-lib-directory) $(ubcsat-mod-lib-directory) $(ubcsat-bin-directory) $(ubcsat-mod-bin-directory) $(ubcsat-extract-directory) $(ubcsat-mod-extract-directory) $(ubcsat-installation-directory) $(ubcsat-mod-installation-directory)

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
	$(call unarchive,ubcsat-1-0-0,$(ubcsat-extract-directory))
	touch $@

$(ubcsat-mod-extract-directory)/tag : | $(ubcsat-base-directory) $(ubcsat-mod-extract-directory)
	$(call unarchive,ubcsat-1-0-0,$(ubcsat-mod-extract-directory))
	touch $@

ubcsat : $(ubcsat-extract-directory)/tag  $(ubcsat-mod-extract-directory)/tag alter $(ubcsat-bin-directory)/ubcsat $(ubcsat-mod-lib-directory)/libubcsat.a

$(paths) : $(ubcsat-lib-directory)/%.o : $(ubcsat-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-lib-directory)
	gcc -c $< -o $@

$(mod-paths) : $(ubcsat-mod-lib-directory)/%.o : $(ubcsat-mod-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-mod-lib-directory) 
	gcc -c $< -o $@

$(ubcsat-bin-directory)/ubcsat : $(paths) | $(ubcsat-bin-directory)
	gcc -O3 -lm -o $(ubcsat-bin-directory)/ubcsat $(paths)

$(ubcsat-mod-lib-directory)/libubcsat.a : $(mod-paths)
	$(AR) $(ARFLAGS) $@ $^

.PHONY : test

alter : 
	echo this alters mylocal.h
	chmod 7 $(ubcsat-mod-src-directory)/mylocal.h
	rm $(ubcsat-mod-src-directory)/mylocal.h -f
	echo '#define ALTERNATEMAIN' > $(ubcsat-mod-src-directory)/mylocal.h

