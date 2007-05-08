// Oliver Kullmann, 10.4.2007 (Swansea)

/*!
  \file Buildsystem/plans/VersionControl.hpp
  \brief Plans for the versioning control system

  \todo Building Git
  <ul>
   <li> Is there html-documentation? </li>
   <li> On csltok asciidoc is not available --- what about cs-wsok? DONE (supplied a build-makefile) </li>
   <li> Perhaps we should just use the default Git-installation into
   the users bin-directory (otherwise it seems hard to use because of
   the many different commands). It should also be possible to do the
   system-wide installion (in /usr/local). DONE (for now just the system-wide installation) </li>
  </ul>

  \todo New version control system
  <ul>
   <li> What about version numbers in Git? What is the
   version-numbering-systems there, and what kind of statistics
   are supported? </li>
   <li> Git can handle symbolic links, so all symbolic links should go
   into the respository.
   </li>
   <li> How to send notification-e-mails with Git?
   </li>
   <li> What about ignoring files in Git? It seems that Git always ignores
   all files not in the respository --- but then we could miss out
   submitting new files?
   </li>
   <li> How to handle change dates and revision numbers in files with Git?
   </li>
   <li> The central repository:
   For a new version control system we have to find out how to establish the role of the repository at
   cs-oksvr as *central*, and how to manage access control (as fine-grained as possible; if possible not
   relying on ssh). </li>
   <li> Models of behaviour for distributed version control:
   Find out how to do the following:
    <ul>
     <li> We have the central repository R and a local repository L. L is changed (with commits, logs
     and everything) to L', while meanwhile R has been changed to R'. Now it should not be possible to
     submit L' to R', since L' is not up-to-date; so first all changes in R' have to be committed to
     L', yielding L'' (without changing R'), and then L'' is submitted to R', yielding R''. </li>
     <li> For the (off-line) laptop csltok: Can we just copy the local repository to disc, then copy this
     to cs-wsok, synchronise it with the local repository on cs-wsok, run the tests, and then submit the
     local repository on cs-wsok to the central repository? Should be no problem. </li>
    </ul>
   </li>
   <li> Branching:
    <ul>
     <li> Yet we never branched; in [{CVU}, vo. 10, no. 2, page 34] it
     is recommended (for Bazaar), that every work on a file (or
     perhaps better on a module?) starts by creating a branch,
     and that (only) after a review the branches are merged into
     the main branch. </li>
     <li> This sounds reasonable; perhaps
     a problem would be, that the review manager (OK) would
     not always be available (and it would also cost him additional
     work). </li>
    </ul>
   </li>
   <li> External sources:
    <ul>
     <li> Optimally, the OKlibrary-package is exactly a clone of the repository (and then one needs to
     run "make" in the top-level directory).
      <ul>
       <li> For this also ExternalSources needs to be in the version control. </li>
       <li> And there needs to be a way to specify "sub-repositories" to separate the public parts
       of the library from the non-public parts. </li>
      </ul>
     </li>
     <li> If we cannot do this, then at least ExternalSources needs to be under version control on its own. </li>
    </ul>
   <li> The different parts in OKplatform:
    <ul>
     <li> As already mentioned in the point "External sources", optimally there is just one repository
     for the whole library (containing the current three repositories Annotations, OKlibrary, Transitional,
     and the new (sub-)repositories ExternalSources and Overview). Then we need the possibility to restrict
     access to parts of the repository (so that annotations and Transitional are not public). </li>
     <li> A problem is, that Annotations currently has public as well as non-public parts; likely this needs to be
     separated. </li>
    </ul>
   </li>
   <li> A distributed version control system as integral part of the library:
    <ul>
     <li> The version control system is built by the library (ExternalSources). </li>
     <li> One has to reflect on how an (external) user of the library might want to
     extend the library (under the version control!). </li>
     <li> Hopefully the distributed version control constitutes also the main part of the
     update-solution for an external user (who extended the library)! </li>
    </ul>
   </li>
   <li> We have a little problems with submissions to the repository, which often span
   many files, so the whole submission process takes a while, and it's not clear from
   outside when it's finished (and the library is again in a well-defined state). DONE (With Git first all submissions are done to a local clone, and only
   once this is all settled, the final push to central respository happens.)
   </li>
   <li> The main conceptual disadvantage (shared with CVS) of Subversion is that
    no local repositories are possible; alternatives: DONE (the three
    existing repositories will be copied over to Git).
    <ol>
     <li> Git (http://git.or.cz/) looks rather good --- one should try it out! </li>
     <li> Bazaar (http://bazaar-vcs.org) was recommended in [{CVU}, vo. 10, no. 2, page 34].
     <li> svk (http://svk.elixus.org), apparently a further development of Subversion.
     Looks somewhat immature. </li>
     <li> What about Arch? </li>
     <li> BitKeeper (http://www.bitkeeper.com) seems to be only proprietary. </li>
    </ol>
   </li>
  </ul>

*/

