// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Mailman.hpp
  \brief Plans regarding installation of the Mailman package

  Mailman is for creating and maintaining mailing lists :

  <ul>
   <li> OKlibrary users subscribe to the list (either by visiting a website or 
   by sending an email to the "list"). </li>
   <li> Subscribers of this list can then send emails to the list which are 
   then forwarded to all other subscribers. </li>
   <li> Such behaviour can be altered, such as to require moderation of 
   emails sent to the list before sending them to subscribers, or to allow 
   non-subscribers to send to the list, for instance.
  </ul>


  \todo Building Mailman on freshly setup machine
  <ul>
   <li> Having installed Postfix (See 
   Buildsystem/ExternalSources/SpecialBuilds/plans/Postfix.hpp ) and Apache (See
   Buildsystem/ExternalSources/SpecialBuilds/plans/Apache.hpp ) the following steps 
   are necessary :
   <li> Download the latest mailman and extract it :
   \verbatim
ExternalSources/Installations/> mkdir mailman
ExternalSources/Installations/> cd mailman
ExternalSources/Installations/mailman> wget http://ftp.gnu.org/gnu/mailman/mailman-2.1.9.tgz
ExternalSources/Installations/mailman> tar -zxvf mailman-2.1.9.tgz
ExternalSources/Installations/mailman> cd mailman-2.1.9
   \endverbatim
   </li>
   <li> Add the system (linux/unix etc) groups which the mailman daemon will 
   run as :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo groupadd mailman
ExternalSources/Installations/mailman/mailman-2.1.9> sudo useradd --comment "GNU Mailman" --shell /no/shell --home /no/home --gid mailman mailman
   \endverbatim
   </li>
   <li> Create the directory to which mailman will be installed and set the 
   correct permissions :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo mkdir /usr/local/mailman
ExternalSources/Installations/mailman/mailman-2.1.9> sudo chgrp mailman /usr/local/mailman 
ExternalSources/Installations/mailman/mailman-2.1.9> sudo chmod a+rx,g+ws /usr/local/mailman
   \endverbatim
   </li>
   <li> Configure and then install mailman :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> ./configure --with-mail-gid=mailman --with-cgi-gid=daemon --with-mailhost=ok-sat-library.org --with-urlhost=ok-sat-library.org
