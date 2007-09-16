// Oliver Kullmann, 23.6.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file SystemSpecifics/plans/Demangling.hpp
  \brief Plans for the module SystemSpecifics/Demangling.hpp

  \todo Testing:
  Write a testfunction (which first defines an MPL-vector of pairs of
  types and expected descriptions, and then just runs through it).

  \todo Usage of demangle.h:
   - Is "cplus_demangle" the right function to call?
   - Is DMGL_TYPES the right option? Are there other useful options?

  \todo Linkage:
   For local versions of gcc linking should not happen with the global
   -liberty, but with OKplatform/ExternalSources/Gcc/?.?.?/lib/libiberty.a.
   And then SystemSpecifics/libiberty.h and SystemSpecifics/demangle.h
   should also be replaced with the local versions.
*/
