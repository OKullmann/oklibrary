# Oliver Kullmann, 20.3.2008 (Swansea)
# Copyright 2008 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# General functions for the buildsystem

# Error checking for sequences of bash-commands
define postcondition
if [ $$? != 0 ]; then exit 1; fi;
endef

