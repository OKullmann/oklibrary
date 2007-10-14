// Matthew Gwynne, 9.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Postfix.hpp
  \brief Plans regarding installation of the Postfix package

  Postfix is for actively sending e-mails and possibly passively
  receiving e-mails:
  <ul>
   <li> Mailman could possibly send e-mails to remote mail-servers
   itself, but it seems much easier that Mailman delivers all e-mails
   to a local mail-server (postfix here), and then the local
   mail-server takes over and delivers it to the remote servers. </li>
   <li> Furthermore, Mailman needs to be notified about received
   e-mails. The best way to achieve this seems to be that some service
   (direct delivery or pulling the mail via e.g. fetchmail) delivers
   the e-mail to the local mail-server (Postfix(!)), which in turn
   has hooks for special actions for special e-mails, in this case
   for calling some dedicated mailman-script which hands over the
   e-mail to the Mailman-system. </li>
  </ul>

  Installing and using Postfix happens as follows:
  <ol>
   <li> Installing Postfix by the OKlibrary is only recommended if no
   Postfix is already in service on the machine (or, more generally,
   no e-mail is sent otherwise from this machine using some mail-server). </li>
   <li> If deemed necessary, then Postfix is installed (system-wide) as
   explained below. </li>
   <li> Later then, when installing Mailman, respective "aliases" have to
   be installed (then) to tell Postfix that certain e-mail-addresses shall trigger
   delivery to the Mailman-system. </li>
  <ul>


  \todo Building Postfix on a freshly setup test machine :
  <ul>
   <li>
   \verbatim  
ExternalSources/Installations> mkdir Postfix
ExternalSources/Installations> cd Postfix
ExternalSources/Installations/Postfix> wget ftp://ftp.fu-berlin.de/unix/mail/postfix/official/postfix-2.4.5.tar.gz
ExternalSources/Installations/Postfix> tar -zxvf postfix-2.4.5.tar.gz
ExternalSources/Installations/Postfix> cd postfix-2.4.5
ExternalSources/Installations/Postfix/postfix-2.4.5> make
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo groupadd postfix
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo useradd postfix -s /bin/false -g postfix
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo groupadd postdrop
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo mkdir /etc/postfix
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo cp ../../../../OKsystem/Transitional/BuildSystem/ReleaseProcess/main.cf /etc/postfix/main.cf
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo make upgrade
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo touch /etc/aliases
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo newaliases
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo echo "
filter f_mailinfo   { level(info)      and facility(mail); };
filter f_mailwarn   { level(warn)      and facility(mail); };
filter f_mailerr    { level(err, crit) and facility(mail); };
filter f_mail       { facility(mail); };

#
# Mail-messages in separate files:
#
destination mailinfo { file("/var/log/mail.info"); };
log { source(src); filter(f_mailinfo); destination(mailinfo); };

destination mailwarn { file("/var/log/mail.warn"); };
log { source(src); filter(f_mailwarn); destination(mailwarn); };

destination mailerr  { file("/var/log/mail.err" fsync(yes)); };
log { source(src); filter(f_mailerr);  destination(mailerr); };

#
# and also all in one file:
#
destination mail { file("/var/log/mail"); };
log { source(src); filter(f_mail); destination(mail); }; " >>  /etc/syslog-ng/syslog-ng.conf
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo syslog-ng restart
ExternalSources/Installations/Postfix/postfix-2.4.5> sudo postfix start
   \endverbatim
   </li>
   <li> The syslog step above might be hard to generalise as there are several 
   different syslog daemons and there will probably be preexisting configurations.
   How to solve this? OK: Which step is the "syslog step" ?
   </li>
   <li> The above configuration should go to a configuration-file. </li>
   <li> How can we make sure, that if we install sendmail, that then *our*
   installation is used by mailman? </li>
  </ul>


  \todo Installation script
  <ul>
   <li> Create the original postfix-configuration-file in
   Buildsystem/Configuration/ReleaseProcess. </li>
   <li> Add configuration-variables to adapt it. </li>
   <li> The Postfix-installation-makefile then creates from this template
   the final configuration-file (similar to the doxygen-configuration-file). </li>
   <li> Otherwise a simple standard makefile is written for installing Postfix, as
   above, but using three sub-targets:
    <ol>
     <li> main-installation </li>
     <li> syslog-configuration </li>
     <li> alias-installation </li>
    </ol>
    (Alias-installation here is independent of any specific mailman-issues.)
   </li>
  </ul>


  \todo Writing docus page
  <ul>
   <li> At least a quick overview is needed on what this thing is doing. </li>
  </ul>

*/
