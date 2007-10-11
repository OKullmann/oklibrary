// Matthew Gwynne, 9.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Postfix.hpp
  \brief Plans regarding installation of the Postfix package

  Apparently, Postfix is for sending e-mails, and mailman (for the mailing lists)
  needs such a service.


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
   <li> A simple standard makefile is written for installing Postfix (as
   above). </li>
  </ul>


  \todo Writing docus page
  <ul>
   <li> At least a quick overview is needed on what this thing is doing. </li>
  </ul>

*/
