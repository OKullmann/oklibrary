// Oliver Kullmann, 24.5.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Html/Internet/plans/general.hpp
  \brief Plans for the internet services (web page, etc.)


  \todo Building the Internet-page: DONE
  <ul>
   <li> DONE (a simple target for moving the packages has been provided,
   which should suffice for now (hard to see how to automatise more (sensibly)))
   The process should be fully automatic, including
   updating the packages etc. --- no alterations by hand
   should be necessary! </li>
   <li> DONE (the policy is to be developed; see Buildsystem/ReleaseProcess/plans/general.hpp)
   For download, simply move the content of
   system_directories/packages to an accessible directory,
   and make this directory readable (so that packages can
   be downloaded). According to some policy to be developed,
   remove packages from time to time which do not represent
   official releases. </li>
   <li> DONE We copy the internet-html-directory to its final place;
   uses a lot of space, but seems the best solution. </li>
   <li> DONE (we do not remove old doxygen-html-pages anymore)
   Apparently the sub-process running oklib uses a less
   powerful shell, admitting fewer arguments to rm, and thus
   the removal of old doxygen-files failes:
    <ol>
     <li> How to get bash involved? </li>
     <li> Or shall we ignore it? Apparently it's not needed?
     Perhaps we should drop the removal at all? </li>
    </ol>
   </li>
   <li> The build-process copies all relevant files to the
   location where the web server accesses the pages : DONE
   (all local documentation now available (in principle)) </li>
   <li> A new target "Internet" : DONE </li>
  </ul>


  \todo Extended content of the Internet-page:
  <ul>
   <li> Access to mailing-lists (with instructions). </li>
   <li> Perhaps a web-interface to the repository. </li>
   <li> A project-page (for example, Heribert Vollmer might have a student
   project about implementing the NP-completeness of SAT (encoding of
   Turing machines)) --- but this perhaps should go to the local system? </li>
  </ul>


  \todo Domain name
  <ul>
   <li> Is the "cloak" method good, or shouldn't we better use redirection?
    <ol>
     <li> If no new pages are created, then the visible address always the same,
     making it harder to create links. (This is as with frames.) </li>
     <li> On the other hand, having only the "public" url visible means
     less confusion with changing "implementation" url's. </li>
    </ol>
    Or can we solve the problem by using the DNS-service?
   </li>
   <li> DONE (yes, such redirection is possible; see
   Buildsystem/ReleaseProcess/plans/MailingLists.hpp)
   Do we need e-mail-forwarding? Can we have
    <ol>
     <li> developers@ok-sat-library.org </li>
     <li> core-developers@ok-sat-library.org </li>
     <li> users@ok-sat-library.org </li>
     <li> announcements@ok-sat-library.org </li>
    </ol>
   </li>
   <li> How to host ok-sat-library.org on our machines DONE (set html-forwarding
   to the local address in Swansea, using the "cloak" method) </li>
   <li> DONE (OK acquired "ok-sat-library.org" for 10 years, which seems
   to include "www.ok-sat-library.org" as well as just "ok-sat-library.org")
   It sems best to acquire a domain name like
    <ol>
     <li> www. oklibrary.edu </li>
     <li> www. oklibrary.org </li>
    </ol>
   </li>
   <li> How to become owner of such a domain name? DONE </li>
  </ul>


*/

