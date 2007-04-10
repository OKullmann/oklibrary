ifndef ExternalSources
  ifdef EXTERNALSOURCES
    ExternalSources := $(EXTERNALSOURCES)
  else
    ExternalSources := $(OKplatform)/ExternalSources
  endif
endif

OKsystem_include := -I$(OKsystem)

Annotations_dir := $(OKsystem)/Annotations

ifndef Boost
  ifdef BOOST
    Boost = -I$(BOOST)
  else
    Boost = -I/usr/local/boost-1_33_1
  endif
endif

ifndef Ubcsat
  ifdef UBCSAT
    Ubcsat_dir := $(UBCSAT)
  else
    Ubcsat_dir := $(ExternalSources)/Ubcsat
  endif
  Ubcsat := -I$(Ubcsat_dir)/1-0-0/src
endif
# TODO: We must update and systematise the use of such variables.

ifndef Mhash
  ifdef MHASH
    Mhash_dir := $(MHASH)
  else
    Mhash_dir := $(ExternalSources)/Mhash
  endif
  Mhash := $(Mhash_dir)/0.9.9
endif
# TODO: We must update and systematise the use of such variables!

ifndef system_directories
  ifdef SYSTEMDIRECTORIES
    system_directories := $(SYSTEMDIRECTORIES)
  else
    system_directories := $(OKplatform)/system_directories
  endif
endif
