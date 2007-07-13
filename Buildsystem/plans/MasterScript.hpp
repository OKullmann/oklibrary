// Oliver Kullmann, 13.7.2007 (Swansea)

/*!
  \file Buildsystem/plans/MasterScript.hpp
  \brief Plans for the masterscript, the "user-interface" to the build system

  Similar to the use of "git" in a Git-repository, we use "oklib"
  in an OKplatform-installation.


  \todo First implementation:
  <ol>
   <li> Find the first .oklib-directory on the path upwards. </li>
   <li> Call make in the current directory, with OKplatform defined
   and all other arguments forwarded. </li>
   <li> The output of make is copied to
   system_directories/log/GenericOutput.txt (this filename is a
   configuration-variable) resp. to ExternalSourcesOutput.txt. </li>
   <li> I (OK) don't think that one needs to distinguish between
   the generic makefile and the various recursive makefiles --- this
   distinction will likely vanish anyway. </li>
  </ol>
  Once oklib is available, the temporary definitions of OKplatform etc.
  in the makefiles shall be removed.


  \todo Further enhancements:
  <ul>
   <li> --help </li>
  </ul>

*/

