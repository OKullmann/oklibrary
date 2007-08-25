// Oliver Kullmann, 25.8.2007 (Swansea)

/*!
  \file Buildsystem/docus/general.hpp
  \brief The central docus-file for the buildsystem

  <h1> Overview on the buildsystem </h1>

  The build-system is called via <code>oklib</code> (Buildsystem/MasterScript/docus/general.hpp), which
  delegates the targets to the makefiles documented here:
  <ul>
   <li> External sources building : Buildsystem/ExternalSources/docus/general.hpp </li>
   <li> %OKlib building : Buildsystem/docus/generic.hpp </li>
  </ul>

  For the very initial set-up:
  <ol>
   <li> Call first <code>oklib --setup</code>, </li>
   <li> then call <code>oklib --prebuild</code>. </li>
  </ol>
  If the path to <code>oklib</code> has not been set yet, then use
  <code>$(oklib_call)</code>.

*/


