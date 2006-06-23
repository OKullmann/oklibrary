// Oliver Kullmann, 23.6.2006 (Swansea)

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
