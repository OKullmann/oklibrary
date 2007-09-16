// Oliver Kullmann, 5.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Html/plans/general.hpp
  \brief General plans regarding the html-services


  \todo General rules for html-pages
  <ol>
   <li> We concentrate first solely on the content, presented in a simple
   and clear way. </li>
   <li> Presentation issues are only considered after the pages have
   been used for a while, and the system stabilised. </li>
   <li> These two basic rules are reflected by the technical requirements,
   that strict XHTML is used (thus deprecated features cannot be used,
   that is, representational issues are suppressed), and that appearance
   is handled via external style sheets, but only at a later point,
   after the development has settled. </li>
   <li> In general we want to leave as much details as possible to
   the browser, so that it can adapt the appearance to the users wishes.
   We are not an enterprise which needs to catch attraction by penetrating
   effects. </li>
  </ol>


  \todo Install configuration system
  <ul>
   <li> Update all usages of absolute paths to our own html-pages or
   to doxygen-pages. </li>
   <li> Also all links to ExternalSources-docs need to be relativised,
   since otherwise apache doesn't understand the url's.
    <ol>
     <li> We need to check, whether this works with ext-sources-doc and
     sys-dir-doc set to their target locations. </li>
     <li> However, then actually for normal use the documentation is
     not movable, since normally the documentation is placed in
     ExternalSources/doc, not in system_directories/doc. </li>
     <li> So better for external-sources-links we have the choice
     between absolute and relative paths, via a make-variable;
     default is absolute, while for creating the internet-html-directory
     it is switched to relative. </li>
    </ol>
   </li>
   <li> The m4-preprocessing should move to the Configuration-module. </li>
   <li> Files created by the preprocessor should have a final line stating this
   and the creation date; then also (for easy of modification) the original
   template file should be specified (in a comment, or on the page).
   The local home page has already a first solution, which should be systematised, that
   is, a make-variable should be provided. Perhaps we just add "from ...", and
   state the original template file also on the page; the new make-variable then
   just contains the text with date etc.:
   \verbatim
