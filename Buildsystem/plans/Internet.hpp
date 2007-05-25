// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/Internet.hpp
  \brief Plans for the internet services (web page, etc.)

  \todo Configuration problem:
  <ul>
   <li> A central problem is: How to create a central repository
   of configuration data, usable by the various web pages as well
   as by the build system?! </li>
   <li> Perhaps a new subdirectory Buildsystem/Configuration? </li>
   <li> Is CMake right for translating the configuration data into
   the various file formats? </li>
   <li> Should we use .css-styles, or plain html-pages?
   See Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp. </li>
   <li> The web-pages perhaps should be written by some system.
   Possible choices:
    <ol>
     <li> asciidoc ?? </li>
     <li> doxygen ?? </li>
    </ol>
  </ul>

  \todo Content of the Internet-page:
  <ul>
   <li> Download of the software </li>
   <li> Access to mailing-lists. </li>
   <li> Online-documentation (just the documentation-index-page;
   see Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp. </li>
  </ul>

*/

