// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/Internet.hpp
  \brief Plans for the internet services (web page, etc.)

  \todo Configuration problem:
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
    don't have to learn anything new (and what we learn is relevant
    for us at other places (!)) --- so I (OK) am in favour of this
    solution. </li>
    <li> The next level of power would be m4 --- this is available on Linux
    and Unix systems (current version is 1.4.9, but the installed versions
    should be fine). </li>
    <li> Still more powerfull is hsc (http://www.linguistik.uni-erlangen.de/~msbethke/software.html).
     <ol>
      <li> Is this system mature? </li>
      <li> Is the above basic task (macro usage) easily accomplished? </li>
      <li> In the long run, using such a system could have advantages. hsc is just
      C, so it's easy to compile. </li>
      <li> Another advantage, of course, is that it understands html. But we have to learn
      another language (which we cannot use for something else). </li>
     </ol>
    </li>
   </ol>
   </li>
   <li> Should we use .css-styles, or plain html-pages?
   See Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp. </li>
   <li> The web-pages perhaps could be written by some system (using some
   higher-level language?). DONE (we should write the web pages by hand (at least at this
   time) --- it's much easier, and we can use any html-element we want)
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

