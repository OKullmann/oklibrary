// Oliver Kullmann, 28.9.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef EMAILHANDLINGWAECHTER

#define EMAILHANDLINGWAECHTER

#include <cstdlib> // for system
#include <cstdio> // for remove

#include <sys/types.h>
#include <sys/stat.h> // for stat, chmod
#include <dirent.h> // for directories

#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <OKlib/General/TimeHandling.hpp>
#include <OKlib/General/StreamHandling.hpp>


namespace emailHandling {

  const std::string self = "emailHandling";

  const std::string email_self = "O.Kullmann@Swansea.ac.uk";
  const std::string mail_agent = "mutt";

  // -------------------------------------------------------------

  struct sendmail {

    struct Error {};
    struct open_file : Error {};
    struct write_file : Error {};
    struct get_file_stat : Error {};
    struct set_file_stat : Error {};

    sendmail() {}
    // does not throw

    sendmail(const std::string& message, const std::string& to, const std::string& subject = "", const std::string& attachment = "", const std::string& bcc = email_self) :
      m(message), t(to), s(subject), a(attachment), b(bcc) {}
    // does not throw
    // attachment is a space-separated list of file names
    // bcc is a space-separated list of e-mail-addresses

    int send_direct() const {

      std::string options;
      if (! s.empty())
	options += " -s \"" + s + "\"";
      if (! a.empty()) {
	std::istringstream L(a);
	std::string attach;
	while (L >> attach)
	  options += " -a " + attach;
      }
      if (! b.empty()) {
	std::istringstream L(b);
	std::string copy;
	while (L >> copy)
	  options += " -b " + copy;
      }
 
      const std::string command = "echo \"" + m + "\" | " + mail_agent + options + " " + t;
      return std::system(command.c_str());
    }
    // does not throw

    enum user_type { user_is_other, user_is_self };
    void send_indirect(const std::string& filename, const user_type ut = user_is_other) const {
      {
	std::ofstream file(filename.c_str(), std::ios_base::out);
	if (! file)
	  throw open_file();
	file << mail_agent << "\n";
	file << t << "\n";
	file << s << "\n";
	file << a << "\n";
	file << b << "\n";
	file << m << std::endl;
	if (! file)
	  throw write_file();
      }
      // user minus write, minus read; group minus read
      {
	struct stat S;
	if (stat(filename.c_str(), &S))
	  throw get_file_stat();
	if (ut == user_is_other) {
	  if (chmod(filename.c_str(), S.st_mode & ~ S_IWUSR & ~ S_IRUSR & ~ S_IRGRP))
	    throw set_file_stat();
	}
	else
	  if (chmod(filename.c_str(), S.st_mode & ~ S_IWUSR & ~ S_IRGRP & ~ S_IROTH))
	    throw set_file_stat();
      }
    }
    // throws Error; 

    std::string m;
    std::string t;
    std::string s;
    std::string a;
    std::string b;
  };

  // -------------------------------------------------------------

  class SendWaitingEmails {
  public :
    struct Error {};
    struct open_log_file : Error {};
    struct write_log_file : Error {};
    struct open_dir : Error {};
    struct get_file_stat : Error {};
    struct open_file : Error {};
    struct read_file : Error {};
    struct delete_file : Error {};
    struct send_mail : Error {};

    SendWaitingEmails(const std::string& logfile, const std::string& directory) :
      L(logfile), D(directory) {}
    // does not throw