ExternalSources/Installations/mailman/mailman-2.1.9> make
ExternalSources/Installations/mailman/mailman-2.1.9> sudo make install
   \endverbatim
   </li>
   <li> Some explanation is needed here on the configure options : 
    <ol>
     <li>"--with-mail-gid" specifies the group which the Mailman scriptwill be 
     run under (called upon receipt of an email for the mailing list, by the 
     mail server - Postfix). In this case, this should be mailman, as Postfix 
     runs the script (specified in the "aliases" file, discussed below) under 
     the user and group which own the aliases file. </li>
     <li> "--with-cgi-gid" specifies the system group which the web server (in 
     this case Apache) will be running under. This is needed to determine the 
     ownership of the files used to drive the web interface. This may vary, and 
     in this case was "daemon", but in some cases may be "apache", "httpd", 
     "www" or similar, as determined by the configuration of Apache during 
     installation.
     </li>
     <li> "--with-mailhost" specifies the DNS domain which the mailing list 
     email addresses will be sent with, and to which emails are sent. For 
     example, emails might be sent to "developers@ok-sat-library.org" by users
     of the list and so "ok-sat-library.org" would be the value of this option.
     </li>
     <li> "--with-urlhost" specifies the DNS domain which the webserver 
     (Apache) can be accessed at. </li>
    </ol>
   <li> What is the default apache user and group? It seems it is specified in 
   "http.conf" but this user and group must also exist on the system. </li>
   </li>
   <li> Apache must be configured to enable the Mailman web interface : 
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo sh -c "echo \"
ScriptAlias /mailman/      /usr/local/mailman/cgi-bin/
Alias /pipermail/ /usr/local/mailman/archives/public/
<Directory /usr/local/mailman/>
Allow from all
Options FollowSymLinks
</Directory>\" >> /usr/local/apache2/conf/httpd.conf"
   \endverbatim
   </li>
   <li> The above specifically says that accessing anything at 
   http://<domain>/mailman/ will be handled by mailman scripts stored in
   "/usr/local/mailman/cgi-bin/" on the server, and anything accessed at 
   http://<domain>/pipermail/ will access the public mailing list archives
   stored on the server in "/usr/local/mailman/archives/public/". <domain> 
   here is the DNS domain of the server (e.g "ok-sat-library.org") . </li>
   <li> The additional options ("Allow from all" etc) specify that scripts 
   and Apache configuration files within "/usr/local/mailman/cgi-bin/" have 
   sufficient permissions for some functions/configurations options within
   Apache, which are not normally available to scripts by default. </li>
   <li> It might be possible to copy the scripts above to Apache's public
   access directory, as with the icons, however it seems better to use
   this "ScriptAlias" etc method as then upgrading Mailman seems simpler and
   won't require making changes to Apache's configuration again. </li>
   <li> It appears the above step was previously documented as :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo cp ../../../../OKsystem/OKlib/Buildsystem/ReleaseProcess/httpd.conf /usr/local/apache2/conf/httpd.conf
   \endverbatim

   Should "http.conf" be included in the repository (it isn't currently)?
   </li>
   <li> The Mailman icons should them be moved to the correct location within
   Apache's public access folder :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo mkdir /usr/local/apache2/htdocs/icons
ExternalSources/Installations/mailman/mailman-2.1.9> sudo cp /usr/local/mailman/icons/*.{jpg,png} /usr/local/apache2/htdocs/icons
   \endverbatim
   </li>
   <li> How to have the mailman web interface accessible from 
   ok-sat-library.org but running on cs-oksvr? Is the web forwarding enough? 
   Is this necessary?
   </li>
   <li> Mailman must be told to use Postfix and the URL for the icons for the 
   web interface :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> sudo sh -c "echo \"
IMAGE_LOGOS = '/icons/'
MTA='Postfix'\" >> /usr/local/mailman/Mailman/mm_cfg.py"
   \endverbatim
   </li>
   <li> Mailman may be used with other mailservers or perhaps even called 
   directly by tools such as fetchmail, but Postfix appears to be the simplest
   and most flexible method of handling delivery of mail to Mailman. </li>
   </li>
   <li> Create the "aliases" for Postfix, to ensure mailing lists email is 
   delivered to mailman :
   \verbatim
ExternalSources/Installations/mailman/mailman-2.1.9> cd /usr/local/mailman
/usr/local/mailman> sudo bin/genaliases
/usr/local/mailman> sudo chown mailman:mailman data/aliases*
/usr/local/mailman> sudo chmod g+w data/aliases*
/usr/local/mailman> sudo sh -c "echo 'alias_maps = hash:/etc/aliases, hash:/usr/local/mailman/data/aliases' >> /etc/postfix/main.cf"
   \endverbatim
   </li>
   <li> When an email for the mailing list is delivered to Postfix (via 
   fetchmail), Postfix must decide which user's mailbox (usually a directory 
   in a given system user's home directory) to deliver the email to. </li>
   <li> "Aliases" define mappings between the user specified in the email being
   sent to (i.e developers@ok-sat-library.org), and system users, commands, or
   other processing to be done on the email. </li>
   <li> In this case, the aliases are defined so that any email's arriving for
   the mailing list mail users (developers, developers-bounce, 
   developers-subscribe etc) are delivered to the mailman script 
   ("/usr/local/mailman/mail/mailman") rather than to a user. </li>
   <li> This script then adds this message to a queue, and wakes 
   up the mailman daemon (mentioned below), which handles sending, archiving 
   of the message and so on. </li>
   <li> One must additionally inform Postfix that it should handle email 
   addressed to the mailing list domain. This requires altering the 
   "mydestination" option in "/etc/postfix/main.cf". This option can be
   added for new installs like so :
   \verbatim
sudo sh -c "echo \"
mydestination = $myhostname,localhost.$mydomain, ok-sat-library.org
\" >> /etc/postfix/main.cf"
   \endverbatim
   Or it can be altered in place like so, if Postfix has already been set
   up with this option : 
   \verbatim
sed -i "s/^mydestination \(.*\)/mydestination \1, ok-sat-library.org/" main.cf
   \endverbatim

   Obviously, this could also be set by simply editing the file.
   </li>
   <li> A default list to handle the administration of the mailman installation
   itself now needs to be setup :
   \verbatim
/usr/local/mailman> sudo /usr/local/mailman/bin/newlist mailman
   \endverbatim
   </li>
   <li> Setup cron jobs for regular tasks performed by mailman :
   \verbatim
/usr/local/mailman> sudo crontab /usr/local/mailman/cron/crontab.in -u mailman
   \endverbatim
   </li>
   <li> Start (or reload) Apache and Postfix :
   \verbatim
/usr/local/mailman> sudo /usr/local/apache2/bin/apachectl -k start
/usr/local/mailman> sudo postfix reload
   \endverbatim
   </li>
   <li> Start the mailman daemon :
   \verbatim
/usr/local/mailman> sudo /usr/local/mailman/bin/mailmanctl start
   \endverbatim

   At this stage one should have a working mailman installation.
   </li>
   <li> To setup the mailman daemon, "mailmanctl", to start automatically with
   the server, one should copy the startup script to "init.d" :
   \verbatim
/usr/local/mailman> sudo /usr/local/mailman/bin/mailmanctl stop
/usr/local/mailman> sudo cp /usr/local/mailman/scripts/mailman /etc/init.d/mailman
/usr/local/mailman> sudo chkconfig --add mailman
   \endverbatim

   This assumes a linux distribution that uses "chkconfig", more information on
   other configurations can be found at
   http://www.gnu.org/software/mailman/mailman-install/node42.html .
   </li>
   <li> We must then check that the startup script is working correctly, and 
   start the mailman daemon again :
   \verbatim
/usr/local/mailman> sudo /etc/init.d/mailman start
/usr/local/mailman> sudo ps auxww | grep mailmanctl | grep -v grep
   \endverbatim
   
   The last command above should result in non-empty output.
   </li>
   <li> Additionally one may create other lists in a similar manner, as with the
   "mailman" list :
   \verbatim 
/usr/local/mailman> sudo /usr/local/mailman/bin/newlist developers
   \endverbatim

   This may also be done via the web interface, which can be accessed at
   http://<domain>/mailman/admininfo where <domain> is the DNS domain of the
   webserver, for example "www.ok-sat-library.org" .
   </li>
   <li> In this specific case of the OKlibrary, there are issues with running 
   a mailserver which is externally visible to the internet, and so mailing
   list email must be forwarded to a seperate email account, and then "pulled" 
   down to the server with Postfix running on it, using a tool called "fetchmail".
   </li>
   <li> So, next, one may need to install fetchmail :
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
  \endverbatim
  </li>
  <li> Configure fetchmail to pull email from the correct account :
   \verbatim
/usr/local/mailman> sudo sh -c 'echo "
set daemon 120
set syslog

poll \"cs-svr1.swan.ac.uk\"
    localdomains ok-sat-library.org 
    protocol IMAP : 
    envelope \"To\" 
    user \"mailuser\" there with password \"PPP\" is * here 
    ssl

smtphost localhost" > /etc/fetchmailrc'
/usr/local/mailman> sudo chmod 710 /etc/fetchmailrc
   \endverbatim
   </li>
   <li> Some explanation of the above configuration : 
    <ol>
     <li> "120" here is the number of seconds interval between polls, i.e 
     fetchmail checks for new email every 2 minutes. </li>
     <li> "set syslog" informs fetchmail to log any messages or errors to
     the system log. </li>
     <li> "cs-svr1.swan.ac.uk" is the DNS domain of the  mailserver the mailing
     list email is originally delivered to. </li>
     <li> "localdomains ok-sat-library.org" specifies that emails with the 
     domain "ok-sat-library.org" should be considered local to this server
     and delivered to local users, i.e "developers@ok-sat-library.org" would be
     delivered to "developers" on this server (to Postfix). </li>
     <li> "protocol IMAP" specifies to use the IMAP protocol to retrieve email
     from the mail server, to which the emails for the mailing list are 
     initially delivered. </li>
     <li> 'envelope "To"' tells fetchmail to look at the "To" header of the 
     emails to determine where to deliver them to on the local server. This 
     is necessary as other headers that it might use will contain the 
     "@swan.ac.uk" address that the email was forwarded to. </li>
     <li> 'user "mailuser"' specifies the username which is used to log into
     the mail server, which the mailing list emails are initially delivered to.
     In this case for example, mailing list email would be forwarded to 
     "mailuser@swan.ac.uk" and fetchmail would log into "cs-svr1.swan.ac.uk"
     as "mailuser" to retrieve this email. </li>
     <li> 'password "PPP"' specifies that, when logging in to the mail server 
     to retrieve the mailing list emails, the password "PPP" should be used to 
     authenticate. </li>
     <li> "*" specifies that unrecognised email users, should simply be passed
     through to the same user on the system/local mail server (Postfix). For 
     example, "developers@ok-sat-library.org" is simply passed to Postfix as
     "developers" (Note : the "localdomains" option is also important here to 
     ensure fetchmail recognises "ok-sat-library.org" as an email for the 
     local mail server). </li>
     <li> "here" simply specifies that all mail retrieved should be delivered
     to the mail server specified by "smtphost". </li>
     <li> "ssl" simply specifies that SSL encryption and authentication should
     be used when retrieving email from mail server with the mailing list email
     on. This is important, as otherwise the password for this account may be
     sent in plaintext, over the network. </li>
    </ol>
   </li>
   <li> Start fetchmail : 
   \verbatim
/usr/local/mailman> sudo fetchmail -f /etc/fetchmailrc
   \endverbatim

   Mailman should now work and one can test this by going to
   http://<domain>/mailman/listinfo, subscribing to one of the lists
   and beginning to use them, where <domain> is the DNS domain of the 
   web server. </li>
   <li> Should "fetchmailrc" be kept under version control? </li>
   <li> (DONE This is specific to mailman and so seems sensible here)
   Perhaps the apache configuration above should be part of the apache 
   installation? </li>
  </ul>

  \todo Building Mailman on cs-wsok (for testing):
  <ul>
   <li> Building the main program </li>
   <li> DONE (Apache works, and so we use it)
   What about the web server: Is Apache the best choice for us?
   (Perhaps we need the web server anyway for the OKplatform-Internet
   page?) </li>
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


  \todo Update above instructions


  \todo Testing
  <ul>
   <li> How can we test the mailman installation on for example my (OK's) laptop
   (offline) ? (Together with the three related services provided by apache,
   fetchmail and postfix.) </li>
  </ul>


  \todo Installation script
  <ul>
   <li> Standard makefile for installing fetchmail. </li>
   <li> Standard makefile for installing mailman. </li>
  </ul>


  \todo Writing docus page
  <ul>
   <li> At least a quick overview is needed on what fetchmail and
   mailman are doing. </li>
  </ul>

*/

