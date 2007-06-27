// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/MailingLists.hpp
  \brief Plans for the mailing lists


  \todo Mailing lists:
  <ul>
   <li> Set up the mailman system on cs-wsok.
    <ol>
     <li> See "Mailman" in Buildsystem/ExternalSources/plans/general.hpp </li>
    </ol>
   </li>
   <li> Lists:
    <ol>
     <li> Developers
      <ul>
       <li> Only developer-e-mails </li>
      </ul>
      A developer is automatically member of "Git-notifications developers"
     </li>
     <li> Users
      <ul>
       <li> Only user-e-mails </li>
      </ul>
      A user is automatically member of "General notifications".
     </li>
     <li> General notifications
      <ul>
       <li> Only special notifications, when new versions are reached etc. </li>
      </ul>
     </li>
     <li> Git-notifications developers
      <ul>
       <li> Exactly all the Git-e-mail notifications for the central repository </li>
      </ul>
     </li>
     <li> Git-notifications users
      <ul>
       <li> Exactly all the Git-e-mail notifications for the user-clone </li>
      </ul>
     </li>
    </ol>
    Every e-mail is sent to exactly one list.
   </li>
   <li> Is it possible to (automatically) show the list of users? </li>
   <li> Yet the Internet-address is "secret". </li>
   <li> We have a web-page explaining the use of the mailing-lists. </li>
   <li> Which part of the mailing-list-system is under version control?
    <ol>
     <li> The mailing-lists-archives are not under version control, but they are backed-up (by the
     departmental back-up on cs-svr1). </li>
     <li> The web-pages and related configuration data is part of the OKlibrary. </li>
     <li> Also the list of developers is part of the OKlibrary. </li>
     <li> What about the list of users? </li>
    </ol>
   </li>
  </ul>


  \todo User mailing list
  <ul>
  <li> Invite the participants (roughly in the following order --- slowly going on):
  <ol>
   <li> Ines Lynce and Joao Marques-Silva: General (?) </li>
   <li> Further collaborators of Ines Lynce and Joao Marques-Silva </li>
   <li> John Franco and Sean Weaver: General (?) </li>
   <li> Hans van Maaren and Marijn Heule: General (?) </li>
   <li> Arnold Beckmann and Klaus Aehlig: ?? </li>
   <li> Ulrich Berger: New module Formalisation </li>
   <li> Markus Roggenbach </li>
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
   <li> Marina de Vos and Martin (Bath) </li>
   <li> Tobias Nipkow </li>
   <li> Uwe Schoening </li>
   <li> Sam Buss (asking very politely) </li>
   <li> Phokion Kolaitis </li>
   <li> Andras Salomon </li>
   <li> Lars Schewe </li>
   <li> Steve Cook and group (theoretical aspects) </li>
   <li> Alasdair Urquhart and group (theoretical aspects) </li>
   <li> Moshe Vardi </li>
  </ol>
  </li>
  <li> Contact e-mail:
   <ol>
    <li> Yet we do not have much resources, but with their help we want to ask for
    further funding. </li>
    <li> Yet please do not publish the web-address at other places (public release
    in 6 months). </li>
    <li> Main request: State your wishes. </li>
    <li> Please stay for some time on the user-list (you can ask for daily
    summaries); if too much "noise", then there is the notification list. </li>
    <li> If you wish to submit, then you need to become member of the developpers
    list. </li>
    <li> It is a "proto release" --- larger changes possible (for example on request
    of these first users(!)). </li>
   </ol>
   We need an official text, and OK contacts each of the above personally,
   adding this text.
  </li>
  <li> Only by invitation (yet). </li>
  </ul>

  \todo Instructions:
  A dedicated weg-page is needed on howto use the mailing-lists
  <ul>
   <li> Subject-lines in e-mails: "precise and short specifications", no nonsense
   like "newbie needs help". </li>
   <li> About the identity of developers. </li>
   <li> "How to reply". </li>
  </ul>

*/

