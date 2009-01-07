# Oliver Kullmann, 28.4.2008 (Guangzhou)
# Copyright 2008, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Helper makefile for git operations (needed to obtain access to the
# configuration data).

SHELL = /bin/bash
.SUFFIXES :

# The oklib-masterscript defines OKplatform.
ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform = $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKconfiguration
  OKconfiguration = $(OKplatform)/.oklib/Configuration
endif

include $(OKconfiguration)/configuration_data.mak
export

# ######################################################################

.PHONY : push_developers pull_developers pull_public push_ssh pull_ssh

push_developers :
	git push $(git_http_push_repository_okl) ${git_arguments_okl}

pull_public :
	git pull ${git_http_repository} ${git_arguments_okl}

push_ssh :
	git push --receive-pack "${remote_repository_okl}/bin/git-receive-pack" ssh://${remote_machine_okl}/${remote_repository_okl}/OKsystem/Transitional ${git_arguments_okl}

pull_ssh :
	git pull --upload-pack "${remote_repository_okl}/bin/git-upload-pack" ssh://${remote_machine_okl}/${remote_repository_okl}/OKsystem/Transitional ${git_arguments_okl}

