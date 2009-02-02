# Oliver Kullmann, 1.2.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

CXX = g++

Standard_options = -ansi -pedantic
Warning_options = -Wall
Optimisation_options = -O3 -DNDEBUG

.SUFFIXES :
.PHONY : all clean cleanall

all : EmptyProgram 

EmptyProgram : EmptyProgram.cpp
	$(CXX) -o EmptyProgram $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) EmptyProgram.cpp

clean :

cleanall :
	- rm EmptyProgram
