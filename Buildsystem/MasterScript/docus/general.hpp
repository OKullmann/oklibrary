// Oliver Kullmann, 25.8.2007 (Swansea)

/*!
  \file Buildsystem/MasterScript/docus/general.hpp
  \brief Documentation on the program for executing build-actions

  <h1> The masterscript "oklib" </h1>

  Calls to <code>oklib</code> depend on the calling directory.
  if you are in a different directory than the one from which
  you want to call <code>oklib</code>, but you do not want
  to change the working directory, then use
  <code>(cd target-directory; oklib)</code>.

  <p></p> Options:
  <ul>
   <li> <code>--version</code> </li>
   <li> <code>--setup</code> </li>
   <li> <code>--prebuild</code> </li>
  </ul>

  Otherwise all targets are delegated to the respective makefiles.

*/

