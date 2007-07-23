// Oliver Kullmann, 23.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/docus/Internals.hpp
  \brief Internals of how the build system for external sources works


  <h1> Internals of the build system for external sources </h1>

  <h2> General overview </h2>
  
  XXX base directory $(ExternalSources) for installations of external sources
  
  XXX version numbers in Buildsystem/Configuration/external_sources_versions.mak

  Buildsystem/ExternalSources/Makefile is the central makefile,
  while $(ExternalSources)/makefile is a symbolic link to it.
  Here are defined the main
  targets for building and cleaning of external libraries.

  Transitional/Buildsystem/ExternalSources.mak includes makefiles
  in Buildsystem/ExternalSources/SpecialBuilds, each responsible
  for building one external source.


  <h2> Adding new external sources </h2>

*/

