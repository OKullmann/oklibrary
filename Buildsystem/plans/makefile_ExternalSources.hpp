// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/plans/makefile_ExternalSources.hpp
  \brief Plans for the makefile responsible for handling external sources

  \todo ERROR

  Suddenly building boost on cs-wsok with a local gcc (version 3.4.3 or
  3.4.6 for example) doesn't work anymore, but there are linking errors.
  It seems that on csltok everything works, so that it could be a
  64bit thing.

  What did change?? And it seems that gcc versions for example 4.1.1
  do work??
  What is the role ofLD_LIBRARY_PATH ?? (On cs-wsok it is empty.)

  We should check in general whether building boost links to the 32bit
  or to the 64bit version.

  \todo General
   - It would ge good, if after doing a local installation, easily the installation could also be
     make global.
   - Optionally there should be also local versions of valgrind and doxygen (and other tools).

  \todo Gcc
   - When building some gcc-version, only the necessary directories
     should be created. This should be handled as we have it now with boost (using
     timestamp-files).
   - If variable "gcc-version" is set, then it should have one of the allowed values (while otherwise we get
     an error).

  \todo Boost
   - When building boost (in some variation) using "gcc-version=...", then as a subtarget
     we have the build of the gcc-version (so that, if necessary, gcc is build).
   - Installation of bjam should be improved: Having exactly one bjam-installation for each boost-version,
     and no need to recreate it if it's already there. Or, perhaps better: We just leave it in
     the distribution directory?
   - Building boost should include copying the documentation to doc (in the subdirectory
     boost-1_33_1 for example).

  \todo PostgreSQL
   - Update PostgreSQL to version 8.1 (and test it).
   - "make initialise-database" should work with the recommended version.

  \todo Tools
   - Investigate the NiX system (a system for managing libraries).

  \todo Other sources:
   - Build a local version of Xerces.
   - Build a local version of gmp.
   - Build the R system (locally and globally).
   - Build and incorporate graphviz (the dot-tool is needed by doxygen for creating the graphs)
     and tulip.
   - SOCI (soci.sourceforge.net) ?
   - gcc-xml (www.gccxml.org) ?

*/