automatic_masthead = Automatically created by the OKbuildsystem on $(current_date) from \
  template file "
   \endverbatim
   where then the template file has to be added (by hand).
   One problem here is that the current date should be the time when the specific page
   was created, not when the whole process started --- is this achieved by recursive
   make variables??
   </li>
   <li> DONE
   Local url's should be relative (so that the html-documentation is
   moveable): The preprocessing approach needs to be generalised:
    <ol>
     <li> DONE (we simply precompute all relative paths; otherwise
     the algorithm is implemented as described)
     Best seems to create a function called by the m4-preprocessor
     (perhaps called "m4_RELPATH(C,T)"), which takes the current location C
     and the target location T, all as absolute paths, and creates
     from that the relative path from C to T:
      <ol>
       <li> The algorithm is, that the longest common prefix (path) P
       of C and T is determined, and then the relative path from C
       to T goes first (size(C) - size(P))-many steps up, and then uses
       the piece of T starting with the end of P. </li>
       <li> Here I used paths as sequences in the C++ sense, and it
       seems best to implement this task as a little C++ application
       (there appears to be no existing tool for that purpose). </li>
       <li> Since all path names are constructed by us, we take their
       representation (as strings) literal, that is, do not use
       the equivalence of paths, but their equality (in therms of
       the Boost filesystem library, part of the standard in the future). </li>
       <li> A generic algorithm determines the longest prefix of
       two sequences (given by input iterators). </li>
      </ol>
     </li>
     <li> The problem is how to get the current location? </li>
     <li> For html-files created by us we know their location
     (that is, where they will be put after preprocessing. Since we
     actually have make-variables (in the configuration system) for this,
     here we shouldn't have a problem. </li>
     <li> But what about files created by doxygen? As we do it now, we
     need a make-variable (in the configuration system) with the absolute
     address (to be manually read-off), which should also be alright. </li>
     <li> So all make-variables with url's contain absolute paths,
     while all created html-files (our's or by doxygen) contain relative
     paths. </li>
     <li> There are two types of html-files, our own and doxygen-created,
     and accordingly four types of links:
      <ul>
       <li> From our own files to other own files or to doxygen files
       we use the system with the make-variables as discussed above. </li>
       <li> From doxygen-files to other doxygen-files we use the
       doxygen ability to handle parts of path names. </li>
       <li> From doxygen-files to own files, there is the doxygen-capability
       of creating links via the "\link" command, but this seems to
       require a hard-coded path. Thus also here we use the above
       mechanism, together with the tag-construction, which creates
       the html-link-tag: Problematic, that only variable values
       can be used (no computations) ? Thus likely we have to compute
       these paths in advance, and store them in (make-)variables. </li>
      </ul>
     </li>
     <li> DONE (created Configuration/Html/relative_paths.mak)
     It would be easiest if all relative addresses would be computed
     in advance in stored in make-configuration variables. Then better
     we create (in Configuration/Html) a specific makefile with all
     these settings, and this configuration makefile is only included
     by OKlibBuilding/Targets/html/Makefile. </li>
    </ol>
   </li>
   <li> See solution to "Configuration problem" below. DONE (the general usage
   is clear) </li>
  </ul>


  \todo Configuration problem:  DONE
   (decided to use the m4-system, since it is always available, it is simple
   to use, and if php is needed later, then we can use both systems)
  <ul>
   <li> We need a simple system which allows
   to add invisible variables to the html-pages, whose visible values
   are updated by running some program (which has the list of current
   values). Then we can just write plain html-pages, and don't have
   to bother about constructing them with make-commands. The solution is an
   html preprocessor (see http://htmlhelp.com/links/preprocessors.html).
   <ol>
    <li> m4 would work --- this is available on Linux
    and Unix systems (current version is 1.4.9, but the installed versions
    should be fine). DONE (to be used as described below)
     <ol>
      <li> According to "Configuration data format" in
      Buildsystem/Configuration/plans/general.hpp all configuration data is
      available as environment variables (at build-time). </li>
      <li> Via
      \verbatim
m4_define(`m4_SHELL', `m4_esyscmd(echo -n ${$1})')m4_dnl
      \endverbatim
      in a file "m4_shell_macro" we get an m4-macro which makes shell-access easy. </li>
      <li> Assume that in "configuration.mak" we have configuration-variable like
      \verbatim
PATH2=XXXXXXXXXXXXXXXXXXXXXXXXXXXX
      \endverbatim
      </li>
      <li> Assume furthermore that in "file.html" we have
      \verbatim
hdhdhd m4_SHELL(PATH2)
dkdkdl
m4_SHELL(HOME) djdjdd
      \endverbatim
      (this is the file in which macro-substitution shall take place). </li>
      <li> Furthermore we need the makefile
      \verbatim
include configuration.mak
export
all :
        m4 --prefix-builtins m4_shell_macro file.html
      \endverbatim
      </li>
      <li> Now "make all" yields
      \verbatim
m4 --prefix-builtins m4_shell_macro file.html
hdhdhd XXXXXXXXXXXXXXXXXXXXXXXXXXXX
dkdkdl
/home/kullmann djdjdd
      \endverbatim
      </li>
     </ol>
    </li>
    <li> Apparently php can also be used as a preprocessor? DONE (not used now; possibly later)
     <ol>
      <li> It seems that for using the doxygen-search-function we need to
      install php anyway (see "Search engine" in Buildsystem/OKlibBuilding/Targets/html/plans/general.hpp). </li>
      <li> Could php be useful for other stuff in the future? </li>
     </ol>
    </li>
    <li> More powerfull is hsc (http://www.linguistik.uni-erlangen.de/~msbethke/software.html). DONE (has a bit more power, but not enough; and the somewhat waci tone
    is unprofessional)
     <ol>
      <li> Is this system mature? </li>
      <li> Is the above basic task (macro usage) easily accomplished? </li>
      <li> In the long run, using such a system could have advantages. hsc is just
      C, so it's easy to compile. </li>
      <li> Another advantage, of course, is that it understands html. But we have to learn
      another language (which we cannot use for something else). </li>
    <li> The simplest case is just to use the C preprocessor (see
    http://www.cs.tut.fi/~jkorpela/html/cpre.html)! This looks
    rather attractive --- by using includes or by using macros we can
    easily add some dynamic content. The main advantage is, that we
    don't have to learn anything new (and what we learn is relevant
    for us at other places (!)). However we get then these errors
    --- not enough control. DONE</li>
     </ol>
    </li>
   </ol>
   </li>
   <li> Should we use .css-styles, or plain html-pages?
   See Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp. DONE (no and yes --- see "General rules for html-pages") </li>
   <li> The web-pages perhaps could be written by some system (using some
   higher-level language?). DONE (we should write the web pages by hand (at least at this
   time) --- it's much easier, and we can use any html-element we want)
   Possible choices:
    <ol>
     <li> asciidoc ?? </li>
     <li> doxygen ?? </li>
    </ol>
  </ul>


  \todo External vs. internal html-pages: DONE (the local home page
  is handled in in this module, at Buildsystem/Html/plans/LocalHomePage.hpp,
  while the doxygen-documentation-building is a different thing, discussed in
  OKlib/Targets/html/plans)
  We have the real Internet-pages, which will be accessible from the www,
  and we have the internal html-pages, documenting the platform.
  We need to clarify the relation to
  <ol>
   <li> Buildsystem/Generic/documentation_building/plans/documentation_building.hpp </li>
   <li> Buildsystem/Generic/documentation_building/plans/documentation_index.hpp </li>
  </ol>
  Perhaps the name "Internet.hpp" is not fully appropriate?


  \todo Html-Directory: DONE
  <ul>
   <li> We should have a new directory Buildsystem/html, where all
   html-page-templates (to be processed by the preprocessor) go. </li>
   <li> Or perhaps "Buildsystem/Html", since it is not a "systematic"
   directory (found also at other places)? </li>
   <li> And also all related material like pictures etc. </li>
  </ul>

*/

