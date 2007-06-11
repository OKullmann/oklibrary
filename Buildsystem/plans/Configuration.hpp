// Oliver Kullmann, 11.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/Configuration.hpp
  \brief Plans regarding configuration management

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

*/

