# Oliver Kullmann, 15.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

m4_shell_macro ?= $(OKbuildsystem)/Html/m4_shell_macro
preprocessing_call ?= m4 --prefix-builtins $(m4_shell_macro)
