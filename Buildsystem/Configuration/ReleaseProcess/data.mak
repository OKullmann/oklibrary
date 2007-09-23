# Oliver Kullmann, 14.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Configuration data for the release process

package_build_script ?= $(OKbuildsystem)/ReleaseProcess/CreatePackage
packages_dir ?= $(system_directories)/packages

release_history_template ?= $(OKbuildsystem)/Configuration/ReleaseProcess/ReleaseHistory
release_history_output ?= $(local_html_dir)/ReleaseHistory

readme_file ?= $(OKbuildsystem)/ReleaseProcess/README

