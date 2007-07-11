ifndef OKconfiguration
  $(error The make-variable OKconfiguration must be defined (as the directory path for the configuration directory; default is a symbolic link to Transitional/Buildsystem/Configuration)!)
endif

include $(OKconfiguration)/main_directories.mak
include $(OKconfiguration)/version_numbers.mak
