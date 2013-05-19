// Oliver Kullmann, 30.7.2008 (Swansea)
/* Copyright 2008, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/SourceControl/plans/Github.hpp
  \brief Plans and todos for accessing and maintaining the OKlibrary via "Github"


  \todo General usage strategy : DONE (we only use Github, no own server)
  <ul>
   <li> Created the central OKlibrary repository at
   \verbatim
git://github.com/OKullmann/oklibrary.git
   \endverbatim
   (this is the "public" clone/pull/fetch URL). </li>
   <li> My (OK's) direct connection (where I can also push) is via
   \verbatim
url = git@github.com:OKullmann/oklibrary.git
   \endverbatim
   (copied from .git/config). </li>
   <li> DONE
   Since everybody can pull from the *public github repository* (to
   introduce a notion here), it seems that the packages should be created
   such that the "pull" automatically pulls from this repository. </li>
   <li> DONE
   So likely the current http-accessible repository (on cs-svr1) should
   be abandoned. </li>
   <li> However, for the core developer group, pushs and pulls should go to
   the local central server (cs-oksvr), and only in emergencies (like
   currently) does work happen via github. </li>
   <li> That is, normally only OK pushs to github, while everybody might
   pull from there. </li>
  </ul>


  \todo Github terminology and features
  <ul>
   <li> A given user (i.e. OKullmann) may "follow" another user (i.e. MLewsey),
   in which case, all activity (commits, new repositories, follows etc) of
   MLewsey is reported on OKullmann's "News Feed". </li>
   <li> A given user also may "watch" a repository. This is analogous to
   "following" a user, but now all activity (commits etc) of the
   *respository* is reported on the user's "News Feed". </li>
   <li> Github has a "group" on Google Groups. While this is not a pure mailing
   list, the same functionality is available, by joining the group and then 
   selecting to read the group by email. Emails can then be sent to the group 
   at "github@googlegroups.com", just like a mailing list, and any responses 
   are received by email. </li>     
  </ul>


  \todo Collaborative work on github
  <ul>
   <li> Basically, collaboration happens as follows:
    <ol>
     <li> In order to submit, the developer must have an account at
     http://github.com/. </li>
     <li> Similar to "OKullmann", this account name should be, %e.g.
     "MHenderson" or "MGwynne". </li>
     <li> Care should be taken that the e-mail address registered at githup
     is the same as the "official" OKlibrary e-mail address. </li>
     <li> For ssh-access, the developer registers his public ssh-address
     (for his computer(s)). </li>
     <li> Once registered, the developer creates on github a new repository
     by "forking" the OKlibrary-repository. </li>
     <li> Using (taking ML as an example)
     \verbatim
git remote add devgithub git@github.com:MLewsey/oklibrary.git
     \endverbatim
     in the *local* repository of the developer, after that the developer
     can communicate with *his* "fork" as usual by
     \verbatim
git push devgithub
git pull devgithub master
     \endverbatim
     etc. </li>
     <li> Using
     \verbatim
git remote add github git://github.com/OKullmann/oklibrary.git
     \endverbatim
     again at the local repository of the developer, he can also use
     \verbatim
git pull github master
     \endverbatim
     to pull to his local repository from the central github repository
     (however, no push). </li>
     <li> In order to "push" in this way, the developer has to make at
     his github-account a "pull-request" to the central repository. This
     appears to be only for information, but provides additional information
     (for example the SHA hash-value). </li>
     <li> Then the central maintainer (OK) apparently has to pull this change
     to his local repository (apparently just pulling from the public repository
     of the developer would work, but the pull-information provided especially
     by github only pulls the changes mentioned in the pull-request, by using a
     SHA hash value). </li>
     <li> Setting up another abbreviation (like "github" or "devgithub" above)
     in this case COULD use (taking for example ML's github repo)
     \verbatim
git remote add -f MLgithub git://github.com/MLewsey/oklibrary.git
     \endverbatim
     (on the central maintainers (OK's) machine). Then via
     \verbatim
git checkout -b MLgithub/master
git pull MLgithub master:4f0ea0c
     \endverbatim
     the pull (by OK) is performed, using the SHA-value as provided by the
     pull-request. Unclear what the "-f" and the "checkout" is good for. </li>
     <li> HOWEVER, this creates a branch, and we
     normally don't do that. So just
     \verbatim
git remote add MLgithub git://github.com/MLewsey/oklibrary.git
git pull MLgithub master:4f0ea0c
     \endverbatim
     is to be used. </li>
     <li> To update the central github repository, the central maintainer then
     has to push the changes. </li>
     <li> So pushing is only possible to own repositories, while pulling is
     always possible. </li>
    </ol>
   </li>
   <li> DONE (no own server anymore)
   This setup is reasonable for allowing anybody to contribute, but for
   the normal work, as stated above, that's a bit too much work, and we just
   push (if available) to the central local (bare) repository (as on cs-oksvr,
   if running). </li>
  </ul>


  \todo SourceForge
  <ul>
   <li> The public (read-only) git-address at SourceForge is
   \verbatim
git://oklibrary.git.sourceforge.net/gitroot/oklibrary/oklibrary
   \endverbatim
   When cloning this repository, one must rename it to "oklib" via
   \verbatim
git clone git://oklibrary.git.sourceforge.net/gitroot/oklibrary/oklibrary oklib
   \endverbatim
   </li>
   <li> A remote-alias for this can be created by
   \verbatim
git remote add sourceforge git://oklibrary.git.sourceforge.net/gitroot/oklibrary/oklibrary
   \endverbatim
   </li>
   <li> The private (read/write) git-address is
   \verbatim
ssh://xgateruq9@oklibrary.git.sourceforge.net/gitroot/oklibrary/oklibrary
   \endverbatim
   (this needs the password for the SourceForge account). </li>
  </ul>

*/
