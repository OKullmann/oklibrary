// Oliver Kullmann, 6.6.2008 (Swansea)
/* Copyright 2008, 2009, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/SourceControl/plans/Branching.hpp
  \brief Todos and plans regarding branching


  \todo Purely local branches
  <ul>
   <li> Branches allow changes and commits for different new features
   to be separated into conceptually different copies of the repository,
   each with their own branch name. </li>
   <li> Each branch can then be pushed and pulled separately, and
   pull requests can be made to the core developers for individual
   branches when individual features are finished. </li>
   <li> In this way, work is modularised and can be accepted and
   rejected in a modular fashion. This speeds up the development
   process and helps to prevent "code rot". </li>
   <li> The OKlibrary is now available on GitHub and so the standard
   development model is to:
    <ol>
     <li> Fork OK's OKlibrary github repository and clone
     this to a local repository (see
     "Collaborative work on github" in SourceControl/plans/Github.hpp)
     </li>
     <li> When a new feature is being developed, create a branch
     "feature_name" and begin working on this feature in branch
     "feature_name":
     \verbatim
OKlib> git checkout -b feature_name
OKlib> git branch
* feature_name
  master
# Do work and edit files as normal
     \endverbatim
     </li>
     <li> Once work is finished, one uses the following procedure
     to submit the changes to OK:
      <ol>
       <li> Checkout branch master. </li>
       <li> Update branch master. </li>
       <li> Checkout branch "feature_name". </li>
       <li> Rebase branch master into branch "feature_name". </li>
       <li> Check through history to ensure recent changes don't affect
       your commits and rerun tests. </li>
       <li> Push changes to own fork of OKlibrary on github. </li>
       <li> Make a pull request on Github to the central OKlibrary repository
       (see "Collaborative work on github" in SourceControl/plans/Github.hpp)
       </li>
      </ol>
      That is:
      \verbatim
OKlib> git checkout master
OKlib> git pull
OKlib> git checkout feature_name
OKlib> git rebase master
# Check history and rerun tests
OKlib> git push devgithub feature_name:feature_name
# On the github website, make a pull request from branch "feature_name".
# See https://help.github.com/articles/using-pull-requests
      \endverbatim
      Rebasing in this way helps to create a much cleaner history
      in the central OKlibrary repository and makes the merge
      procedure conceptually simpler for the core developers.
     </li>
     <li> If branch "feature_name" is ready for inclusion in the
     central OKlibrary repository, then there is nothing more to do and
     OK will merge the changes. </li>
     <li> If the pull request is rejected and changes are requested then
     the user should checkout branch "feature_name", do additional work,
     push again, and make another pull request:
     \verbatim
OKlib> git checkout feature_name
# Do more work
OKlib> git push devgithub feature_name:feature_name
# On the github website, make a pull request from branch "feature_name".
# The original pull request may need to be closed first.
# See https://help.github.com/articles/using-pull-requests
     \endverbatim
     However(!), after the *initial* pull request, DO NOT rebase branch
     master into branch "feature_name" a second time. After the initial
     pull request, the history of branch "feature_name" has meaning
     to the core developers and might be referred to, and so should
     (in general) not be tampered with. </li>
    </ol>
   </li>
   <li> DONE (merging is no longer the standard model)
   For a local branch (not to be exported) the standard model is as
   follows:
    <ol>
     <li> Create branch B by "git branch B" (while current branch being
     master). </li>
     <li> Once master has been updated, update B by "git merge master" (being
     on branch B). </li>
     <li> Finally, for abandoning B, use "git merge B" (being on branch
     master). </li>
     <li> Removal of B then by "git branch -d B" (deleting B). </li>
    </ol>
   </li>
   <li> DONE (subsumed by standard model using rebase)
   If the branch really is never exported, then the following
   model provides an alternative, where the history of the branch is
   linearised:
    <ol>
     <li> Everything as above. </li>
     <li> Except that when master has been updated, B is updated now by
     "git rebase master" (being on branch B). </li>
     <li> In this way the history of B is rewritten, but this might not
     be relevant. </li>
     <li> When finally abonding B, it must still be merged as above. </li>
    </ol>
   </li>
   <li> DONE (standard model using rebase is now given)
   However, outside of this situation I don't see a use of "rebase":
    <ol>
     <li> The change of history doesn't seem to be a fundamental problem, since
     in case of conflict with some external repository, git automatically
     creates an appropriate merge. </li>
     <li> But it seems to me that then the standard merge is to be preferred.
     </li>
    </ol>
   </li>
  </ul>


  \todo Problems with branch rijndael:
  <ul>
   <li> We don't have branch rijndael anymore, but the problems are
   symptomatic. </li>
   <li> Git-gui used to allow to switch branches, but then it stopped doing
   so?? </li>
   <li> Merge cascades still occur --- the key is to understand the
   merge-model of git, and its implications on how to reach a fixed point.
   </li>
   <li> Likely we have to ask these questions on the git-mailing-list. </li>
   <li> Why are there these merge-cascades?  Apparently on my laptop I merge
   master into rijndael, then on the workstation again, and then again on the
   server (without wanting to do so)??? It seems that setting up the pull's so
   that they automatically merge branches master and rijndael doesn't work
   nicely: Every different repository performs the merge again, and does not
   recognise that this has already happened. (Why is this the case? A bug?)
   So the cure is that every branch just pulls "itself", and no automatic
   merges (different from what is proposed below). It is then the
   responsibility of the submitter to make sure that branch
   rijndael is always a superset of of branch master. DONE (in this way it
   works reasonably) </li>
  </ul>


  \todo Tutorial on branching : DONE (basic principle are understood; needs to
  be transferred to the documentation-document)
  <ul>
   <li> Creating a (completely new) branch is a (purely) local matter; the
   name is better unique (different form files or other branches). Let's call
   it "br" for demonstration here, then the basic command is
   \verbatim
git branch br
   \endverbatim
   which can be checked by <code>git branch</code> (whether the branch has
   been created), while switching to the branch happens via
   \verbatim
git checkout br
   \endverbatim
   This can be combined into one step via
   \verbatim
git checkout -b br
   \endverbatim
   The new branch by default will contain the full history of the branch on
   which the branch-creation command was issued (one can go back in history by
   adding a start-point like a sha1-hash-value after the name of the new
   branch). </li>
   <li> A cloned repository automatically "tracks" branches from the master
   clone, that is, when issuing "git pull" then the changes to the remotely
   tracked branches are stored (but don't result in branches; one can see them
   via <code>gitk --all</code>, but apparently one cannot do anything else
   with this stored information?). </li>
   <li> When setting up the new branch (see above), then one can use instead
   \verbatim
git branch --track br 
   \endverbatim
   which has the only meaning that when "git pull" is issued when on branch br,
   then the changes of the branch m on which the branch-creating-command was
   issued (the tracked branch) are merged into br. So it appears to be just a
   sort of shortcut to issuing
   \verbatim
git merge m
   \endverbatim
   when on branch br.
   </li>
   <li> With the new local branch br (yet nobody else knows about it) one can
   work, do all sorts of things, and its typical destination is to be merged
   into another branch m via
   \verbatim
git merge br
   \endverbatim
   when on branch m, and then possibly deleting the branch via
   <code>git branch -d br</code> when on another branch. (It seems that
   somewhat strange things can happen when working further with br (for
   example files created on br suddenly appear on m as untracked files)?
   But it should be normal to do so?) </li>
   <li> Now pushing a new branch to the shared central repository doesn't
   just happen via "git push" now, since this pushes all "references" which
   exist locally *and* on the remote repository, and yet there is no reference
   on the master clone (the repository to which "git push" is adressed when
   not further specified, which we assume is normally the shared central
   repository). To make the branch available use
   \verbatim
git push origin br:br
   \endverbatim
   where "origin" is the git-abbreviation of the url of the master clone
   (instead of "origin" any other url of any other git-repository can be
   used), and "br:br" is a short version of the full "refspec", the
   specification of the reference at the source (on the left-hand side;
   this is the current repository) and at the destination (on the right-hand
   side; note that one can push to any other branch, either existing or new
   (then created), but then there would be no automatic link between the
   histories of the local branch br and the remote branch br_remote).
   Alternatively one can use
   \verbatim
git push --all
   \endverbatim
   pushing all existing references.
   </li>
   <li> Now when on the central shared repository the new branch has been
   created, then, via the automatic tracking, a "git pull" on another clone
   (of the central shared repository) will pull it in, but not creating a new
   branch, which needs a special checkout-command; the whole sequence on the
   clone then is
   \verbatim
git pull
git checkout --track -b br origin/br
   \endverbatim
   where, as discussed above, the "--track" option just has the result, then
   when issuing "git pull" (no further arguments) on branch br, then a merge
   from the central repository of branch br is performed).
   </li>
   <li> It appears, that actually git doesn't know about the assumed relation
   between for example branch br on the shared central repository and on the
   clone: Every branch is always local, and that they have the same name as
   some branch somewhere else is considered to be purely coincidental ---
   only the tracked references make some branches automatically merge into
   other branches (namely "git pull" on branch m will fetch and merge the
   referenced branch into m). </li>
   <li> In the above example, the creator of branch br has a different
   relation to it then the other which just copies it:
    <ol>
     <li> The creator tracks branch m (possibly the master-branch) and thus
     is responsible for keeping br up-to-date with m (without the
     "--track"-option there would be no default relation to any other branch).
     </li>
     <li> The other (the copier) only tracks the changes coming from the
     central repository. </li>
    </ol>
    Perhaps this asymmetry is not appropriate (the creator was just the
    first one, like the first one creating a file, but then other join in),
    and then in file .git/config more complicated relations have to be
    established. For example, on the copying clone the entry for tracking
    branch br from the central repository is
    \verbatim
[branch "br"]
        remote = origin
        merge = refs/heads/br
    \endverbatim
    which sets variable branch.br.merge to "refs/heads/br", the default for
    "git pull" (without arguments). Now this variable can have a multiple
    values (realised by several assignments(!)), and so in this case several
    remote branches can be pulled at once. The above sets also variable
    branch.br.remote to "origin", where here apparently only one value (not a
    list) is allowed, namely the repository from which "git pull" will pull
    by default; for tracking something from the local repository the special
    value "." for remote is used, so for example for the creator of branch br:
    \verbatim
[branch "br"]
        remote = .
        merge = refs/heads/master
    \endverbatim
    It appears not to be possible to combine local and remote merges.
   </li>
   <li> As an aside, such variable settings can either be handling by editing
   file .git/config, or by using for example
   \verbatim
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/m"
   \endverbatim
   which would for the copying clone result in not just following the central
   version of br, but also the "master"-branch m. For the creator of branch m,
   who currently has to use
   \verbatim
git pull origin br
   \endverbatim
   to get the changes from the repository (recall that he tracks the local
   branch m), it would make sense to use
   \verbatim
git config branch.br.remote "origin"
git config branch.br.merge "refs/heads/br"
git config --add branch.br.merge "refs/heads/m"
   \endverbatim
   to get the same settings.
   </li>
   <li> We remark the asymmetry between pull and push: With "git pull" one
   only gets the specific branches which are referenced by the current branch
   (except of that somewhat strange situation, where a new branch appears on
   the master clone), while via "git push" one pushes all currently referenced
   connections (not just on the current branch). </li>
   <li> Finally, merging br with its "master" m (abandoning br) happens via
   issuing by someone on the (respective) master-branch m
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


  \todo On branching (in our situation) DONE (fixed a reasonable procedure;
   needs to be transferred to the general documentation))
  <ul>
   <li> The appropriate routine for creating a new branch br by developer D
   and making it available on the shared repository seems to be as follows:
    <ol>
     <li> D in the master branch issues
     \verbatim
git branch br
     \endverbatim
     followed by making it known
     \verbatim
git push origin br:br
     \endverbatim
     (or, easier, by <code>git push --all</code>), followed by setting
     tracking behaviour (tracking master and br remotely)
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
     changes to the central repository (all branches), while
     <code>git pull</code> on some branch will pull in all changes related to
     the current branch only. </li>
     <li> In order to have pulls which combine different pull-actions,
     in .git/remotes for example the file "Laptop" has been created with
     the content
     \verbatim
URL: ~LaptopArchiv/OKsystem/OKlib/
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

*/
