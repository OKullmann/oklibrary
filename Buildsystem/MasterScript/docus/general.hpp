// Oliver Kullmann, 25.8.2007 (Swansea)

/*!
  \file Buildsystem/MasterScript/docus/general.hpp
  \brief Documentation on the program for executing build-actions

  <h1> The masterscript "oklib" </h1>


  <h2> The underlying algorithm </h2>

  Calls to <code>oklib</code> depend on the calling directory:
  <ul>
   <li> Starting with the calling directory, all directories on the
   path to the enclosing OKplatform-directory (defined by having
   a <code>.oklib</code> subdirectory) are considered, and the first
   time a directory D is recognised defines this directory as determinative
   for the makefile to call:
    <ol>
     <li> If D is "ExternalSources" then external sources building (see
     Buildsystem/ExternalSources/docus/general.hpp) is invoked. </li>
     <li> If D contains a file "definitions.mak" then the generic
     makefile for compiling the components of the library is invoked
     (see Buildsystem/docus/generic.hpp). </li>
     <li> If D is "Transitional" then a recursive makefile is invoked,
     calling the generic makefiles at lower levels. </li>
     <li> If D is "OKsystem" then another recursive makefile is invoked,
     this time designed to call different types of makefiles for
     the different repositories placed at OKsystem-level. </li>
     <li> At OKplatform-level currently there are no build-actions
     (other than given by the options below). </li>
    </ol>
    <li> If you are in a different directory than the one from which
    you want to call <code>oklib</code>, but you do not want
    to change the working directory, then use
    <code>(cd target-directory; oklib)</code>. </li>
  </ul>
  
  
  <h2> Options </h2>

  <ul>
   <li> <code>--version</code> </li>
   <li> <code>--setup</code> </li>
   <li> <code>--prebuild</code> </li>
  </ul>

  Otherwise all targets are delegated to the respective makefiles.

*/

