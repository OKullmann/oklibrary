# Oliver Kullmann, 11.7.2007 (Swansea)

# This makefile provides all configuration variables

ifndef OKconfiguration
  $(error The make-variable OKconfiguration must be defined (as the directory path for the configuration directory; default is a symbolic link to Transitional/Buildsystem/Configuration in OKplatform/.oklib)!)
endif

include $(OKconfiguration)/main_directories.mak
include $(OKconfiguration)/version_numbers.mak
include $(OKconfiguration)/ExternalSources/all.mak
include $(OKconfiguration)/system_definitions.mak
include $(OKconfiguration)/build_directories.mak
include $(OKconfiguration)/macro_replacement.mak
include $(OKconfiguration)/doxygen_documentation.mak
include $(OKconfiguration)/local_html.mak

ifndef OKoverrideconfig
  $(error The make-variable OKoverrideconfig  must be defined (as the path to the makefile containing overriding configuration definitions; default is OKplatform/.oklib/override.mak)!)
endif

include $(OKoverrideconfig)

