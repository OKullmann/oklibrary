// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/Internet.hpp
  \brief Plans for the internet services (web page, etc.)

  \todo Configuration problem:
  <ul>
   <li> A central problem is: How to create a central repository
   of configuration data, usable by the various web pages as well
   as by the build system?! </li>
   <li> We need a simple system which allows
   to add invisible variables to the html-pages, whose visible values
   are updated by running some program (which has the list of current
   values). Then we can just write plain html-pages, and don't have
   to bother about constructing them with make-commands.
   <ol>
    <li> The solution is an html preprocessor (see http://htmlhelp.com/links/preprocessors.html). </li>
    <li> The simplest case is just to use the C preprocessor (see
    http://www.cs.tut.fi/~jkorpela/html/cpre.html)! This looks
    rather attractive --- by using includes or by using macros we can
    easily add some dynamic content. The main advantage is, that we
    don't have to learn anything new --- so I (OK) am in favour of this
    solution. </li>
    <li> The next level of power would be m4 --- this is available on Linux
    and Unix systems (current version is 1.4.9, but the installed versions
    should be fine). </li>
    <li> Still more powerfull is hsc (http://www.linguistik.uni-erlangen.de/~msbethke/software.html). </li>
   </ol>
   </li>
   <li> Perhaps a new subdirectory Buildsystem/Configuration? </li>
   <li> Is CMake right for translating the configuration data into
   the various file formats? </li>
   <li> Should we use .css-styles, or plain html-pages?
   See Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp. </li>
   <li> The web-pages perhaps could be written by some system (using some
   higher-level language?).
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
   see Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp). </li>
  </ul>

*/

