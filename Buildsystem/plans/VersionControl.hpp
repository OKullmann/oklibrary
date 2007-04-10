// Oliver Kullmann, 10.4.2007 (Swansea)

/*!
  \file Buildsystem/plans/VersionControl.hpp
  \brief Plans for the versioning control system

  \todo New version control system
  <ul>
   <li> We have a little problems with submissions to the repository, which often span
   many files, so the whole submission process takes a while, and it's not clear from
   outside when it's finished (and the library is again in a well-defined state).
    <ul>
     <li> We need some (e-mail) protocol to handle the situation with CVS. </li>
     <li> Subversion has atomic commits, however it seems that for one commit one can
     only use one log-message, and thus this feature is to weak here to be useful
     (one should check this at the subversion-e-mail list). </li>
     <li> With Git the problem should be easy to solve: All the submissions first
     happen locally, and then, in one go, all changes are submitted to the central
     repository! </li>
    </ul>
   </li>
   <li> The main conceptual disadvantage (shared with CVS) of Subversion is that
    no local repositories are possible; alternatives:
    <ol>
     <li> Git (http://git.or.cz/) looks rather good --- one should try it out! </li>
     <li> Bazaar (http://bazaar-vcs.org) was recommended in [{CVU}, vo. 10, no. 2, page 34].
     <li> svk (http://svk.elixus.org), apparently a further development of Subversion.
     Looks somewhat immature. </li>
     <li> What about Arch? </li>
     <li> BitKeeper (http://www.bitkeeper.com) seems to be only proprietary. </li>
    </ol>
    We should try out Git/Bazaar, in two steps:
     <ol>
      <li> Playing around with it (creating little repositories). (MH, OK, ML) </li>
      <li> Then, in a concentrated effort, the whole OKlibrary is copied to Git/Bazaar, and
      submissions are done for both systems; after a few days we decide which of the
      two systems will be used. </li>
     </ol>
   </li>
   <li> A disadvantage of Subversion compared to CVS is that individual
   files do not have version numbers (these "individual version numbers"
   are better understood as a change-statistics, documenting activity).
    <ul>
     <li> This could easily be changed
      <ul>
       <li> by a new subversion-version </li>
       <li> or by a graphical user interface which determines how often with
       a new version also the considered file has changed. </li>
      </ul>
     One should ask at the subversion-e-mail list (referring to such "file
     version numbers" perhaps as some form of basic statistics). </li>
     <li> What about Git/Bazaar? What is the version-numbering-systems there, and what kind
     of statistics are supported? </li>
    </ul>
   </li>
   <li> CVS cannot handle links:
    <ul>
     <li> Version 1.0.8 of Subversion cannot handle links either --- does this change
     with newer Subversion versions? </li>
     <li> What about Git/Bazaar? </li>
    </ul>
   </li>
   <li> Sending notification-e-mails:
    <ul>
     <li> How to tell the Subversion server to send out e-mails in case of commits?
     "Hook scripts" seem the answer here, especially the commit-email.pl script
     (however it seems that the whole process is not completely straight-forward?). </li>
     <li> How about Git/Bazaar? </li>
    </ul>
   </li>
   <li> Ignoring files:
    <ul>
     <li> Ignoring files is handled by Subversion with the svn:ignore property of
     directories: This property has to be set to (for example) the list of forbidden
     patterns in .cvsignore,
     using
      \code OKplatform/OKsystem/Transitional> svn propset svn:ignore -F ~/.cvsignore * \endcode
      Better, instead of "*" use
      \code $(find * -type d -and -not -path "* /.*") \endcode
     (where the space in the shell pattern needs to be removed ---
     we must avoid ending the comment here!),
     so that in all subdirectories these patterns are ignored). </li>
     <li> What about Git/Bazaar? </li>
    </ul>
   </li>
   <li> Change dates and revision numbers in files:
    <ul>
     <li> In Subversion, the use of $Date and $Revision in macro OKLIB_FILE_ID is
     replaced by
      <ul>
       <li> $LastChangedDate$ </li>
       <li> $LastChangedRevision$ </li>
      </ul>
     and for these files the property svn:keywords has to be set:
      \code svn propset svn:keywords "LastChangedDate LastChangedRevision" \endcode
     Should we configure subversion's automatic property setting to set svn:keywords for
     these files? </li>
     <li> What about Git/Bazaar? </li>
    </ul>
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
  </ul>

*/

