# Oliver Kullmann, 9.9.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# This makefile provides all configuration variables for html-purposes
# Configuration data in Configuration/Html is only relevant
# w.r.t. configuration building.

include $(OKconfiguration)/Html/doxygen_documentation.mak
include $(OKconfiguration)/Html/local_html.mak
include $(OKconfiguration)/Html/documents.mak

include $(OKconfiguration)/ExternalSources/tests.mak

# To be included last:
include $(OKconfiguration)/Html/relative_paths.mak

