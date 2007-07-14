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
   <li> The doxygen system (see "Definitions for doxygen" below). </li>
   <li> Personal information:
    <ol>
     <li> See Buildsystem/Configuration/plans/Persons.hpp. </li>
    </ol>
  </ul>


  \todo Configuration data format
  <ul>
   <li> Standardisation of naming configuration variables:
    <ol>
     <li> Perhaps a directory gets the prefix "dir_", and a main directory "Directory"
     is then called "dir_Directory". </li>
    </ol>
   </li>
   <li> Perhaps for defining configuration variables we should always first check whether
   it's already defined, and if not, then look for an environment variable. </li>
   <li> In order that the configuration-variables become available, the corresponding master-process
   has to be invoked by some makefile; so likely over time a collection of small specialised
   makefiles arises. </li>
   <li> Splitting the definitions over several files is likely preferable from the
   order point of view. Shouldn't be too complicated to use them. </li>
   <li> In directory .oklib a file "override.mak" is placed, where overriding definitions
   of configuration variables are to be put (for example variable ExternalSources
   could be redefined here, to use the external sources of another
   OKplatform-installation). DONE </li>
   <li> Should the configuration-make-variables be recursive or not? Recursive variables
   give more power ("one never knows"), and so at the moment the decision is to make
   all configuration-make-variables recursive. DONE (recursive variables are needed,
   so that definitions can be overriden) </li>
   <li> DONE (we use makefiles with makevariable-definitions for all configuration data)
     We could use makefile-syntax, that is, we have make-files containing only variable settings
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
   <li> The solution seems to be not to do translations, but to have all
   configuration-variables defined in makefiles as as make-variables.
   Via the export-function then we have all them plus the environment
   variables at hand, and every other usage of configuration variables
   (doxygen or an html-preprocessor) just accesses environment variables.
   DONE (we do as proposed here) </li>
   <li> If we are going to use CMake, then perhaps the variable settings should
     be done in the cmake-syntax. DONE (the cmake-transition, if at all, can
     happen only after we have a running make-system) </li>
   </li>
  </ul>


  \todo Primary versus derived configurations
  <ul>
   <li> The primary configurations are under version control. </li>
   <li> Derived values (like source- and link-libraries for external libraries)
   are to be found perhaps in system_directories/configurations ? There might be also
   a symbolic link to the primary configurations? </li>
  </ul>


  \todo system_definitions.mak:
  <ul>
   <li> The role of system_definitions.mak must be clarified.
   Do those library variables (Boost, Ubcsat) belong to it??
   <ul>
    <li> system_definitions.mak should
    contain definitions of Make variables which are used by
    several makefiles. However, the prefix "system_" also implies that 
    these variables belong to the OKlibrary and not to the external
    sources, so perhaps the definitions of variables relevant
    for the external sources are moved elsewhere.</li>
    <li> Perhaps all variable definitions relevant for the 
    external sources (including version numbers) should go into
    ExternalSources/definitions.mak? OK:
    <ol>
     <li> What about that "ExternalSources/definitions_.mak" ?
     (This name seems strange to me anyway.) </li>
     <li> The main question is about visibility. There needs to be fixed
     the architecture of our makefiles (w.r.t. inclusion and variable
     definitions). DONE (handled by the new configuration system) </li>
    </ol>
   </li>
   </ul>
   </li>
   <li> What is the precise relation to external_sources_versions.mak ? </li>
   <li> Boost:
    <ul>
     <li> The default value of Boost
     <code> -I/usr/local/boost-1_34_0 </code>
     is stale; the recommended Boost version number is needed. 
      <ul>
       <li> So for every external library Extlib the default value
       of the Make variable Extlib should be the recommended version 
       number? </li>
      </ul>
     </li>
     <li> If the variable includes the "-I", then for other
     variables we use a suffix "_include", which we should also
     do for the boost-variables. </li>
     <li> We need a precise and central definition of those
     boost-variables. </li>
     <li> The boost-variable definitions are inconsistent with
     the rest of the build-system, so they should be reverted (as
     discussed). DONE </li>
    </ul>
   </li>
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
   <li> Allows the Doxyfile for includes or macro expansion? See "Environment-variable expansion"
   in Buildsystem/OKlibBuilding/Targets/html/plans/general.hpp. DONE (yes) </li>
  </ul>

*/

