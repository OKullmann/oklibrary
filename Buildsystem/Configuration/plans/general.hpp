// Oliver Kullmann, 11.6.2007 (Swansea)

/*!
  \file Buildsystem/Configuration/plans/general.hpp
  \brief Plans regarding configuration management


  \todo Configuration data overview :
  We need to get an overview on the different types of configuration data.
  <ul>
   <li> Build-related </li>
   <li> External sources:
    <ol>
     <li> See Buildsystem/ExternalSources/plans/Configuration.hpp. </li>
    </ol>
   </li>
   <li> html-pages </li>
   <li> doxygen </li>
   <li> Personal information:
    <ol>
     <li> See Buildsystem/Configuration/plans/Persons.hpp. </li>
    </ol>
  </ul>


  \todo Configuration data format
  <ul>
   <li> We could use makefile-syntax, that is, we have make-files containing only variable settings
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
   </li>
  </ul>


  \todo Primary versus derived configurations
  <ul>
   <li> The primary configurations are under version control. </li>
   <li> Derived values (like source- and link-libraries for external libraries)
   are to be found perhaps in system_directories/configurations ? There might be also
   a symbolic link to the primary configurations? </li>
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

*/

