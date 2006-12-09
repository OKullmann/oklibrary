# Matthew Lewsey, 9.11.2006 (Swansea)
# filename : BuildSystem/ExternalSources/makefile_ubcsat.mak

# ##################################
# Usage
# ##################################

# make ubcsat

# ##################################
# Targets
# ##################################

include $(OKbuildsystem)/external_sources_versions.mak

# ##################################
# Directory Structure
# ##################################

ubcsat-base-directory := $(prefix)/Ubcsat

ubcsat-directories := $(ubcsat-base-directory)

$(ubcsat-directories) : % : 
	mkdir $@


# #################################
# The Target
# ################################

$(ubcsat-base-directory)/$(ubcsat_targets) : 
	mkdir -p $(ubcsat-base-directory)/ubcsat-1-0-0
	$(call unarchive,ubcsat-1-0-0,$(ubcsat-base-directory)/ubcsat-1-0-0)

ubcsat : $(ubcsat-base-directory)/ubcsat-1-0-0
