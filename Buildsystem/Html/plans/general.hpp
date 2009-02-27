// Oliver Kullmann, 5.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Html/plans/general.hpp
  \brief General plans regarding the html-services


  \todo Link checking
  <ul>
   <li> A tool is needed which can follow all links and checks, for the
   local home page and for the Internet home page, whether they all
   work (returning the list of dysfunctional links). </li>
  </ul>


  \todo General rules for html-pages
  <ol>
   <li> We concentrate first solely on the content, presented in a simple
   and clear way. </li>
   <li> Indentation style:
    <ol>
     <li> The standard xemacs indentation style doesn't work well;
     perhaps it doesn't understand xhtml? </li>
     <li> Setting the indentation step to 1 (instead of 2, the default)
     helps to reduce the amount of indentation. </li>
     <li> However, how to configure xemacs so that this is the default?? </li>
     <li> When in html-mode, xemacs has a menu-entry HTML -> File Options ->
     Indent Step, which by default is 2, and which should be set to 1. </li>
     <li> One can demand to store the settings into a file, however,
     this stores in the html-file --- which is unacceptable. </li>
     <li> The variable settings seem to be
     \verbatim
(setq sgml-indent-data t)
(setq sgml-indent-step 1)
     \endverbatim
     however putting this into custom.el doesn't make any difference? </li>
    </ol>
   </li>
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
   <li> We need documentation on this system for handling paths in
   html-documents. </li>
   <li> Update all usages of absolute paths to our own html-pages or
   to doxygen-pages. </li>
   <li> The m4-preprocessing should move to the Configuration-module. </li>
   <li> Files created by the preprocessor should have a final line stating this
   and the creation date; then also (for easy of modification) the original
   template file should be specified (in a comment, or on the page).
   The local home page has already a first solution, which should be
   systematised, that is, a make-variable should be provided. Perhaps we just
   add "from ...", and state the original template file also on the page; the
   new make-variable then just contains the text with date etc.:
   \verbatim
automatic_masthead = Automatically created by the OKbuildsystem on $(current_date) from \
  template file "
   \endverbatim
   where then the template file has to be added (by hand). One problem here is
   that the current date should be the time when the specific page was
   created, not when the whole process started --- is this achieved by
   recursive make variables??
   </li>
   <li> DONE (introduced makevariable extsrc_relative, which if non-empty
   activates relative paths to external-sources-documentation)
   Also all links to ExternalSources-docs need to be relativised,
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
   <li> DONE
   Local url's should be relative (so that the html-documentation is
   movable): The preprocessing approach needs to be generalised:
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
       the equivalence of paths, but their equality (in terms of
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


  \todo Search engines
  <ul>
   <li> See "Meta tags" in Buildsystem/Html/Local/plans/general.hpp. </li>
   <li> The question is about whether search engines should be allowed
   to index the documentation. </li>
   <li> We need to distinguish between external documentation and
   OKlib-documentation, and between the main OKlibrary-website and websites
   of users. </li>
   <li> Perhaps the index pages for external documentation (e.g., documentation
   for Maxima or Boost) should be blocked, so that they are not indexed by
   search engines. </li>
   <li> This is requested by the Gecode project (to avoid that a web search
   will lead to stale documentation). </li>
   <li> The other web-pages (doxygen-created and "handmade") should invite
   search machines at the OKlibrary-website, but perhaps for the same reason
   they should block search engines at a user's site. </li>
   <li> So index pages for external sources should obtain their "negative"
   meta-tags. </li>
   <li> And our handmade html-pages as well as the doxygen-pages should
   obtain the meta-tages in the form of an m4-macro, which turns them on
   or off. </li>
   <li> More precisely, turning on happens by
   \verbatim
<meta name="robot" content="all" />
<meta "keywords" content="XXX, YYY" />
   \endverbatim
   while turning off apparently happens perhaps by
   \verbatim
<meta name="robot" content="none" />
   \endverbatim
   <li> For the doxygen-created pages we need to find an appropriate
   doxyygen-template (for the preamble of all created pages). </li>
  </ul>


  \todo Handling of paths
  <ul>
   <li> The current system is rather complex. </li>
   <li> We should seek to simplify it. </li>
  </ul>


  \todo Creating styles
  <ul>
   <li> Carefully, basic style-sheets are introduced. </li>
   <li> The information at the bottom (last change date, and page-author)
   should perhaps be one line, left-aligned the date, right-aligned the
   author. </li>
  </ul>
   
*/

