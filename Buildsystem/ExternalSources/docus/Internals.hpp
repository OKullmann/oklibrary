// Oliver Kullmann, 23.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/docus/Internals.hpp
  \brief Internals of how the build system for external sources works


  <h1> Internals of the build system for external sources </h1>

  <h2> General overview </h2>
  
  XXX base directory $(ExternalSources) for installations of external sources
  
  XXX version numbers etc. in Buildsystem/Configuration/ExternalSources/all.mak XXX

  Buildsystem/ExternalSources/Makefile is the central makefile:
  <ul>
   <li> $(ExternalSources)/makefile is a symbolic link to it XXX </li>
   <li> Defines the main targets for building and cleaning of
   external libraries. XXX </li>
   <li> It includes makefiles in Buildsystem/ExternalSources/SpecialBuilds,
   each responsible for building one external source. XXX </li>
  </ul>


  <h2> Adding new external sources </h2>

  XXX


  <h2> In case of trouble </h2>

  <ol>
   <li> See the error messages in <code>$(external_sources_log)</code>. </li>
   <li> Often some library or some tool is missing; either you can add
   the missing part yourself (perhaps just using the package management
   of your Linux distribution), or you might find it in the OKlibrary. </li>
   <li> If this doesn't help, consult the mailing list XXX </li>
   <li> If we can't resolve it, since it seems to depend on specialities
   of your system, then
    <ol>
     <li> extract the sequence of build instruction as follows XXX </li>
     <li> reproduce the problem independently of the OKlibrary, by using
     the extracted build instructions --- if the problem goes away, report
     back to our mailing list XXX. </li>
     <li> Otherwise contact the support of the respective package, and ask
     what's wrong with the build instructions. </li>
     <li> Either you now can "repair" your own system, so that the build
     of the OKlibrary now works, or you report back to our mailing list XXX.
     </li>
    </ol>
   </li>
  </ol>

*/

