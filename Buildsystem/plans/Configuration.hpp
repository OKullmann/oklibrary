// Oliver Kullmann, 11.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/Configuration.hpp
  \brief Plans regarding configuration management

  \todo Primary versus derived configurations
  <ul>
   <li> The primary configurations are under version control. </li>
   <li> Derived values (like source- and link-libraries for external libraries)
   are to be found perhaps in system_directories/configurations ? There might be also
   a symbolic link to the primary configurations? </li>
  </ul>


  \todo Configuration data
  <ul>
   <li> A central problem is: How to create a central repository
   of configuration data, usable by the various web pages as well
   as by the build system?!
    <ol>
     <li> Perhaps a new subdirectory Buildsystem/Configuration? </li>
     <li> Inside we have a make-file containing only variable settings
     \verbatim
CONFIGVAR1=value1
CONFIGVAR2=value2
     \endverbatim
     To be used by the C preprocessor, this has to be translated into
     \verbatim
#define CONFIGVAR1 value1
#define CONFIGVAR2 value2
     \endverbatim
     This seems to work whatever value1 is (as long value1 does not contain
     line breaks).
     To be used by m4, this has to be translated into
     \verbatim
m4_define(CONFIGVAR1,value1)
m4_define(CONFIGVAR2,value2)
     \endverbatim
     while for usage by cmake the translation is
     \verbatim
SET(CONFIGVAR1 value1)
SET(CONFIGVAR2 value2)
     \endverbatim
     In both cases there might be problems if value1 for example contains
     a closing bracket? Perhaps we could just use the export-facility of make,
     so that these variables become environment variables?!? But then we would
     loose control.
     </li>
     <li> If we are going to use CMake, then perhaps the variable settings should
     be done in the cmake-syntax. </li>
     <li> Splitting the definitions over several files is likely preferable from the
     order point of view. Shouldn't be too complicated to use them. </li>
    </ol>
   </li>
  </ul>


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


  \todo Make-variables for external libraries : UPDATE needed (%w.r.t. the new view
  of the configuration center)
  <ul>
   <li> How to define variables like Boost and Ubcsat in general?
     We need a nice little method (plus documentation), so that we can add easily as
     many external libraries as we want. </li>
   <li> And then we have make-variables "gcc_version_number" (for generic.mak) and
     "gcc-version" (for boost.mak) which likely should have the same names. </li>
   <li> In system_definitions.mak we have a definition of Boost which seems wrong
     to me (OK) (or at least misleading). DONE (resp. moved to other todos) </li>
  </ul>


  \todo Definitions for doxygen:
  <ul>
   <li> The Doxyfile should be part of the (primary) configuration system. </li>
   <li> Perhaps it would be preferable, if the following variable definitions
    <ol>
     <li> ALIASES </li>
     <li> PREDEFINED </li>
     <li> EXPAND_AS_DEFINED </li>
     <li> FILE_PATTERNS </li>
    </ol>
    could come from a separate configuration file --- how to achieve this? </li>
   <li> Allows the Doxyfile for includes or macro expansion? </li>
   <li> Perhaps it's best to define a "master-doxyfile" in the primary configuration system,
   containing macros or includes for the above four definitions, and then the build system
   builds the finale Doxyfile (in the derived configurations) in dependency on these
   includes/macro-definitions. (Alternatively, one can specify directly a doxyfile to
   be used --- how to achieve this with cmake? In the normal case, target html depends
   on the doxyfile to be created if necessary, but if some variable is specified on the
   make command line, then the doxyfile is not used.) </li>
   <li> With every new version of doxygen, the update-wizzard of doxygen has to be run
   on the created doxyfile, one has to study the changes, and update the master-doxyfile
   accordingly. </li>
  </ul>


  \todo Developers information:
  <ul>
   <li> We should have a subdirectory Configuration/Developers, containing files
    <ol>
     <li> current_main </li>
     <li> past_main </li>
     <li> past_external </li>
     <li> current_external </li>
    </ol>
    Each of those files contains lines like
    <ol>
     <li> OK "Oliver Kullmann" O.Kullmann@Swansea.ac.uk </li>
     <li> MH "Matthew Henderson" XXX </li>
     <li> ML "Matthew Lewsey" XXX </li>
    </ol>
    Or is there some "official" file format for such address-data?!
   </li>
   <li> Out of these files an html-file is produced. </li>
  </ul>


*/

