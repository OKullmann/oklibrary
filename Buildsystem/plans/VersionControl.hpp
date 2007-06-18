// Oliver Kullmann, 10.4.2007 (Swansea)

/*!
  \file Buildsystem/plans/VersionControl.hpp
  \brief Plans and todos for the versioning control system

  \todo Building and using Git
  <ul>
   <li> Moving:
   How to move Learning/plans/Learning.hpp to Learning/plans/research/Learning.hpp, such that
   also the complete history of Learning/plans/Learning.hpp is moved (nothing remains)?
   Usage of "git mv" and subsequent commits is not clear:
    <ol>
     <li> The commit is performed by "git commit" (so except of the move nothing else should be
     staged (since the commit message concerns all what is staged)). </li>
     <li> Also with "git-gui" the commit will automatically work. </li>
     <li> However, all what is done is that the old file is no longer in the repository,
     while the new file is in the repository, with empty history except of the mv-information ---
     the old file is still in the history, while the new file has no history! </li>
     <li> "git mv file new_file" is equivalent to
     \verbatim
mv file new_file
git rm file
git add new_file
     \endverbatim
     Now for the commit the removal and the addition are staged, which git automatically combines
     into a renaming. </li>
     <li> The question seems now to be how to move also the history. </li>
    </ol>
   </li>
   <li> Searching:
   How do we search for files with a given content, like searching
   for files in the history which contained "Sam Buss"?
    <ol>
     <li> The git manual says:
     \verbatim
Somebody hands you a copy of a file, and asks which commits modified a file such that it contained the given content either before or after the commit. 
You can find out with this:
$  git log --raw -r --abbrev=40 --pretty=oneline -- filename |
         grep -B 1 `git hash-object filename`
     \endverbatim </li>
     <li> However this doesn't allow specification of the *content* (??),
     and what if we want to search in all files? </li>
     <li> Ask the git mailing list. </li>
    </ol>
   </li>
   <li> How does remote access work:
    <ol>
     <li> A clone stores the url of the source (supposedly): Where? Can one see this? </li>
     <li> When pushing to or pulling from a remote repository, how does git know how to communicate?
     It seems there are three options:
      <ul>
       <li> ssh is used (either an automatic channel is set up, or the password is asked for; is
       this always established by git-shell as in the next method?) </li>
       <li> ssh is used and a special git-ssh-connection is established (git-shell); is a password
       needed here? (or is this just the same as the "general ssh access"?!)  </li>
       <li> no ssh is used, but on the remote repository git-daemon is running (this apparently does not
       require anything on the pushing/pulling side?). </li>
      </ul>
      Does git automatically choose? Do we have a choice??
     </li>
     <li> Copied clones which know how to connect:
     How to create a clone, which can be copied (as a directory),
     and wherever this clone is used, by "git push" and "git pull" it connects by one of the three
     above methods to the source, given that the service is activated? In this way we can make
     the clone downloadable from the Internet, anybody can start developing locally, and they can
     connect to the source-clone if they have the permissions. </li>
     </li>
    </ol>
   </li>
   <li> Why does the following not work: On csltok I have a copy of a clone of a repository on cs-wsok;
   now when trying to push to it remotely, the following happens:
   \verbatim
> git push csoliver@cs-wsok:LaptopArchiv/OKsystem/Transitional
Password:
bash: git-receive-pack: command not found
fatal: The remote end hung up unexpectedly
error: failed to push to 'csoliver@cs-wsok:LaptopArchiv/OKsystem/Transitional'
   \endverbatim
   What's wrong here?? The command is there:
   \verbatim
> which git-receive-pack
/usr/local/bin/git-receive-pack
   \endverbatim
   </li>
   <li> Cloning:
    <ol>
     <li> How can we clone also the ignore-patterns? </li>
     <li> What does actually belong to a repository? Is there a
     "full cloning" ? </li>
    </ol>
   </li>
   <li> It seems that the .git directories grow rather fast? For csltok:
    <ol>
     <li> 6.6.2007: 4.3 MB </li>
     <li> 7.6.2007: 4.4 MB </li>
     <li> 9.6.2007: 4.5 MB; after "git gc": 4.1 MB </li>
     <li> 10.6.2007: 4.3 MB </li>
     <li> 14.6.2007: 4.3 MB; after "git gc": 4.2 MB </li>
     <li> 17.6.2007: 4.3 MB; after "git gc": 4.2 MB </li>
    </ol>
   </li>
   <li> DONE Problems with the repository: On csltok I get
   \verbatim
kullmann@csltok:Transitional> git fsck --strict
dangling blob c921acdcf384f7c8b942d863002dbbaad9f73c43
dangling blob cfded5f225e72661cf535b3a5200f47fc1d1982b
   \verbatim
   What to do now? It seems that "git gc --prune" will clean up, and
   that there is no problem here. With "git show BLOB-SHA" one can
   see the contant of a dangling blob. </li>
   <li> Created now also for Transitional a shared Git-repository. DONE </li>
   <li> DONE In order that commits have the full name ("Oliver Kullmann" instead of "csoliver") and e-mail-address
   ("O.Kullmann@Swansea.ac.uk" instead of "csoliver@cs-wsok.swan.ac.uk"), easiest is to tell Git for all
   repositories of the user (that is, in  ~/.gitconfig instead of .git/config) about it via
   \verbatim
> git config --global user.name "Oliver Kullmann"
> git config --global user.email O.Kullmann@Swansea.ac.uk
   \endverbatim
   </li>
   <li> DONE The creation of the initial Annotations-respository was done via
   \verbatim
su csoksc
cd /work/Repositories/Git/Annotations
git cvsimport -d /work/Repositories/OKdevelopment/  -v -A /work/Repositories/Git/e-mail_addresses Annotations
   \endverbatim
   Then the shared repository has been set up via (as explained in "git for CVS users")
   \verbatim
su csoksc
cd /work/Repositories/Git/bare
mkdir Annotations
cd Annotations
git --bare init --shared
bit --bare fetch /work/Repositories/Git/Annotations master:master
   \endverbatim
   Now every user has to create his master clone himself, via for example
   \verbatim
csoliver@cs-wsok:~/LaptopArchiv/OKsystem> git clone cs-oksvr:/work/Repositories/Git/bare/Annotations
   \endverbatim
   that is, go to the directory where you want to place the master-close, and issue the git-clone-command.
   Updating the master-clone happens simply via "git pull", submitting changes via "git push".
   However, <strong>before submission</strong> everything in the master-clone needs to be completely up-to-date and tested!!
   </li>
   <li>  DONE Creating a local copy of the master-clone simply via
   \verbatim
git clone PATH-TO-EXISTING-CLONE
   \endverbatim
   which will create the clone in the current directory. Then the following actions are possible:
   <ol>
    <li> Updating the subclone via "git pull" in the sub-clone. </li>
    <li> Updating the masterclone either via "git pull PATH-TO-SUB-CLONE" in the masterclone,
    or via "git push" in the sub-clone, followed by "git checkout -f" in the master-clone. </li>
   </ol>
   </li>
   <li> Is there html-documentation? DONE (documentation-links seem to be fine (and all what is available)) </li>
   <li> DONE Problems with cvsimport:
   \verbatim
csoksc@cs-oksvr:/work/Repositories/Git/Annotations> git cvsimport -d /work/Repositories/OKdevelopment/ \
-C /work/Repositories/Git/Annotations -a -v -A ../e-mail_addresses Annotations
   \endverbatim
   resulted in
   \verbatim
Done.
DONE; creating master branch
D       200511_UniformHittingClauseSets.tex
D       200512_Alldifferent.tex
D       200512_OKlibrary.tex
D       200512_OKlibrary_Annotations
D       BuchOKsystems.tex
D       FullDocumentation.tex
D       History_OKsystem
D       Latex_bib/C++_Books.bib
D       Latex_bib/Constraints.bib
D       Latex_bib/EigArt.bib
D       Latex_bib/MiscSAT.bib
D       Latex_macros/Basis.tex
D       Latex_macros/Definitionen.tex
D       Latex_macros/FolienDefN.tex
D       Latex_macros/OKplatform_Texpackages.tex
D       Release-1_0
D       definitions.mak
   \endverbatim
   What's the point in deleting all the files you just wanted to import?? Okay,
   \verbatim
git checkout -f
   \endverbatim
   is needed after the import (the "D" just means the files are not there, i.e.,
   supposedly have been deleted).
   </li>
   <li> On csltok asciidoc is not available --- what about cs-wsok? DONE (supplied a build-makefile) </li>
   <li> Perhaps we should just use the default Git-installation into
   the users bin-directory (otherwise it seems hard to use because of
   the many different commands). It should also be possible to do the
   system-wide installion (in /usr/local). DONE (for now just the system-wide installation) </li>
  </ul>


  \todo New version control system
  <ul>
   <li> Likely when advancing the version of Transitional, we should tag all
   files:
    <ol>
     <li> How to do this? "git tag": Apparently a tag is just a mark for a commit;
     so when committing the central milestones-file with the new version number,
     thereafter a tag "Transitional ?.?.?" should be created. </li>
     <li> Are there several possibilities? Yes ... </li>
    </ol>
   </li>
   <li> How to send notification-e-mails with Git: In the shared repository the file
   "hooks/post-receive" has to be made executable, and then filled with action; temporary
   solution (for Annotations):
   \verbatim
mutt -s "OKlibrary::Annotations Git Push -- $USER" O.Kullmann@Swansea.ac.uk m.j.henderson@swansea.ac.uk csmatthewl@swan.ac.uk
   \endverbatim
   Apparently to the script "post-receive" per branch a line with reference-data is passed on stdin,
   while stdin is passed onto mutt which then sends these lines in the body. A more sophisticated
   solution is given in in the Git-repository under "contrib/hooks/post-receive-email" (see also
   the text in "hooks/post-receive"), which we should examine (it seems we should also set up
   an OKlibrary-e-mail-list ?!).
   </li>
   <li> Git can handle symbolic links, so all symbolic links should go
   into the respository? On the other hand, this seems to imply one universal
   convention like "/h/21/GemeinsameBasis", which perhaps one better should avoid?!?
   </li>
   <li> Once we have ExternalSources under version control, we need the possibility to just have
   those binaries (the archives) in it, without any history, changes whatsoever (otherwise space
   would explode over time) --- how to achieve this?
    <ol>
     <li> Since every new version has a new name, it seems that we just need the possibility to remove
     the history of an item? </li>
     <li> And perhaps we can tell git in advance that the new entry is "don't care" ? </li>
    </ol>
   </li>
   <li> What about version numbers in Git? What is the
   version-numbering-systems there, and what kind of statistics
   are supported? </li>
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
     local repository on cs-wsok to the central repository? Should be no problem. DONE (yes, that's possible)</li>
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
   <li> Unified repositories ("holistic" or "active" libraries)
    <ul>
     <li> Optimally, the OKlibrary-package is exactly a clone of the
     repository (and then one just needs to run "make" in the top-level
     directory). </li>
     <li> So "Release = clone", and the user has the same power as the
     developer --- the idea of an active library (users extend the library)! </li>
     <li> For this also ExternalSources needs to be in the version control. </li>
     <li> Optimally there is just one repository
     for the whole library (containing the current three repositories Annotations,
     OKlibrary, Transitional, and the new (sub-)repositories ExternalSources and Overview).
     Then we need the possibility to restrict access to parts of the repository (so
     that annotations and Transitional are not public). </li>
     <li> A problem is, that Annotations currently has public as well as
     non-public parts; likely this needs to be separated. </li>
     <li> A good solution would be, if "selective cloning" would be possible
     (push and pull for such clones then only concern the embedded parts).
     And "repository surgery" is needed (like merging of repositories). </li>
     <li> It seems, that at least at a higher level Git currently does not
     offer something in this direction. Send an e-mail to the Git-list! </li>
     <li> Is "git-submodule" a solution? Unclear what it does?? And not available with
     1.5.2.1. </li>
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
   <li> When a sub-module progresses to a higher version number:
   Tagging all files in the sub-module? DONE (Apparently
   it is not possible to tag files, but only commits, and then tags seem to be
   a global thing. So when advancing the version number of a module, we use the standardised
   log-message
   <center>
   *** New version number ?.?.? (module XXX) ***
   </center>
   or
   <center>
   *** Initial version number ?.?.? (module XXX) ***
   </center>
   and this must suffice.)
   </li>
   <li> Files to ignore can be entered to .git/info/exclude. DONE </li>
   </li>
   <li> We have a little problems with submissions to the repository, which often span
   many files, so the whole submission process takes a while, and it's not clear from
   outside when it's finished (and the library is again in a well-defined state). DONE (With Git first all submissions are done to a local clone, and only
   once this is all settled, the final push to central respository happens.)
   </li>
   <li> The main conceptual disadvantage (shared with CVS) of Subversion is that
    no local repositories are possible; alternatives: DONE (the three
    existing repositories have been copied over to Git).
    <ol>
     <li> Git (http://git.or.cz/) looks rather good --- one should try it out! </li>
     <li> BitKeeper (http://www.bitkeeper.com) seems to be only proprietary. Can't find anything special
     for it. DONE</li>
     <li> What about Arch (http://www.gnuarch.org/gnuarchwiki/)? Doesn't seem to have the strong
     development team as Git; and I can't see anything special about it. DONE</li>
     <li> Bazaar (http://bazaar-vcs.org) was recommended in [{CVU}, vo. 10, no. 2, page 34]. See whether
     meanwhile the have a comparison with Git! No, not yet --- my (OK) impression is that git is technically
     much stronger (while Bazaar aims at being "nice"). DONE </li>
     <li> svk (http://svk.elixus.org), apparently a further development of Subversion.
     Looks somewhat immature --- and aims at just improving svn in some parts. DONE </li>
    </ol>
   </li>
  </ul>

*/

