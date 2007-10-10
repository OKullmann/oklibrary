// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Mailman.hpp
  \brief Plans regarding installation of the Mailman package


  \todo Building Mailman on cs-wsok (for testing):
  <ul>
   <li> Building the main program </li>
   <li> DONE (Apache works, and so we use it)
   What about the web server: Is Apache the best choice for us?
   (Perhaps we need the web server anyway for the OKplatform-Internet
   page?) </li>
  </ul>

  \todo Building Mailman on freshly setup machine
  <ul>
   <li>
   \verbatim
ExternalSources/Installations/Postfix/postfix-2.4.5> cd ../../
ExternalSources/Installations> mkdir fetchmail
ExternalSources/Installations> cd fetchmail
ExternalSources/Installations/fetchmail> wget http://download.berlios.de/fetchmail/fetchmail-6.3.8.tar.bz2
ExternalSources/Installations/fetchmail> tar -jxvf fetchmail-6.3.8.tar.bz2
ExternalSources/Installations/fetchmail> cd fetchmail-6.3.8
ExternalSources/Installations/fetchmail/fetchmail-6.3.8> ./configure
ExternalSources/Installations/fetchmail/fetchmail-6.3.8> make
ExternalSources/Installations/fetchmail/fetchmail-6.3.8> sudo make install

