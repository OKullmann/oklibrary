// Oliver Kullmann, 3.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/plans/Configuration.hpp
  \brief Plans for handling of configuration data regarding external sources


  \todo Software management:
  All information about whether software (like doxygen, cmake, gcc etc.)
  needed is installed locally or system-wide, and where to find it,
  must be found in the configuration directory (in an easily accesible
  way). It must be possible to update the system (installing a new
  cmake, for example, or a new gcc), perhaps by setting variables
  about supported and recommended versions.


  \todo external_sources_versions.mak:
  <ul>
   <li> Names like
   <code> doxygen_supported_not_recommended_version_numbers </code>
   are misleading (these versions are not "not recommended"), and
   they are too long. </li>
   <li> How we call our internal targets must be decoupled from
   how packages are called etc. </li>
   <li> There are several inline-comments in external_sources_versions.mak,
   typically referring to apparantly stale variables. </li>
   <li> The variables related to the targets all (should) follow
   the same scheme, and perhaps should be defined in a general
   way in the main external-sources makefile (perhaps using
   a list for all external sources supported, and then generating
   these variables) ?! </li>
   <li> The role of Configuration/external_sources_versions.mak must
   be clarified. DONE (contains all variable setting for making
   special sources which are general and relate to the version
   number and/or to the documentation and web pages) </li>
  </ul>

*/  

