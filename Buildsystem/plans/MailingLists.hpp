// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/plans/MailingLists.hpp
  \brief Plans for the mailing lists

  \todo Mailing lists:
  <ul>
   <li> Set up the mailman system on cs-wsok.
    <ol>
     <li> See "Mailman" in Buildsystem/plans/ExternalSources.hpp </li>
    </ol>
   </li>
   <li> At least two lists: "Developers" and "Users". </li>
   <li> Perhaps also a list where only general notifications are sent to. </li>
   <li> And some list where only the Git-notifications are sent to. </li>
   <li> Is it possible to (automatically) show the list of users? </li>
   <li> Yet the Internet-address is "secret". </li>
   <li> We have a web-page explaining the use of the mailing-lists. </li>
  </ul>

  \todo User mailing list
  <ul>
  <li> Invite the participants:
  <ul>
   <li> Joshua Buresh-Oppenheim: module Resolution </li>
   <li> Stefan Szeider and Marko Samer: new module Backdoors </li>
   <li> Hans van Maaren and Marijn Heule: General (?) </li>
   <li> John Franco and Sean Weaver: General (?) </li>
   <li> Ines Lynce and Joao Marques-Silva: General (?) </li>
   <li> Further collaborators of Ines Lynce and Joao Marques-Silva </li>
   <li> Daniel Le Berre (observer) </li>
   <li> Zumkeller: ?? </li>
   <li> Arnold Beckmann and Klaus Aehlig: ?? </li>
   <li> Xishun Zhao: ?? </li>
   <li> Victor Marek and Mirek Truszczynski: ?? </li>
   <li> Ulrich Berger: New module Formalisation </li>
   <li> Marina de Vos and Martin (Bath) </li>
   <li> Sam Buss (asking very politely) </li>
   <li> Tobias Nipkow </li>
   <li> Markus Roggenbach </li>
   <li> David Mitchell (observer) </li>
   <li> Holger Hoos and David (observer) </li>
   <li> Uwe Schoening </li>
   <li> Elitza Maneva </li>
   <li> Heribert Vollmer </li>
   <li> Hans Kleine Buening, Thorsten Lettmann, Uwe Bubeck </li>
   <li> Enrico Giunchiglia </li>
   <li> Phil Grant </li>
   <li> Andras Salomon </li>
   <li> Peter Jonsson and group </li>
   <li> Jakob Nordstroem </li>
   <li> Steve Prestwich </li>
   <li> Nicola Galesi and group </li>
   <li> Maria-Luisa Bonet and group </li>
  </ul>
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
   </ol>
  </li>
  <li> Only by invitation (yet). </li>
  <li> How can we create special "views" for the users? They should be able to register
  for modules or subjects, and then get commit-notifications related to those modules.
  Sending them also the patches? Or the new files??
  </li>
  <li> An issue is how to separate out the "secret research":
   <ol>
    <li> See Buildsystem/plans/VersionControl.hpp. </li>
    <li> See "Research sub-modules" in Transitional/plans/Transitional.hpp. </li>
   </ol>
  </li>
  <li> More generally the question is, what kind of distribution-package the user get?
   <ol>
    <li> They simply get a clone of the whole library (supposing that we have merged the different
    repositories plus the new ExternalSources-repository into one repository), with doc-directory
    preinstalled (so that they don't need to run anything --- just unpack and see).
    See point "Download possibilities" in Buildsystem/plans/Release.hpp. </li>
    <li> As discussed above, all research-subdirectories are filtered out. </li>
    <li> One master-user-clone is created on cs-oksvr, and the users get clones of it as
    described in "Copied clones which know how to connect" in Buildsystem/plans/VersionControl.hpp. </li>
   </ol>
  </li>
  </ul>

  \todo Notification list:
  We had a list with names and addresses of people interested in the library,
  which shall be informed personally about the release --- where is this
  gone ??? Perhaps with Git we can search for "Sam Buss" ?!
  See "Building and using Git" in Buildsystem/plans/VersionControl.hpp.

  \todo Mailing list software : DONE (decided to use "Mailman")
  <ul>
   <li> We need to set up an OKlibrary mailing list with web interface. </li>
   <li> "Mailman" http://www.gnu.org/software/mailman/ seems to be a good choice ?! </li>
  </ul>

*/

