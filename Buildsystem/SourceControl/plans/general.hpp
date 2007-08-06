// Oliver Kullmann, 10.4.2007 (Swansea)

/*!
  \file Buildsystem/SourceControl/plans/general.hpp
  \brief Plans and todos for the versioning control system


  \bug MG submitted to the shared repository, but no notification e-mail was created?
  <ul>
   <li>One has to check whether the post-receive script is being executed.<li>
   <li>Most obvious guess is that something's wrong with the identity of MG on cs-oksvr
   (perhaps a missing group-membership).</li>
   <li>MG - Point to note, if I use sendmail from the command line without the envelope sender
   (-f option), the mail doesn't arrive, but if I specify it, the mail arrives fine. From
   what I can see, the envelope sender is only set in the post-receive script if 
   hooks.envelope_sender is set. Ie - 
   \verbatim
csmatthewg@cs-oksvr:~> /usr/sbin/sendmail -t -f 360678@swan.ac.uk
To: 360678@swan.ac.uk
From: 360678@swan.ac.uk
Subject: Testing sendmail for git

Testing
. 
   \endverbatim
   Works but the following doesn't 
   \verbatim
csmatthewg@cs-oksvr:~> /usr/sbin/sendmail -t
To: 360678@swan.ac.uk
From: 360678@swan.ac.uk
Subject: Testing sendmail for git

Testing
. 
   \endverbatim
   Perhaps the mail is being sent by the hook script but isn't being routed
   by the university mail servers due to the envelope sender being set to 
   csmatthewg or something similar which it doesn't recognise as a valid
   address/user to route for? Perhaps someone with access to the mail logs
   on ok-svr could take a look to see if there is anything to suggest such 
   a problem?
   </li>
  </ul>


  \todo Notification-e-mails
  <ul>
   <li> Improvements of the automatic e-mail:
     <ol>
      <li> The subject header "Transitional branch, master, updated." is
      misleading: There is no branch "Transitional". </li>
      <li> Further in the subject header:
      "Testsystem_to_TestSystem_rename_tag-1504-geea1666" --- why this old
      tag? </li>
      <li> Additionally, for each log-message we need the summary of
      changes. </li>
      <li> Replace "[SCM]" by "[OKlibrary::Transitional]". DONE </li>
     </ol>
   </li>
   <li> DONE (to be put to the documentation)
   To configure the script to send emails to OK,MH and ML we tell Git via
   \verbatim
git config hooks.mailinglist "Oliver Kullmann <O.Kullmann@swansea.ac.uk>, Matthew Henderson <M.J.Henderson@swansea.ac.uk>, Matthew Lewsey <csmatthewl@Swansea.ac.uk>, Ulrich Berger <U.Berger@swansea.ac.uk>, Matthew Gwynne <M.S.GWYNNE.360678@swansea.ac.uk>"
   \endverbatim
   </li>
   <li> DONE (to be put to the documentation)
   To have the name Transitional appear in the email subject it is necessary to modify the
   file /work/Repositories/Git/bare/Transitional/description so that it contains the single line 
   \verbatim
OKlib::Transitional  
   \endverbatim
   </li>
   <li> DONE (put under version control in Configuration/SourceControl; after an update it has to
      be copied to .git/hooks of the central shared repositories)
   Should we use a link instead of replacing the original post-receive script? (OK: a link to what?)
   (MH: My intention was that the script would also be under version control. For example, it could be
   in Transitional/Buildsystem/post-receive-email and then the Transitional bare repository on cs-oksvr 
   has a link 
   \verbatim
/work/Repositories/Git/bare/Transitional/hooks/post-receive-email --> /work/Repositories/Git/Transitional/Buildsystem/post-receive-email
   \endverbatim
   But this isn't possible because then we always have to update the clone, or? OK: It seems saver to me to have a copy in the shared repositories, but
   the script should be under version control (since we have to tweak it;
   if it has already been changed, then we need first the original version
   into version control).
   </li>
   <li> DONE
   As an intermediate solution we copy a modified version of the script "contrib/hooks/post-receive-email" to
   both /work/Repositories/Git/bare/Annotations/hooks/post-receive-email and/work/Repositories/Git/bare/Transitional/hooks/post-receive-email.
   </li>
   <li> DONE (replaced by the solution via the (much) more advanced Git-script)
   Simple solution: In the shared repository the file
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
  </ul>


  \todo Remote access
  <ul>
   <li> How does remote access work:
    <ol>
     <li> A clone stores the url of the source in the .git/config of the clone and this can be
     accessed either directly via the config file or via
     \verbatim
git config remote.origin.url
     \endverbatim</li>
     <li> Remote repositories can be handled via git-remote (see man page - MG) </li>
     <li> When pushing to or pulling from a remote repository, how does git know how to communicate?
     It seems there are two options:
      <ul>
       <li> ssh is used (either an automatic channel is set up, or the password is asked for; at
       the "plumbing"-level the commands "git-ssh-fetch" and "git-ssh-upload" are responsible for
       this). For "untrusted users", on the server-side the special git-shell
       should be used, which needs to be set up as the login-shell for that user (apparently
       ssh has no control over the login-shell, but it's up to the login-shell on the server-side). </li>
       <li> No ssh is used, but on the remote repository git-daemon is running (this apparently does not
       require anything on the pushing/pulling side?). </li>
      </ul>
      ssh is the default protocol, you can explicitly specify which you wish to use by adding the protocol
      specifier to the url like so - ssh://username@host:/path/to/repository - 
      (see http://www.kernel.org/pub/software/scm/git/docs/git-push.html#URLS or man git-push)
     </li>
     <li> Copied clones which know how to connect:
     How to create a clone, which can be copied (as a directory),
     and wherever this clone is used, by "git push" and "git pull" it connects by one of the three
     above methods to the source, given that the service is activated? In this way we can make
     the clone downloadable from the Internet, anybody can start developing locally, and they can
     connect to the source-clone if they have the permissions. </li>
     </li>
     <li> It seems that shared repositories (that's what we are interested in) behave as follows:
      <ol>
       <li> When created, the default-group of the user is considered, and every user belonging
       to this group can push (i.e., write) to this repository (while all can pull, i.e., read). </li>
       <li> To give an external developer access, one has to create an account, where the developer
       just belongs to the group of the repository and to nothing else (also no home directory). </li>
       <li> The login-shell of this user must be "/usr/local/bin/git-shell", which should ensure that
       from the outside only this very restricted git-shell is used. </li>
       <li> Such a system-user should not be able to do any harm other than pushing (and pulling) from
       the repository. </li>
       <li> For this to work, for every (external) developer-group a (Linux) user-group on cs-oksvr
       has to be created, with a representative user in it (who has no other allowances). </li>
       <li> Core developers would have to be members of all such groups. </li>
       <li> Access would be via ssh. </li>
      </ol>
     </li>
     <li> For "active users" (who pull from the user-clone) anonymous pull is needed, and thus
     the git-daemon seems to be needed. </li>
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
   A similar thing happened on cs-oksvr when trying to clone there a repository from cs-wsok.
   </li>
  </ul>


  \todo Tagging
  <ul>
   <li> When advancing the version of Transitional, we should create a tag:
    <ol>
     <li> "git tag": A tag is just a mark for a commit;
     so when committing the central milestones-file with the new version number,
     thereafter a tag "Transitional_?.?.?" should be created. </li>
     <li> This tag is only a leight-weight tag: For releases a "full tag" with
     PGP-signature should be used. </li>
     <li> Or perhaps we should use such a "full tag" for every new version
     of Transitional --- in this way we also make sure that the tag always
     gets transmitted (via push or pull)! </li>
    </ol>
   </li>
   <li> Compare with "Special tag" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
  </ul>


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
   <li> Cloning:
    <ol>
     <li> How can we clone also the ignore-patterns? </li>
     <li> What does actually belong to a repository? Is there a
     "full cloning" ? </li>
    </ol>
   </li>
   <li> Secure pushs to the central repository on cs-oksvr:
    <ol>
     <li> Is it possible to only allows pushs to a repository if the pushing
     repository is identical in content to the receiving repository? </li>
     <li> By default only fast-forward pushs are possible, that is, the pushing repository
     is a "subset" (or "predecessor") of the receiving repository --- however with
     <code>git push --force</code> this check could be overwritten? </li>
     <li> So the problem is whether the application of option "--force" to push
     can be disabled. </li>
     <li> In Git/Documentation/cvs-migration.html under "Advanced Shared Repository Management"
     there is a script (using an update hook) explained which allows differentiation between users who can use
     --force and those who can't (and further measures). </li>
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
   <li> The configuration data about developers, library names etc. must go
   to Configuration (see Buildsystem/Configuration/plans/Persons.hpp). </li>
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
     the files. See "ExternalSources repository" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
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
   <li> Branching: DONE (for some developers, working on special areas and feeling unsage,
   creating a branch might be quite reasonable, but for others who like OK work on many
   parts of the library, follow the credo of permanent improvement, it would be a waste of
   time)
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
  </ul>


  \todo Space usage:
  <ul>
   <li> It seems that the .git directories grow rather fast? For csltok
   (data always after "git gc --prune"):
    <ol>
     <li> 9.6.2007: 4.1 MB </li>
     <li> 14.6.2007: 4.2 MB </li>
     <li> 17.6.2007: 4.2 MB </li>
     <li> 21.6.2007: 4.2 MB </li>
     <li> 23.6.2006; 4.3 MB; Transitional total: 15.3 MB </li>
     <li> 24.6.2007; 4.3 MB; Transitional total: 15.4 MB </li>
     <li> 26.6.2007: 4.3 MB; Transitional total: 15.4 MB </li>
     <li> 27.6.2007: 4.4 MB; Transitional total: 15.4 MB </li>
     <li> 29.6.2007: 4.4 MB; Transitional total: 15.6 MB </li>
     <li> 1.7.2007: 4.5 MB; Transitional total: 15.7 MB </li>
     <li> 3.7.2006: 4.6 MB; Transitional total: 15.9 MB </li>
     <li> 8.7.2006: 4.6 MB; Transitional total: 15.9 MB </li>
     <li> 11.7.2006: 4.7 MB; Transitional total: 16.1 MB </li>
     <li> 14.7.2006: 4.8 MB; Transitional total: 16.2 MB </li>
     <li> 19.7.2006: 4.9 MB; Transitional total: 16.3 MB </li>
     <li> 26.7.2006: 5.0 MB; Transitional total: 16.5 MB </li>
     <li> 29.7.2006: 5.0 MB; Transitional total: 16.5 MB </li>
     <li> 1.8.2006: 5.0 MB; Transitional total: 16.6 MB </li>
    </ol>
   </li>
  </ul>


  \todo Tutorial on branching : DONE (basic principle are understood; needs to be
  transferred to the documentation-document)
  <ul>
   <li> Creating a (completely new) branch is a (purely) local matter; the name is
   better unique (different form files or other branches). Let's call it "br"
   for demonstration here, then the basic command is
   \verbatim
git branch br
   \endverbatim
   which can be checked by <code>git branch</code> (whether the branch has been created),
   while switching to the branch happens via
   \verbatim
git checkout br
   \endverbatim
   This can be combined into one step via
   \verbatim
git checkout -b br
   \endverbatim
   The new branch by default will contain the full history of the branch on which the
   branch-creation command was issued (one can go back in history by adding a start-point
   like a sha1-hash-value after the name of the new branch).
   </li>
   <li> A cloned repository automatically "tracks" branches from the master clone,
   that is, when issuing "git pull" then the changes to the remotely tracked branches
   are stored (but don't result in branches; one can see them via <code>gitk --all</code>,
   but apparently one cannot do anything else with this stored information?). </li>
   <li> When setting up the new branch (see above), then one can use instead
   \verbatim
git branch --track br 
   \endverbatim
   which has the only meaning that when "git pull" is issued when on branch br,
   then the changes of the branch m on which the branch-creating-command was issued (the
   tracked branch) are merged into br. So it appears to be just a sort of shortcut to issuing
   \verbatim
git merge m
   \endverbatim
   when on branch br.
   </li>
   <li> With the new local branch br (yet nobody else knows about it) one can work, do all sorts
   of things, and its typical destination is to be merged into another branch m via
   \verbatim
git merge br
   \endverbatim
   when on branch m, and then possibly deleting the branch via <code>git branch -d br</code>
   when on another branch. (It seems that somewhat strange things can happen when working
   further with br (for example files created on br suddenly appear on m as untracked files)?
   But it should be normal to do so?) </li>
   <li> Now pushing a new branch to the shared central repository doesn't just happen via
   "git push" now, since this pushes all "references" which exist locally *and* on the
   remote repository, and yet there is no reference on the master clone (the repository
   to which "git push" is adressed when not further specified, which we assume is normally
   the shared central repository). To make the branch available use
   \verbatim
git push origin br:br
   \endverbatim
   where "origin" is the git-abbreviation of the url of the master clone (instead of "origin"
   any other url of any other git-repository can be used), and "br:br" is a short version
   of the full "refspec", the specification of the reference at the source (on the left-hand side;
   this is the current repository) and at the destination (on the right-hand side; note
   that one can push to any other branch, either existing or new (then created), but then
   there would be no automatic link between the histories of the local branch br and the remote
   branch br_remote). Alternatively one can use
   \verbatim
git push --all
   \endverbatim
   pushing all existing references.
   </li>
   <li> Now when on the central shared repository the new branch has been created, then, via the
   automatic tracking, a "git pull" on another clone (of the central shared repository) will
   pull it in, but not creating a new branch, which needs a special checkout-command; the whole
   sequence on the clone then is
   \verbatim
git pull
git checkout --track -b br origin/br
   \endverbatim
   where, as discussed above, the "--track" option just has the result, then when issuing
   "git pull" (no further arguments) on branch br, then a merge from the central repository
   of branch br is performed).
   </li>
   <li> It appears, that actually git doesn't know about the assumed relation between for example
   branch br on the shared central repository and on the clone: Every branch is always local,
   and that they have the same name as some branch somewhere else is considered to be purely
   coincidental --- only the tracked references make some branches automatically merge into
   other branches (namely "git pull" on branch m will fetch and merge the referenced branch
   into m). </li>
   <li> In the above example, the creator of branch br has a different relation to it then
   the other which just copies it:
    <ol>
     <li> The creator tracks branch m (possibly the master-branch) and thus is responsible
     for keeping br up-to-date with m (without the "--track"-option there would be no
     default relation to any other branch). </li>
     <li> The other (the copier) only tracks the changes coming from the central repository. </li>
    </ol>
    Perhaps this asymmetry is not appropriate (the creator was just the first one, like the first
    one creating a file, but then other join in), and then in file .git/config more complicated
    relations have to be established. For example, on the copying clone the entry for tracking
    branch br from the central repository is
    \verbatim
[branch "br"]
        remote = origin
        merge = refs/heads/br
    \endverbatim
    which sets variable branch.br.merge to "refs/heads/br", the default for "git pull" (without
    arguments). Now this variable can have a multiple values (realised by
    several assignments(!)), and so in this case several
    remote branches can be pulled at once. The above sets also variable branch.br.remote
    to "origin", where here apparently only one value (not a list) is allowed, namely
    the repository from which "git pull" will pull by default; for tracking something from
    the local repository the special value "." for remote is used, so for example for
    the creator of branch br:
    \verbatim
[branch "br"]
        remote = .
        merge = refs/heads/master
    \endverbatim
    It appears not to be possible to combine local and remote merges.
   </li>
   <li> As an aside, such variable settings can either be handling by editing file .git/config,
   or by using for example
   \verbatim
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/m"
   \endverbatim
   which would for the copying clone result in not just following the central version of br,
   but also the "master"-branch m. For the creator of branch m, who currently has to use
   \verbatim
git pull origin br
   \endverbatim
   to get the changes from the repository (recall that he tracks the local branch m), it would make
   sense to use
   \verbatim
git config branch.br.remote "origin"
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/m"
   \endverbatim
   to get the same settings.
   </li>
   <li> We remark the asymmetry between pull and push: With "git pull" one only gets the
   specific branches which are referenced by the current branch (except of that somewhat
   strange situation, where a new branch appears on the master clone), while via
   "git push" one pushes all currently referenced connections (not just on the current branch).
   </li>
   <li> Finally, merging br with its "master" m (abandoning br) happens via issuing by someone
   on the (respective) master-branch m
   \verbatim
git merge br
   \endverbatim
   followed by
   \verbatim
git branch -d br
   \endverbatim
   on the central repository(!), followed by
   \verbatim
git branch -d br
   \endverbatim
    on all local copies.
   </li>
  </ul>


  \todo On branching (in our situation) DONE (fixed a reasonable procedure)
  <ul>
   <li> The appropriate routine for creating a new branch br by developer D and making it
   available on the shared repository seems to be as follows:
    <ol>
     <li> D in the master branch issues
     \verbatim
git branch br
     \endverbatim
     followed by making it known
     \verbatim
git push origin br:br
     \endverbatim
     (or, easier, by <code>git push --all</code>), followed by setting tracking behaviour 
     (tracking master and br remotely)
     \verbatim
git config branch.br.remote "origin"
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/master"
     \endverbatim
     </li>
     <li> Anybody else picks it up by
     \verbatim
git pull
git checkout -b br origin/br
git config branch.br.remote "origin"
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/master"
     \endverbatim
     </li>
     <li> Pushing now via <code>git push</code> means pushing all (committed)
     changes to the central repository (all branches), while <code>git pull</code> on 
     some branch will pull in all changes related to the current branch only. </li>
     <li> In order to have pulls which combine different pull-actions,
     in .git/remotes for example the file "Laptop" has been created with
     the content
     \verbatim
URL: ~LaptopArchiv/OKsystem/Transitional/
Push: master:master
Push: rijndael:rijndael
Pull: master:master
Pull: rijndael:rijndael
     \endverbatim
     which means, that by using <code>git pull Laptop</code> both branches 
     are updated from the Laptop-archive (independently), and that by 
     "git push" one could push both branches (as said elsewhere, "pull" seems 
     to be better than "push" in case of non-bare repositories) --- this works
     independently of the branch from where the pull/push is issued. </li>
     <li> Since br is assumed to be an extension of m, normal works happens
     in branch br, but when committing files, one has to take care only to 
     commit the special files, while for the other files one changes to branch
     m (via <code>git checkout m</code>; after all committs in branch br have
     been handled --- otherwise git won't switch). </li>
     <li> Finally abandoning the branch, re-merging it into master via
     \verbatim
git merge br
     \endverbatim
     while on branch master, followed by
     \verbatim
git branch -d br
     \endverbatim
     on the central repository and each local clone. </li>
    </ol>
   </li>
   <li> It appears that "secret" development cannot be achieved via branching,
   but a new dedicated repository has to be created. </li>
  </ul>


  \todo Git on the cs-oksvr : DONE
  <ul>
   <li> The notification e-mails need to be re-installed (perhaps
   this time with some improvements). DONE </li>
   <li> Git needs to be updated (and this also regularly) : DONE (put the
   OKlibrary into csoliver's account) </li>
  </ul>


  \todo Moving files: DONE
  <ol>
   <li> Use "git mv source destination" to move single files as well as whole directories. </li>
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
    History-surgery is only an option as long as there are no public clones out. But actually
    we should not be hindered to move files and directories around as needed, and thus we
    should not worry at all about certain problems regarding the history!
   </li>
  </ol>

*/