    void send() const {

      std::ofstream log(L.c_str(), std::ios_base::out | std::ios_base::app);
      if (! log)
	throw open_log_file();

      log << "\n----------------------\n" << self << "\n" << TimeHandling::currentDateTime() << std::endl;
      if (! log)
	throw write_log_file();

      DIR* const dir = opendir(D.c_str());
      if (! dir) {
	log << "Error::open_dir" << std::endl;
	throw open_dir();
      }
      for (struct dirent* p = readdir(dir); p; p = readdir(dir)) {
	const char* const namep = p -> d_name;
	if (! std::strcmp(namep, ".") or ! std::strcmp(namep, ".."))
	  continue;
	log << " file name: " << namep << "\n";
	const std::string fullName = D + "/" + namep;
	struct stat S;
	if (stat(fullName.c_str(), &S)) {
	  log << "Error::get_file_stat" << std::endl;
	  throw get_file_stat();
	}
	if (S.st_mode & S_IWUSR)
	  continue;

	try {
	  StreamHandling::FileLines file(fullName.c_str());
	  StreamHandling::FileLines::iterator i = file.begin();
	  if (*i != mail_agent)
	    continue;
	  else
	    log << mail_agent << "\n";
	  const std::string& to = *++i;
	  log << "To: " << to << "\n";
	  const std::string& sub = *++i;
	  const std::string& a = *++i;
	  const std::string& b = *++i;
	  std::ostringstream message;
	  std::copy(++i, file.end(), std::ostream_iterator<std::string>(message, "\n"));
	  sendmail send(message.str(), to, sub, a, b);
	  if (send.send_direct()) {
	    log << "Error::send_mail" << std::endl;
	    throw send_mail();
	  }
	}
	catch (StreamHandling::Error_FileLines::open_file) {
	  log << "Error::open_file" << std::endl;
	  throw open_file();
	}
	catch (StreamHandling::Error_FileLines::read_file) {
	  log << "Error::read_file" << std::endl;
	  throw read_file();
	}

	if (std::remove(fullName.c_str())) {
	  log << "Error::delete_file" << std::endl;
	  throw delete_file();
	}

	if (! log)
	  throw write_log_file();
      }
      
    }
    // throws Error

  private :
    std::string L;
    std::string D;
  };

  bool check_e_mail_address(const std::string& address) {
    return address.find('@') != std::string::npos;
  }

  // Sending a serial e-mail ------------------------------------------------

  class Serial_e_mail {
  public :
    struct Error {
      virtual ~Error() {}
    };
    struct invalid_address : Error {};
    struct missing_name : Error {};
    struct missing_text : Error {};
    struct empty_dirname : Error {};
    
    Serial_e_mail(const std::string& form) : formula(form) {}

    void read_recipients(std::istream& is) {
      // throws invalid_address and mising_name
      using namespace std;
      string address;
      string name;
      while (is >> address) {
	if (not check_e_mail_address(address))
	  throw invalid_address();
	if (not (is >> name))
	  throw missing_name();
	string name_rest;
	getline(is, name_rest, '\n');
	list_recipients.push_back(recipient(address, name + name_rest));
      }
    }
    void read_subject(const std::string& sub) {
      subject = sub;
    }
    void read_text(const std::string& t) {
      text = t;
    }

    void send_direct() const { send(direct); }
    void send_indirect(const std::string& d) const {
      if (d.empty())
	throw empty_dirname();
      dirname = d;
      send(indirect);
    }

  private :
    std::string formula;
    struct recipient {
      recipient(const std::string& a, const std::string& n) :
	address(a), name(n) {}
      std::string address;
      std::string name;
    };
    std::vector<recipient> list_recipients;
    std::string subject;
    std::string text;
    
    static const std::string prefix; // for send_indirect
    mutable std::string dirname; // for send_indirect
    enum options { direct, indirect };

    void send(options o) const {
      using namespace std;
      if (text.empty()) throw missing_text();
      for (vector<recipient>::const_iterator i = list_recipients.begin(); i != list_recipients.end(); ++i) {
	const string& message = formula + " " + i -> name + ",\n\n" + text;
	sendmail Send(message, i -> address, subject, "", "");
	switch (o) {
	case direct : Send.send_direct(); break;
	case indirect : Send.send_indirect(dirname + "/" + prefix + i -> address, sendmail::user_is_self); break;
	}
      }
    }
  };
  const std::string Serial_e_mail::prefix = "MassenVersendung_";

}

#endif
