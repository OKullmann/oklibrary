// Oliver Kullmann, 24.5.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Html/Internet/plans/general.hpp
  \brief Plans for the internet services (web page, etc.)


  \todo Checking all links
  <ul>
   <li> We need to run from time to time a tool for checking
   the links on the internet html pages. </li>
  </ul>


  \todo Extended content of the Internet-page:
  <ul>
   <li> A link with explanations to the public repository at github. </li>
   <li> Updates of the explanations on the release process (emphasise on
   github as the public interface, through which participation happens). </li>
   <li> Access to mailing-lists (with instructions). </li>
   <li> A project-page (for example, Heribert Vollmer might have a student
   project about implementing the NP-completeness of SAT (encoding of
   Turing machines)) --- but this perhaps should go to the local system? </li>
   <li> DONE
   Creating a link to the download area for ExternalSources etc. </li>
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

