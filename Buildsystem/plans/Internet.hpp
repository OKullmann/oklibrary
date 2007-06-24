// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/Internet.hpp
  \brief Plans for the internet services (web page, etc.)

  \todo External vs. internal html-pages:
  We have the real Internet-pages, which will be accessible from the www,
  and we have the internal html-pages, documenting the platform.
  We need to clarify the relation to
  <ol>
   <li> Buildsystem/Generic/documentation_building/plans/documentation_building.hpp </li>
   <li> Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp </li>
  </ol>
  Perhaps the name "Internet.hpp" is not fully approrpiate?


  \todo Html-Directory:
  <ul>
   <li> We should have a new directory Buildsystem/html, where all
   html-page-templates (to be processed by the preprocessor) go. </li>
   <li> Or perhaps "Buildsystem/Html", since it is not a "systematic"
   directory (found also at other places)? </li>
   <li> And also all related material like pictures etc. </li>
  </ul>


  \todo Configuration problem:
  <ul>
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
   <li> A project-page (for example Heribert Vollmer might have a student
   project about implementing the NP-completeness of SAT (encoding of
   Turing machines)). </li>
  </ul>


  \todo Building the Internet-page:
  <ul>
   <li> A new target "Internet". </li>
   <li> The build-process copies all relevant files to the
   location where the web server accesses the pages. </li>
   <li> The process should be fully automatic, including
   updating the packages etc. --- no alterations by hand
   should be necessary! </li>
  </ul>

*/

