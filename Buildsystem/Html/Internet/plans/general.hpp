// Oliver Kullmann, 24.5.2007 (Swansea)

/*!
  \file Buildsystem/Html/Internet/plans/general.hpp
  \brief Plans for the internet services (web page, etc.)


  \todo Content of the Internet-page:
  <ul>
   <li> Download of the software:
    <ol>
     <li> The current development-package (core), that is,
     the skeleton directory structure of OKplatform, with
     Transitional/OKlib filled in. </li>
     <li> Current external sources to download.
      <ol>
       <li> Package data (see Buildsystem/plans/PackageBuilding.hpp) </li>
       <li> The full current package. </li>
      </ol>
     </li>
     <li> The installed docs-package. </li>
     <li> The full package (with core, external sources and docs). </li>
     <li> The package history
      <ol>
       <li> With SHA's to identify git-repository-states. </li>
       <li> And md5sum's of packages. </li>
       <li> And the packages themselves (only the cores). </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Access to mailing-lists (with instructions). </li>
   <li> Online-documentation (the local home page):
    <ol>
     <li> Using
     \verbatim
internet_dir := /usr/local/apache2/htdocs/OKlib
     \endverbatim
     in "override.mak", we get the Internet home page after
     <code> oklib internet </code> at
     \verbatim
http://localhost/OKlib/internet_html/doc/internet_html/index.html
     \endverbatim
     </li>
     <li> htdocs/OKlib perhaps is best made owned by root (as
     the creator, with group "users", and group-writable. </li>
     <li> For the Internet page, the directories should better be made
     non-readable. </li>
     <li> DONE (corrected this by using the configuration system)
     Slightly awkward the double nesting of the index page,
     but it reflects that building of the internet home page
     happened in the environment, where the new doc-directory
     is not "system_directories/doc", but
     "system_directories/doc/internet_html/doc" (for movability).
     And it will be hidden anyway by ok-sat-library.org. </li>
    </ol>
   </li>
   <li> Perhaps a web-interace to the repository. </li>
   <li> A project-page (for example, Heribert Vollmer might have a student
   project about implementing the NP-completeness of SAT (encoding of
   Turing machines)) --- but this perhaps should go to the local system? </li>
   <li> DONE (reports are available on the history page, as part of the
   local documentation, which is provided via a link)
   The EPSRC final report (short and long version) should be made
   available. It should be accessible via the local documentation as well
   as via the Internet.. So perhaps the Internet has some shortcut to
   the pure html-parts of the local documentation? </li>
  </ul>


  \todo Building the Internet-page:
  <ul>
   <li> The process should be fully automatic, including
   updating the packages etc. --- no alterations by hand
   should be necessary! </li>
   <li> We copy the internet-html-directory to its final place;
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


  \todo Domain name
  <ul>
   <li> Is the "cloak" method good, or shouldn't we better use redirection?
    <ol>
     <li> If no new pages are created, then the visible address always the same,
     making it harder to create links. (This is as with frames.) </li>
     <li> On the other hand, having only the "public" url visible means
     less confusion with changing "implementation" url's. </li>
    </ol>
    Or can we solve the problem by using the DNS-sverice?
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

