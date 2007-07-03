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


  \todo external_sources_versions.mak: UPDATE needed (%w.r.t. the new view
  of the configuration center)
  <ul>
   <li> The role of external_sources_versions.mak must
   be clarified. </li>
   <li> Names like
   <code> doxygen_supported_not_recommended_version_numbers </code>
   are misleading (these versions are not "not recommended"), and
   they are too long. </li>
  </ul>

*/  

