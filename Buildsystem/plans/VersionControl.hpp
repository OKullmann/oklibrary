// Oliver Kullmann, 10.4.2007 (Swansea)

/*!
  \file Buildsystem/plans/VersionControl.hpp
  \brief Plans and todos for the versioning control system

  \todo More advanced usage:
  <ul>
   <li> Install qgit:
    <ol>
     <li> Perhaps it allows to follow renaming (like --follows and --parents)? </li>
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
     <li> 21.6.2007: 4.3 MB; after "git gc": 4.2 MB </li>
     <li> 23.6.2006; 4.3 MB; after "git gc": 4.3 MB;
          Transitional total: 15.3 MB </li>
     <li> 24.6.2007; 4.3 MB; Transitional total: 15.4 MB </li>
    </ol>
   </li>
   <li> Secure pushs to the central repository on cs-oksvr:
    <ol>
     <li> Is it possible to only allows pushs to a repository if the pushing
     repository is identical in content to the receiving repository? </li>
    </ol>
   </li>
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
     <li> The question seems now to be how to move also the history:
      <ol>
       <li> The new "git-filter-branch" (not in 1.5.2.2) could be the solution for filtering
       out certain files. Or one can build a completely new repository, with the appropriate
       commits not (re-)done. </li>
       <li> Still open the question of how to prepend the history of one file to the history
       of another file? Seems not possible (without recreating the whole repository)? </li>
       <li> A tool for rewriting the whole history (creating a new branch) is "cg-admin-rewritehis"
       (belonging to the "cogito"-tool): With this files can be filtered out, log-messages
       changed etc. The documentation of this command specifically contains an example of
       how to remove a file from history. </li>
       <li> So we should install the cogito-tool and experiment with it. </li>
       <li> Actually, apparently "git-filter-branch" is supposed to replace "cg-admin-rewritehis",
       so that we don't need cogito? </li>
       <li> The problem with the broken-history-chain (when renaming) appparently
       is solved with the new feature 
       \verbatim
git log -p --follow old_file
       \endverbatim
       which allows to show the whole history, following renamings. (What about gitk?) </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Combining different repositories:
   <ul>
    <li> Accidentally, from the Transitional-repository I pulled the Annotations-repository
    --- and it worked: It merged the complete history of the Annotations-files and -directories
    into the Transitional-directory. </li>
    <li> Problematic only that it moved everything to the top-level: How can we achieve that
    they all are moved to some sub-directory? The git-pull documentation seems not to say something
    here? </li>
    <li> The canonical thing to do seems to first create in the repository Annotations
    a directory "Annotations", move all files with
    \verbatim
git mv file1 file2 dir1 dir2 Annotations
    \endverbatim
    to this subdirectory (with a subsequent "git commit"), and then with pulling
    from this directory we get all files into Transitional (with new part "Annotations"). </li>
   </ul>
   </li>
  </ul>


  \todo Exploring usage patterns;
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
    <ol>
     <li> Symbolic links are stored exactly as given (in absolute or relative form). </li>
     <li> So by using relative links we could put the links into the repository. </li>
     <li> So it seems we should do that (since some directories have such a link, some
     not; and the relative position of the build directory is also known --- in case of
     a move something has to be done anyway). </li>
    </ol>
   </li>
   <li> Once we have ExternalSources under version control, we need the possibility to just have
   those binaries (the archives) in it, without any history, changes whatsoever (otherwise space
   would explode over time) --- how to achieve this?
    <ol>
     <li> Since every new version has a new name, it seems that we just need the possibility to remove
     the history of an item? </li>
     <li> And perhaps we can tell git in advance that the new entry is "don't care" ? </li>
     <li> Another possibility is that the external sources are not under version control,
     but we manage information like md5-checksums, and it's up to the user to download
     the files. </li>
     <li> For convenience we provide also an archive with all current external sources in it. </li>
     <li> And/or the clone to download can be populated with the current external sources. </li>
     <li> In any case, ExternalSources gets a sub-directory "sources". </li>
    </ol>
   </li>
   <li> What about version numbers in Git? What is the
   version-numbering-systems there, and what kind of statistics
   are supported? It seems the answer is simple -- nothing?! </li>
   <li> How to handle change dates and revision numbers in files with Git?
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
     (push and pull for such clones then only concern the embedded parts). </li>
     <li> It seems, that at least at a higher level Git currently does not
     offer something in this direction. Send an e-mail to the Git-list! </li>
     <li> Is "git-submodule" a solution? Unclear what it does?? And not available with
     1.5.2.1. </li>
     <li> And "repository surgery" is needed (like merging of repositories). DONE (see
     "Combining different repositories" in the above todo) </li>
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
   <li> The central repository:
   For a new version control system we have to find out how to establish the role of the repository at
   cs-oksvr as *central*, and how to manage access control (as fine-grained as possible; if possible not
   relying on ssh). </li>
  </ul>

*/

