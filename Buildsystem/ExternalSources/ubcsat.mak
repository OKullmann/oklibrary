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
ubcsat-tmp-src-directory := $(ubcsat-base-directory)/1-0-0/tmp

ubcsat-directories := $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-lib-directory) $(ubcsat-bin-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)

ubcsat_names := adaptnovelty algorithms gsat gsat-tabu gwsat hsat hwsat irots mt19937ar mylocal novelty parameters reports rnovelty rots samd saps ubcsat ubcsat-help ubcsat-internal ubcsat-io ubcsat-mem ubcsat-reports ubcsat-time ubcsat-triggers walksat walksat-tabu

ubcsat_c_files := $(addsuffix .c, $(ubcsat_names))
ubcsat_c_files_paths := $(addprefix $(ubcsat-src-directory)/, $(ubcsat_c_files))
ubcsat_o_files := $(addsuffix .o, $(ubcsat_names))

$(ubcsat-directories) : % : 
	mkdir $@

paths := $(addprefix $(ubcsat-lib-directory)/, $(ubcsat_o_files))

# #################################
# The Targets
# ################################

$(ubcsat-extract-directory)/tag : | $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)
	$(call unarchivefolder,ubcsat-1-0-0,$(ubcsat-extract-directory),src)
	dos2unix $(ubcsat-src-directory)/*.c
	cp $(ubcsat-src-directory)/* $(ubcsat-tmp-src-directory)
	cp -f $(OKsystem)/Transitional/AutarkySearch/ubcsat-types.h $(ubcsat-tmp-src-directory)
	touch $@

ubcsat : $(ubcsat-extract-directory)/tag $(ubcsat-bin-directory)/ubcsat $(ubcsat-lib-directory)/libubcsat.a cleanup

$(paths) : $(ubcsat-lib-directory)/%.o : $(ubcsat-tmp-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-lib-directory)
	gcc -c $< -o $@

$(ubcsat-bin-directory)/ubcsat : $(paths) | $(ubcsat-bin-directory)
	gcc -O3 -lm -o $(ubcsat-bin-directory)/ubcsat $(ubcsat_c_files_paths)

$(ubcsat-lib-directory)/libubcsat.a : $(paths)
	$(AR) $(ARFLAGS) $@ $^

.PHONY : cleanup

cleanup :
	rm -rf $(ubcsat-tmp-src-directory)