ExternalSources/Installations/fetchmail/fetchmail-6.3.8> cd ../../
ExternalSources/Installations/> mkdir mailman
ExternalSources/Installations/> cd mailman
ExternalSources/Installations/mailman> wget http://ftp.gnu.org/gnu/mailman/mailman-2.1.9.tgz
ExternalSources/Installations/mailman> tar -zxvf mailman-2.1.9.tgz
ExternalSources/Installations/mailman> cd mailman-2.1.9
ExternalSources/Installations/mailman/mailman-2.1.9> sudo groupadd mailman
ExternalSources/Installations/mailman/mailman-2.1.9> sudo useradd --comment "GNU Mailman" --shell /no/shell --home /no/home --gid mailman mailman
ExternalSources/Installations/mailman/mailman-2.1.9> sudo mkdir /usr/local/mailman
ExternalSources/Installations/mailman/mailman-2.1.9> sudo chgrp mailman /usr/local/mailman 
ExternalSources/Installations/mailman/mailman-2.1.9> sudo chmod a+rx,g+ws /usr/local/mailman
ExternalSources/Installations/mailman/mailman-2.1.9> ./configure --with-mail-gid=mailman --with-cgi-gid=apache --with-mailhost=ok-sat-library.org --with-urlhost=ok-sat-library.org
ExternalSources/Installations/mailman/mailman-2.1.9> make
ExternalSources/Installations/mailman/mailman-2.1.9> sudo make install
ExternalSources/Installations/mailman/mailman-2.1.9> sudo cp ../../../../OKsystem/Transitional/BuildSystem/ReleaseProcess/http.conf /usr/local/apache2/conf/httpd.conf
   \endverbatim
   Perhaps the next step should be part of the apache installation? 
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo mkdir /usr/local/apache2/htdocs/icons
ExternalSources/Installations/mailman/mailman-2.1.9> sudo cp /usr/local/mailman/icons/*.{jpg,png} /usr/local/apache2/htdocs/icons
ExternalSources/Installations/mailman/mailman-2.1.9> sudo sh -c "echo \"
IMAGE_LOGOS = '/icons/'
MTA='Postfix'\" >> /usr/local/mailman/Mailman/mm_cfg.py"
ExternalSources/Installations/mailman/mailman-2.1.9> cd /usr/local/mailman
/usr/local/mailman> sudo bin/genaliases
/usr/local/mailman> sudo chown mailman:mailman data/aliases*
/usr/local/mailman> sudo chmod g+w data/aliases*
/usr/local/mailman> sudo sh -c "echo 'alias_maps = hash:/etc/aliases, hash:/usr/local/mailman/data/aliases' >> /etc/postfix/main.cf"
/usr/local/mailman> sudo /usr/local/mailman/bin/newlist mailman
/usr/local/mailman> sudo crontab /usr/local/mailman/cron/crontab.in -u mailman

/usr/local/mailman> sudo /usr/local/apache2/bin/apachectl -k start
/usr/local/mailman> sudo postfix reload
/usr/local/mailman> sudo /usr/local/mailman/bin/newlist developers

/usr/local/mailman> sudo echo "
set daemon 120

poll aeternus.no-ip.org with proto IMAP and options no dns
         localdomains ok-sat-library.org
         user testlist with pass "testlist453"
         is * here options dropdelivered

smtphost localhost" > /etc/fetchmailrc
   \endverbatim
   </li>
   <li> Some explanation of the above configuration : 
    <ol>
     <li> "120" here is the number of seconds interval between polls (Ie 
     fetchmail checks for mail every 2 minutes) </li>
     <li> "aeternus.no-ip.org" is the mailserver I am pulling the mail from 
     (would be cs-svr1?) </li>
     <li>"testlist" is the my user on that mail server (ie might be developers 
     if using developers@swan.ac.uk) "testlist453" is my password on that 
     mailserver </li>
    </ol>
   </li>
   <li> Should this file be kept under version control even though it isn't part
   of the Release? (OK: which file? you mean /etc/fetchmailrc? and which
   release it isn't a part of? their release?)
   \verbatim
/usr/local/mailman> sudo chmod 710 /etc/fetchmailrc
/usr/local/mailman> sudo /usr/local/mailman/bin/mailmanctl start
/usr/local/mailman> sudo fetchmail -f /etc/fetchmailrc
   \endverbatim
   This should allow you to control mailinglists from 
   http://ok-sat-library.org/mailman/admin/ .
   (OK: Where does this address "http://ok-sat-library.org/mailman/admin/"
   come from?)
   </li>
  </ul>


  \todo Building Mailman in general
  <ul>
   <li> Initially following instructions from http://www.gnu.org/software/mailman/mailman-install/index.html </li>
   <li>Building the main program
    <ol>
     <li> First of all
     a user and group must be setup which will own mailmans
     installed files and under which it will run (clearly these commands have to
     be run as the superuser/root):
     \verbatim
groupadd mailman
useradd --comment "GNU Mailman" --shell /no/shell --home /no/home --gid mailman mailman
     \endverbatim
     </li>
     <li> Then the directory to install mailman into must be created (the default is
     /usr/local/mailman, this is referenced by "$prefix" in the documentation)
     and the setguid bit must be set:
     \verbatim
mkdir /usr/local/mailman/
cd /usr/local/mailman/
chgrp mailman .
chmod a+rx,g+ws .
     \endverbatim
     </li>
     <li> Now the configure script can be run. The main options here seem to be 
      <ul>
       <li> "--with-mail-gid "
       <p></p> This is a comma separated list of symbolically named groups or group ids
       which the mail program (Postfix, Exim etc, or possibly fetchmail etc (see below))
       which runs mailman will run mailman as. How the mail
       is delivered to mailman will effect which group(s) are set here.
       </li>
       <li> "--with-cgi-gid"
       <p></p> This is a comma seperated list of symbolically named groups or group ids
       which the web server will run mailman's cgi scripts under. This is most likely
       something like httpd, apache or www.
       OK: What does this mean?? In Buildsystem/ExternalSources/SpecialBuilds/plans/Apache.hpp
       one can see precisely how Apache was installed, and apparently this does
       exactly install the group "www".
       </li>
       <li> "--with-mailhost"
       <p></p> This is the default domain name that a new list will assume it is under
       (ie ok-sat-library.org) regarding email. This can be configured later by setting
       DEFAULT_EMAIL_HOST in $prefix/Mailman/mm_cfg.py.
       </li>
       <li> "--with-urlhost"
       <p></p> This is the default domain name that a new list will assume it is under
       (ie ok-sat-library.org) regarding web access. This can be configured later by
       setting DEFAULT_URL_HOST in $prefix/Mailman/mm_cfg.py .
       </li>
      </ul>
     </li>
     <li> I used a domain of my own in testing but have substituted ok-sat-library.org in
     it's place below:
     \verbatim
./configure --with-mail-gid=mail --with-cgi-gid=www --with-mailhost=ok-sat-library.org --with-urlhost=ok-sat-library.org
make
make install
     \endverbatim
     </li>
     <li> That is for the OKlibrary:
     \verbatim
ExternalSources> mkdir Installations/Mailman
ExternalSources> cd Installations/Mailman
Mailman> tar -xzf ../../sources/Mailman/mailman-2.1.9.tgz
Mailman> cd mailman-2.1.9
mailman-2.1.9> ./configure --with-mail-gid=mail --with-cgi-gid=www --with-mailhost=ok-sat-library.org --with-urlhost=ok-sat-library.org
mailman-2.1.9> make
mailman-2.1.9> sudo make install
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Hooking up to Apache
    <ol>
     <li> This should leave mailman installed in /usr/local/mailman/. Now assume
     apache has been setup for the given domain beforehand. </li>
     <li> How does this happen?? See Buildsystem/ExternalSources/SpecialBuilds/plans/Apache.hpp.
     Especially how do we create a test system locally on some machine, without
     Internet access? </li>
     <li> It can now be setup to allow
     web administration of mailman by adding the following lines to the virtual 
     host configuration (wherever the default vhost setup is stored):
     \verbatim
ScriptAlias /mailman/      /usr/local/mailman/cgi-bin/
Alias /pipermail/ /usr/local/mailman/archives/public/
<Directory /usr/local/mailman/>
Allow from all
Options FollowSymLinks
</Directory>
     \endverbatim
     </li>
     <li> Again, what is the "virtual host configuration (wherever the default vhost
     setup is stored)" ??? </li>
     <li> Copy the mailman icons across to somewhere web accessible (the main apache
     public directory for instance - although this and the next step appear to be purely
     asthetic, the mailman pages will work perfectly well without the icons, the images
     will simply be replaced with with their title text):
     \verbatim
cp /usr/local/mailman/icons/*.{jpg,png} /var/www/localhost/htdocs/icons
     \endverbatim
     </li>
     <li> What does this mean for the OKlibrary?
      <ol>
       <li> "Somewhere web accessible" is obviously not correct then. </li>
       <li> At this time, the ok-library-home page is located on the main departmental
       server (cs-svr1), while the mailing lists shall be installed on the
       main oklibrary-server (cs-oksvr). </li>
      </ol>
     </li>
     <li> And then set add the IMAGE_LOGOS option to $prefix/Mailman/mm_cfg.py to
     inform it of where the logos are, with respect to the url, not the local file path:
     \verbatim
IMAGE_LOGOS = '/icons/'
     \endverbatim
     </li>
     <li> DONE
     Of course Apache isn't the only choice here, any webserver capable of running
     cgi scripts should be just as valid,
     although Apache is very popular and widely supported, so it seems the most appropriate
     choice, especially when 
     considering the ability to transfer OKlibrary in it's entirety to other systems. </li>
    </ol>
   </li>
   <li> Setting up mail delivery to mailman
    <ol>
     <li> OK: The following discussion is utterly confusing for me, though the issues
     seems to me rather trivial:
      <ol>
       <li> As discussed in "Mailing lists" in Buildsystem/ReleaseProcess/plans/MailingLists.hpp,
       we map the public e-mail-lists (like "developers@ok-sat-library.org")
       to local e-mail-addresses. </li>
       <li> These are best on the departmental server cs-svr1 (so that the
       technical staff takes care about firewall-issues). </li>
       <li> The only question here seems to be whether one address is enough
       (we can separate out later the original different lists), or we need
       several e-mail-addresses. </li>
       <li> Then apparently we need some process running on the cs-oksvr (the
       machine where mailman will run) which fetches these e-mails (or
       should this be mailman itself?). </li>
       <li> And finally mailmen need to be told about the new e-mail, so that
       it does its act. </li>
      </ol>
      This needs to be achieved.
     </li>
     <li> Now some set of controls must be set up to pass mail onto mailman when it is
     received. The basic approach is that when newlist is called (see below), mailman
     returns a list of aliases which can be placed in /etc/mail/aliases or the equivalent
     on the system, and then when a mail arrives to whatever mail server
     is running on the machine, those aliases inform the mail server that the mailman
     command must be run with the given arguments. OK: What does it mean that
     an alias "informs" something? Please add more details, and this only
     regarding our case (see above). </li>
     <li> An example of this is:
     \verbatim
testlist:              "|/usr/local/mailman/mail/mailman post testlist"
testlist-admin:        "|/usr/local/mailman/mail/mailman admin testlist"
testlist-bounces:      "|/usr/local/mailman/mail/mailman bounces testlist"
testlist-confirm:      "|/usr/local/mailman/mail/mailman confirm testlist"
testlist-join:         "|/usr/local/mailman/mail/mailman join testlist"
testlist-leave:        "|/usr/local/mailman/mail/mailman leave testlist"
testlist-owner:        "|/usr/local/mailman/mail/mailman owner testlist"
testlist-request:      "|/usr/local/mailman/mail/mailman request testlist"
testlist-subscribe:    "|/usr/local/mailman/mail/mailman subscribe testlist"
testlist-unsubscribe:  "|/usr/local/mailman/mail/mailman unsubscribe testlist"
     \endverbatim
     OK: What is the meaning of any of these lines?
     </li>
     <li> However, this appears to be possible in a variety of ways. One of the
     discussed possibilities involved having all of the mailing list mails forwarded
     to another account. In this case, something like fetchmail
     could be used with something like the following in /etc/fetchmailrc:
     \verbatim
poll email.swan.ac.uk with proto IMAP and options no dns
         user O.Kullmann with pass "????"  is 'list' here options ssl keep

smtphost localhost
     \endverbatim
     OK: What is "anther account"? What type of "account"? For what purpose??
     </li>
     <li> Which, assuming fetchmail were setup properly to run as a daemon upon startup,
     would regularly pull the mail from the O.Kullmann@Swansea.ac.uk account down to the
     local mail server to a user "list" (whether or not this mail server can be sent mail
     from the outside should be irrelevant, it must only be able to route local mail and
     emails must somehow be able to be sent out) and then if this "list" user had a home
     directory and procmail were setup, something
     like the following could be placed in /home/list/.procmailrc:
     \verbatim
:0
* ^(To|X-Original-To|Cc).*testlist@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman post testlist


:0
* ^(To|X-Original-To|Cc).*testlist-admin@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman admin testlist

:0
* ^(To|X-Original-To|Cc).*testlist-bounces@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman bounces testlist

:0
* ^(To|X-Original-To|Cc).*testlist-confirm@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman confirm testlist

:0
* ^(To|X-Original-To|Cc).*testlist-join@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman join testlist

:0
* ^(To|X-Original-To|Cc).*testlist-join@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman join testlist


:0
* ^(To|X-Original-To|Cc).*testlist-leave@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman leave testlist

:0
* ^(To|X-Original-To|Cc).*testlist-owner@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman owner testlist

:0
* ^(To|X-Original-To|Cc).*testlist-request@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman request testlist

:0
* ^(To|X-Original-To|Cc).*testlist-subscribe@aeternus.no-ip.org 
| /usr/local/mailman/mail/mailman subscribe testlist

     \endverbatim
     Which would cause the given mailman commands to be run when mail was pulled
     down to the test user. OK: What is the meaning of any of these lines?
     What is the "O.Kullmann@Swansea.ac.uk-account" ?? Where is the local
     mail server --- university, department, cs-oksvr, or mailman??? </li>
     <li> I (MG) have tested this case myself and it appears to work perfectly well.
     Of course, if the email address to which the mailing list
     mail is forwarded/redirected to is actively used, then how to ensure that this
     pulling/reading of mail doesn't interfere  with normal usage/reading of mail or
     vice versa? </li>
     <li> Of course, in this case, a fetchmail and procmail solution isn't the only
     solution, would be it be more convenient or better suited to allow some other
     set of processes handle the delivery? For instance, mutt could be setup to call
     mailman by use of it's hooks feature when mail arrives, although this would be
     dependent on usage patterns of mutt as an email client. </li>
    </ol>
   </li>
   <li> Finishing mailman setup
    <ol>
     <li> However that is done, afterwards a default/sitewide mailing list for mailman
     must be created, entering details as necessary:
     \verbatim
/usr/local/mailman/bin/newlist mailman
     \endverbatim
     </li>
     <li> Then the cron  jobs that mailman needs, need to be setup:
     \verbatim
sudo crontab /usr/local/mailman/cron/crontab.in -u mailman
     \endverbatim
     </li>
     <li> The mailman daemon which handles the processing and sending of list messages
     can then be started:
     \verbatim
/usr/local/mailman/bin/mailmanctl start
     \endverbatim
     </li>
     <li> This should be setup to be started when the machine starts but how to do this
     will depend on what sort of system one uses (MG - how does cs-oksvr do this?
     was this done for apache? This usually involves some kind of init script in
     /etc/init.d)). OK: This should be done using Yast. </li>
     <li> Opening http://www.ok-sat-library.org/mailman/admin (assuming the domain of the
     system is www.ok-sat-library.org) should then offer options to create further lists
     and administrate users. A new list can then be created via this or via the newlist
     command line script as done previously when creating the default mailman list. </li>
    </ol>
   </li>
  </ul>

*/

