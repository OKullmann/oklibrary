// Oliver Kullmann, 24.5.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ReleaseProcess/plans/MailingLists.hpp
  \brief Plans for the mailing lists


  \todo Mailing lists:
  <ul>
   <li> DONE (we use Sourceforge instead)
   Set up the mailman system on cs-wsok.
    <ol>
     <li> See Buildsystem/ExternalSources/SpecialBuilds/plans/Mailman.hpp.
     </li>
    </ol>
   </li>
   <li> The following entries need update (they still refer to the local
   installation of Mailman). </li>
   <li> For the following public e-mail-addresses we need corresponding
   local e-mail-addresses.
    <ol>
     <li> The question is, whether one is sufficient (would be easier)? </li>
     <li> It seems that redirection from ok-sat-library.org to one local e-mail
     account (from which Mailman then gets the mail) works for all different
     mailing lists. </li>
    </ol>
   </li>
   <li> Lists:
    <ol>
     <li> Developers (developers@ok-sat-library.org)
      <ul>
       <li> Created; OK the only administrator. </li>
       <li> Only developer-e-mails. </li>
      </ul>
      A developer is automatically member of "Git-notifications developers".
     </li>
     <li> Core developers (core-developers@ok-sat-library.org)
      <ul>
       <li> Only for core developers. </li>
      </ul>
      A developer is automatically member of "Developers".
     </li>
     <li> Users (users@ok-sat-library.org)
      <ul>
       <li> Only user-e-mails. </li>
      </ul>
      A user is automatically member of "General notifications".
     </li>
     <li> General notifications (announcements@ok-sat-library.org)
      <ul>
       <li> Only special notifications, when new versions are reached etc.
       </li>
      </ul>
     </li>
     <li> Git-notifications developers
      <ul>
       <li> Exactly all the Git-e-mail notifications for the central
       repository </li>
      </ul>
     </li>
     <li> Git-notifications users
      <ul>
       <li> Exactly all the Git-e-mail notifications for the user-pull-clone.
       </li>
       <li> This allerts users about new updates. </li>
      </ul>
     </li>
     <li> Git-notifications user-submissions
      <ul>
       <li> Exactly all the Git-e-mail notifications for the user-push-clones.
       </li>
       <li> This serves also as a certificate of submission. </li>
      </ul>
     </li>
    </ol>
    Every e-mail is sent to exactly one list.
   </li>
   <li> Adminstration
    <ol>
     <li> The automatic "Mailman" mailing list is not listed, and subscription
     needs authorisation. </li>
     <li> The general entry point for all mailing lists is
     http://cs-oksvr.swan.ac.uk/mailman/listinfo/ </li>
     <li> Adminstration access is granted by the specific list-password and by
     the general admin-password. </li>
     <li> The admin-password needs to be kept somewhere secure; since the other
     passwords are sent by e-mails, I've sent an e-mail to myself containing
     this password. </li>
    </ol>
   </li>
   <li> General appearance
    <ol>
     <li> How can we modify the entry page
     http://cs-oksvr.swan.ac.uk/mailman/listinfo/ ? We should mention there
     the OKlibrary. </li>
    </ol>
   </li>
   <li> Which part of the mailing-list-system is under version control?
    <ol>
     <li> The mailing-lists-archives are not under version control, but they
     are backed-up (by the departmental back-up on cs-svr1). </li>
     <li> The web-pages and related configuration data is part of the OKlibrary. </li>
     <li> Also the list of developers is part of the OKlibrary. </li>
     <li> What about the list of users? </li>
    </ol>
   </li>
  </ul>


  \todo Instructions:
  <ul>
   <li> A dedicated web-page is needed on howto use the mailing-lists. </li>
   <li> The meaning of the different mailing lists, and how to use them
   (registration etc.) </li>
   <li> How to access the history. </li>
   <li> About the identity of developers. </li>
   <li> Subject-lines in e-mails: "precise and short specifications", no
   nonsense like "newbie needs help". </li>
   <li> "How to reply". </li>
  </ul>


  \todo User mailing list
  <ul>
   <li> Invite the participants (roughly in the following order --- slowly
   going on). </li>
   <li> Ines Lynce and Joao Marques-Silva: General (?) </li>
   <li> Further collaborators of Ines Lynce and Joao Marques-Silva </li>
   <li> John Franco and Sean Weaver: General (?) </li>
   <li> Hans van Maaren and Marijn Heule </li>
   <li> Arnold Beckmann </li>
   <li> Ulrich Berger: New module Formalisation </li>
   <li> Markus Roggenbach </li>
   <li> Anton Setzer </li>
   <li> Phil Grant </li>
   <li> Elitza Maneva </li>
   <li> Isabelle Oitavem and Reinhard Kahle (especially "theory") </li>
   <li> David Mitchell (observer) </li>
   <li> Holger Hoos and David T (observer) </li>
   <li> Joshua Buresh-Oppenheim: module Resolution </li>
   <li> Stefan Szeider and Marko Samer: new module Backdoors </li>
   <li> Heribert Vollmer </li>
   <li> Hans Kleine Buening, Thorsten Lettmann, Uwe Bubeck </li>
   <li> Enrico Giunchiglia </li>
   <li> Daniel Le Berre (observer) </li>
   <li> Xishun Zhao </li>
   <li> Victor Marek and Mirek Truszczynski: ?? </li>
   <li> Peter Jonsson and group </li>
   <li> Jakob Nordstroem </li>
   <li> Steve Prestwich </li>
   <li> Nicola Galesi and group </li>
   <li> Maria-Luisa Bonet and group </li>
   <li> Nadia Creignou </li>
   <li> Zumkeller: ?? </li>
   <li> Paul Beame and group </li>
   <li> Marina de Vos and Martin Brain (Bath) </li>
   <li> Tobias Nipkow </li>
   <li> Uwe Schoening </li>
   <li> Sam Buss (asking very politely) </li>
   <li> Edward Hirsch and group </li>
   <li> Phokion Kolaitis </li>
   <li> Andras Salomon </li>
   <li> Lars Schewe </li>
   <li> Steve Cook and group (theoretical aspects) </li>
   <li> Alasdair Urquhart and group (theoretical aspects) </li>
   <li> Tonian Pitassi and group </li>
   <li> Moshe Vardi </li>
   <li> Contact e-mail:
    <ol>
    <li> Yet we do not have much resources, but with their help we want to ask
    for further funding. </li>
    <li> Yet please do not publish the web-address at other places (public
    release in 6 months). </li>
    <li> Main request: State your wishes. </li>
    <li> Please stay for some time on the user-list (you can ask for daily
    summaries); if too much "noise", then there is the notification list. </li>
    <li> If you wish to submit, then you need to become member of the
    developpers list. </li>
    <li> It is a "proto release" --- larger changes possible (for example on
    request of these first users(!)). </li>
    </ol>
    We need an official text, and OK contacts each of the above personally,
    adding this text.
   </li>
   <li> Only by invitation (yet). </li>
  </ul>

*/

