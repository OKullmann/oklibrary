// Oliver Kullmann, 28.9.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// use with
// -lcgicc

#ifndef CGIHANDLINGWAECHTER

#define CGIHANDLINGWAECHTER

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <ostream>
#include <sstream>

#include <sys/stat.h> // for chmod

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/FormFile.h>

#include <OKlib/General/emailHandling.hpp>
#include <OKlib/General/TimeHandling.hpp>
#include <OKlib/General/StringHandling.hpp>
#include <OKlib/General/StreamHandling.hpp>
#include <OKlib/General/AutomatedCompilation.hpp>
#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Kryptologie/HashMD5lib.hpp>

namespace CgiHandling {

  const std::string self = "CgiHandling";
  const std::string cgi_address = "/cgi-bin/ok_html/registration.cgi";

  const std::string name_authentication_field = "SerialName";

  const std::string name_type_field = "Type";
  const std::string type_original = "original";
  const std::string type_mirror = "mirror";
  const std::string type_query = "query";
  const std::string type_submission = "file submission";

  const std::string header_reprise = "You have entered the following data:";
  const std::string failed_test = "There has been a mistake:";
  const std::string failed_advice = "Please use the back button of your browser to correct the entry.";
  const std::string seems_ok1 = "If you want to submit the data, then please press the button.";
  const std::string seems_ok2 = "Otherwise use the back button of your browser to correct the entries.";
  const std::string manipulation = "The form data have been manipulated!";
  const std::string success = "You have successfully registered. You entered the following data:";
  const std::string passwd_notification = "Your password is:";
  const std::string success_headline = "You have successfully registered!";
  const std::string further = "A confirmation has been sent to your e-mail address (including your password).";
  const std::string further2 = "As long as registration is open, you can always override your data (except of the personal information).";
  const std::string refresh_notification = "Your site needs to be refreshed! Please use the back button of your browser, and then the refresh button. (In case this doesn't help and you disabled all local caching by your web browser, likely it is your Internet Service Provider, which doesn't give you access to the current web page, but only to an old version cached by your Internet Service Provider.)";
  const std::string invalid_page = "Your site is invalid!! Do not continue to use the current site provided by your browser!!! (If you are not trying to hack this site, then likely your browser uses an outdated version, and you need to REFRESH all your sites!)";


  // Exception classes
  // ---------------------------------------------------------------

  struct Error : ErrorHandling::Error {
    Error(const std::string& m) : ErrorHandling::Error(m) {}
    void add(const std::string& s) {what_ += s;}
  };

  struct serial_name_error : Error {
    serial_name_error(const std::string& m) : Error(m) {}
  };
  struct type_error : Error {
    type_error(const std::string& m) : Error(m) {}
  };
  struct open_log_file_error : Error {
    open_log_file_error(const std::string& m) : Error(m) {}
  };
  struct write_log_file_error : Error {
    write_log_file_error(const std::string& m) : Error(m) {}
  };
  struct invalide_key : Error {
    invalide_key(const std::string& m) : Error(m) {}
  };
  struct missing_hidden_field : Error {
    missing_hidden_field(const std::string& m) : Error(m) {}
  };
  struct missing_public_field : Error {
    missing_public_field(const std::string& m) : Error(m) {}
  };
  struct unknown_transfer_error : Error {
    unknown_transfer_error(const std::string& m) : Error(m) {}
  };
  struct mail_error : Error {
    mail_error(const std::string& m) : Error(m) {}
  };
  struct database_error : Error {
    database_error(const std::string& m) : Error(m) {}
  };
  struct save_program_file : Error {
    save_program_file(const std::string& m) : Error(m) {}
  };
  struct chmod_program_file : Error {
    chmod_program_file(const std::string& m) : Error(m) {}
  };
  struct open_program_file : Error {
    open_program_file(const std::string& m) : Error(m) {}
  };
  struct Libc : Error {
    Libc(const std::string& m) : Error(m) {}
  };
  struct system_call_error : Error {
    system_call_error(const std::string& m) : Error(m) {}
  };
  struct compilation_error : Error {
    compilation_error(const std::string& m) : Error(m) {}
  };
  struct different_outputs : Error {
    different_outputs(const std::string& m, const std::string& user, bool passed_when_ignored) : Error(m), u(user), ignore(passed_when_ignored) {}
    std::string u;
    bool ignore;
  };
  struct remove_program : Error {
    remove_program(const std::string& m) : Error(m) {}
  };
  struct conversion_error : Error {
    conversion_error(const std::string& m) : Error(m) {}
  };
  struct open_test_directory : Error {
    open_test_directory(const std::string& m) : Error(m) {}
  };
  struct read_correct_output : Error {
    read_correct_output(const std::string& m) : Error(m) {}
  };
  struct missing_refresh_field : Error {
    missing_refresh_field(const std::string& m) : Error(m) {}
  };
  struct unlock_impossible : Error {
    unlock_impossible(const std::string& m) : Error(m) {}
  };
  struct unknown_submission_type : Error {
    unknown_submission_type(const std::string& m) : Error(m) {}
  };
  struct error_output : Error {
    error_output(const std::string& m) : Error(m) {}
  };
  struct error_handling_done : Error {
    error_handling_done(const std::string& m) : Error(m) {}
  };
  struct error_from_own_library : Error {
    error_from_own_library(const std::string& m) : Error(m) {}
  };
  struct error_from_std_library : Error {
    error_from_std_library(const std::string& m) : Error(m) {}
  };
  struct unknown_error : Error {
    unknown_error(const std::string& m) : Error(m) {}
  };
  struct non_critical_program_abortion : Error {
    non_critical_program_abortion(const std::string& m) : Error(m) {}
  };
  struct critical_program_abortion : Error {
    critical_program_abortion(const std::string& m) : Error(m) {}
  };


  // Constants
  // ----------------------------

  const int hexdigits_passwd = 6;

  const int max_file_length = 40000;

  struct PublicFieldDeclarations {
    const char * const name;
    const char * const text;
  };

  typedef std::map<std::string, std::string> ValueMap;

  typedef std::string Testfunction(const ValueMap&);
  // returns the empty string iff no error
  // otherwise returns the (html) error message

  typedef std::string Identificationfunction(const ValueMap&);
  
  typedef void Transferfunction(const ValueMap&, const std::string&);
  // second argument is the password

  enum Return_Values { ok = 0, invalid_user_input = 1, transfer_error = 2 };

  // Main class
  // ---------------------------------------------------

  struct Submission {

    const std::string& page_title;
    const std::string& mail_dir;

    StreamHandling::LockedFileOstream LogFile;
    
    Submission(
	       const std::string& _page_title,
	       const std::string& serial_name,
	       const std::string& log_file,
	       const std::string& secret_key,
	       const char* const hidden_field_names[],
	       const unsigned int number_hidden_fields,
	       const PublicFieldDeclarations public_field_names[],
	       const unsigned int number_public_fields,
	       const std::string& _mail_dir,
	       Testfunction* testfunction,
	       Transferfunction* transferfunction,
	       Identificationfunction* identificationfunction,
	       Identificationfunction* emailaddressfunction,
	       const std::string& refresh_field,
	       const std::string& refresh_value
	       );
    // throws Error

  protected :
    void error_handling(Error err);

  };

}

// Implementation
// -------------------------------------------------------

namespace CgiHandling {

  inline void Submission::error_handling(Error err) {
    // rethrows err in case additional errors happen
    // otherwise throws error_handling_done
    using namespace cgicc;

    const std::string err_type = typeid(err).name();
    const std::string time = TimeHandling::currentDateTime();

    LogFile << "SYSTEM ERROR\n" << page_title << "\n" << time << "\n" << "Error type: " << err_type << "\n" << "Error content: " << err.what() << std::endl;
    if (! LogFile)
      err.add(" LOG STREAM ERROR!");

    emailHandling::sendmail emergency;
    emergency.t = emailHandling::email_self;
    emergency.b = "";
    emergency.s = self + ": FEHLER";
    emergency.m = page_title + "\n" + time + "\n" + "Fehler-Typ: " + err_type + "\nFehler-Inhalt: " + err.what();
    try {
      emergency.send_indirect(mail_dir + "/Emergency" + time);
    }
    catch(emailHandling::SendWaitingEmails::Error& f) {
      const std::string mail_error_type = typeid(f).name();
      std::cout << h1("Serious Malfunction") << "\n";
      std::cout << strong() << "Please contact " << a("me").set("href", "mailto:" + emailHandling::email_self) << "!" << strong() << "\n";
      std::cout << body() << html() << std::endl;
      err.add(" Emergency e-mail impossible! " + mail_error_type);
      LogFile << "Emergency e-mail impossible!\n" << "E-mail error type: " << mail_error_type << std::endl;
      if (! LogFile)
	err.add(" LOG STREAM ERROR!");
      throw err;
    }
    catch(...) {
	std::cout << h1("Serious Malfunction") << "\n";
	std::cout << strong() << "Please contact " << a("me").set("href", "mailto:" + emailHandling::email_self) << "!" << strong() << "\n";
	std::cout << body() << html() << std::endl;
	err.add(" Emergency e-mail impossible! Unknown error type.");
	LogFile << "Emergency e-mail impossible!\n" << "Unknown error type." << std::endl;
	if (! LogFile)
	  err.add(" LOG STREAM ERROR!");
	throw err;
      }
    
      std::cout << h1("Temporary Malfunction") << "\n";
      std::cout << p("Please try again later.") << "\n";
      std::cout << body() << html() << std::endl;
      throw error_handling_done(err.what() + " ERROR HANDLING of type " + err_type + " done.");
  }

  // ---------------------------------------

  struct direct_access {
    direct_access(const char* const * p) : p0(p) {}
    operator const char* () const {return *p0;}
    const char* const * const p0;
  };
  struct indirect_access {
    indirect_access(const PublicFieldDeclarations* const p) : p0(p) {}
    operator const char* () const {return p0 -> name;}
    const PublicFieldDeclarations* const p0;
  };
  template <class Access, class It>
  inline std::string read_variables(const cgicc::Cgicc& cgi, const It field_names_b, const It field_names_e, ValueMap& M) {
    const cgicc::const_form_iterator end_it = cgi.getElements().end();
    for (It i = field_names_b; i != field_names_e; ++i) {
      const char* const name = Access(i);
      cgicc::const_form_iterator const f = cgi.getElement(name);
      if (f == end_it)
	return name;
      M.insert(std::make_pair(name, **f));
    }
    return std::string();
  }
  // returns the first missing field name or the empty string

  // ------------------------------------------

  inline void provide_variables(const ValueMap& M) {
    for (ValueMap::const_iterator i = M.begin(); i != M.end(); ++i)
      std::cout << cgicc::input().set("type", "hidden").set("name", i -> first).set("value", i -> second) << "\n";
  }

  // -----------------------------------------

  inline std::string compute_password(const std::string& secret_key, const std::string& identification) {
    const std::string key = secret_key + identification;
    return StringHandling::deleteHead(hashMD5(key.c_str()), 26);
  }

  // --------------------------------------------------------------------
  // --------------------------------------------------------------------

  Submission::Submission(
		 const std::string& _page_title,
		 const std::string& serial_name,
		 const std::string& log_file,
		 const std::string& secret_key,
		 const char* const hidden_field_names[],
		 const unsigned int number_hidden_fields,
		 const PublicFieldDeclarations public_field_names[],
		 const unsigned int number_public_fields,
		 const std::string& _mail_dir,
		 Testfunction* const testfunction,
		 Transferfunction* const transferfunction,
		 Identificationfunction* identificationfunction,
		 Identificationfunction* emailaddressfunction,
		 const std::string& refresh_field,
		 const std::string& refresh_value
		 ) :
    page_title(_page_title), mail_dir(_mail_dir), LogFile(log_file)
  {

    try {

      using namespace cgicc;

      // Send HTTP header
      std::cout << HTTPHTMLHeader() << "\n";
      // Set up the HTML document
      std::cout << html() << head(title(page_title)) << "\n";
      std::cout << body() << "\n";

      const std::string time = TimeHandling::currentDateTime();

      StreamHandling::LockedFileOstream LogFile(log_file);
      if (! LogFile)
	error_handling(open_log_file_error("Can't open log file!"));

      cgicc::Cgicc cgi;
      {
	cgicc::CgiEnvironment Env(cgi.getEnvironment());
	LogFile << "\n-----------------------\n" << self << "\n" << time << "\n";
	LogFile << "Content length: " << Env.getContentLength() << "\n";
	LogFile << "Remote address: " << Env.getRemoteAddr() << "\n";
	LogFile << "Remote host: " << Env.getRemoteHost() << "\n";
	LogFile << "Remote identity: " << Env.getRemoteIdent() << "\n";
	LogFile << "Remote user: " << Env.getRemoteUser() << "\n";
	LogFile << "User agent: " << Env.getUserAgent() << "\n";
      }

      const cgicc::const_form_iterator end_it = cgi.getElements().end();

      ValueMap hidden_f;
      {
	const std::string missing_hidden = read_variables<direct_access>(cgi, hidden_field_names, hidden_field_names + number_hidden_fields, hidden_f);
	if (! missing_hidden.empty()) {
	  std::cout << h2() << invalid_page << h2() << std::endl;
	  error_handling(missing_hidden_field("Missing hidden field: " + missing_hidden));
	}
      }
      ValueMap public_f;
      {
	const std::string missing_public = read_variables<indirect_access>(cgi, public_field_names, public_field_names + number_public_fields, public_f);
	if (! missing_public.empty()) {
	  std::cout << h2() << invalid_page << h2() << std::endl;
	  error_handling(missing_public_field("Missing public field: " + missing_public));
	}
      }
      
      LogFile << "Hidden input Fields:" << "\n";
      for (ValueMap::const_iterator i = hidden_f.begin(); i != hidden_f.end(); ++i)
	LogFile << i -> first << ", " << i -> second << "\n";
      LogFile << "Public input Fields:" << "\n";
      for (ValueMap::const_iterator i = public_f.begin(); i != public_f.end(); ++i)
	LogFile << i -> first << ", " << i -> second << "\n";
      
      {
	cgicc::const_form_iterator const authentication = cgi.getElement(name_authentication_field);
	if (authentication == end_it) {
	  std::cout << h2() << invalid_page << h2() << std::endl;
	  error_handling(serial_name_error("Missing special input field " + name_authentication_field));
	}
	if (**authentication != serial_name) {
	  std::cout << h2() << invalid_page << h2() << std::endl;
	  error_handling(serial_name_error("Wrong value of authentication field " + name_authentication_field + ": " + **authentication));
	}
	LogFile << "Authentication field: " << serial_name << "\n";
      }
      
      cgicc::const_form_iterator const type = cgi.getElement(name_type_field);
      if (type == end_it) {
	std::cout << h2() << invalid_page << h2() << std::endl;
	error_handling(type_error("Missing special input field " + name_type_field));
      }
      LogFile << "Type field: " << **type << "\n";
      cgicc::const_form_iterator const refresh = cgi.getElement(refresh_field);
      if (refresh == end_it) {
	std::cout << h2() << invalid_page << h2() << std::endl;
	error_handling(missing_refresh_field("Missing special input field " + refresh_field));
      }
      if (**refresh != refresh_value) {
	std::cout << strong() << refresh_notification << strong() << "\n";
	LogFile << "Wrong refresh value: " << **refresh << "\nCorrect refresh value: " << refresh_value << std::endl;
      }
      else if (**type == type_original) { // original form
	std::cout << h1(header_reprise) << "\n";
	std::cout << dl() << "\n";
	{
	  for (const PublicFieldDeclarations* p = public_field_names; p != public_field_names + number_public_fields; ++p)
	    std::cout << dt(p -> text) << dd(public_f[p -> name]) << "\n";
	} // G++ ERROR: wants to use p also later!!!
	std::cout << hr();
	const std::string test_result = testfunction(public_f);
	if (not test_result.empty()) {
	  std::cout << h2(failed_test) << p() << test_result << p() << "\n";
	  std::cout << strong(failed_advice) << std::endl;
	  LogFile << "User input error:\n" << test_result << "\n";
	}
	else { // no error
	  std::cout << p().set("style", "text-align: center") << seems_ok1 << br() << seems_ok2 << br() << "\n";
	  std::cout << form().set("action", "\"" + cgi_address + "\"").set("method", "post") << "\n";
	  std::cout << p() << "\n";
	  std::cout << "<center>" << input().set("type", "submit") << "</center>" << "\n";
	  provide_variables(public_f); provide_variables(hidden_f);
	  std::cout << input().set("type", "hidden").set("name", name_authentication_field).set("value", serial_name) << "\n";
	  std::cout << input().set("type", "hidden").set("name", name_type_field).set("value", type_mirror) << "\n";
	  std::cout << form() << "\n";
	  LogFile << "User input seems ok.\n";
	}
      }
      
      else if (**type == type_mirror) { // mirror form
	const std::string test_result = testfunction(public_f);
	if (not test_result.empty()) {
	  std::cout << h2(manipulation) << "\n";
	  error_handling(invalide_key("The double check failed! Error message: " + test_result));
	  // this check has to be replaced by the use of a HASH function
	}
	const std::string identification = identificationfunction(public_f);
	const std::string passwd = compute_password(secret_key, identification);
	try {
	  transferfunction(public_f, passwd);
	  // also hidden_f should be used here!
	}
	catch (const ErrorHandling::Error& e) {
	  LogFile << "Exception from own library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_own_library("Error when transferring to the database!"));
	}
	catch (const std::exception& e) {
	  LogFile << "Exception from standard library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_std_library("Error when transferring to the database!"));
	}
	catch (...) {
	  LogFile << "Error with transferfunction" << std::endl;
	  error_handling(unknown_transfer_error("Error when transferring to the database!"));
	}
	const std::string email = emailaddressfunction(public_f);
	emailHandling::sendmail notification;
	notification.t = email;
	notification.b = "";
	notification.s = page_title;
	notification.m = page_title + "\n";
	notification.m += success + "\n\n";
	{
	  for (const PublicFieldDeclarations* p = public_field_names; p != public_field_names + number_public_fields; ++p)
	    notification.m += (p -> text) + std::string(" ") + (public_f.find(p -> name) -> second) + std::string("\n");
	}
	notification.m += "\n";
	notification.m +=  passwd_notification + "\n" + passwd + "\n";
	try {
	  notification.send_indirect(mail_dir + "/Notification" + email);
	}
	catch(...) {
	  error_handling(mail_error("Can not send notification to " + email));
	}
	std::cout << h1(success_headline) << "\n";
	std::cout << p(further) << "\n";
	std::cout << p(further2) << "\n";
      }
      
      else if (**type == type_query) { // query form
	std::string test_result;
	try {
	  test_result = testfunction(public_f);
	}
	catch (database_error& e) {
	  LogFile << "Error with database" << std::endl;
	  error_handling(e);
	}
	catch (const Error& e) {
	  LogFile << "Error with testfunction" << std::endl;
	  error_handling(e);
	}
	catch (const ErrorHandling::Error& e) {
	  LogFile << "Exception from own library thrown by testfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_own_library("Exception thrown by testfunction!"));
	}
	catch (const std::exception& e) {
	  LogFile << "Exception from standard library thrown by testfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_std_library("Exception thrown by testfunction!"));
	}
	catch (...) {
	  LogFile << "Error with testfunction" << std::endl;
	  error_handling(unknown_transfer_error("Test function has thrown unknown exception!"));
	}
	if (not test_result.empty()) {
	  std::cout << test_result;
	  LogFile << "Identification error! Error messsage:\n" << test_result << "\n";
	}
	else {
	  try {
	    transferfunction(public_f, "");
	    // no difference between hidden and public here;
	    // no password created
	  }
	  catch (database_error& e) {
	    LogFile << "Error with database" << std::endl;
	    error_handling(e);
	  }
	  catch (Error& e) {
	    LogFile << "Error with transferfunction" << std::endl;
	    error_handling(e);
	  }
	  catch (const ErrorHandling::Error& e) {
	    LogFile << "Exception from own library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	    error_handling(error_from_own_library("Exception thrown by transferfunction!"));
	  }
	  catch (const std::exception& e) {
	    LogFile << "Exception from standard library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	    error_handling(error_from_std_library("Exception thrown by transferfunction!"));
	  }
	  catch (...) {
	    LogFile << "Error with transferfunction" << "\n";
	    error_handling(unknown_transfer_error("Error when transferring data from the database!"));
	  }
	  LogFile << "Display of data seems to be okay.\n";
	}
      }
      
      else if (**type == type_submission) { // submission form
	std::string test_result;
	try {
	  test_result = testfunction(public_f);
	}
	catch (database_error& e) {
	  LogFile << "Error with database" << std::endl;
	  error_handling(e);
	}
	catch (const Error& e) {
	  LogFile << "Error with testfunction" << std::endl;
	  error_handling(e);
	}
	catch (const ErrorHandling::Error& e) {
	  LogFile << "Exception from own library thrown by testfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_own_library("Exception thrown by testfunction!"));
	}
	catch (const std::exception& e) {
	  LogFile << "Exception from standard library thrown by testfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
	  error_handling(error_from_std_library("Exception thrown by testfunction!"));
	}
	catch (...) {
	  LogFile << "Error with testfunction" << std::endl;
	  error_handling(unknown_transfer_error("Test function has thrown unknown exception!"));
	}
	if (not test_result.empty()) {
	  std::cout << test_result;
	  LogFile << "Identification error! Error messsage:\n" << test_result << std::endl;
	}
	else { // identification check ok
	  cgicc::const_file_iterator const file = cgi.getFile(secret_key);
	  if (file == cgi.getFiles().end()) {
	    LogFile << "No file uploaded." << std::endl;
	    std::cout << strong() << "You have not uploaded a file!" << strong() << "\n";
	    std::cout << p() << "Please use the back button of your browser and try again." << p() << "\n";
	  }
	  else { // file was successfully uploaded
	    const std::string& remote_name = file -> getFilename();
	    LogFile << "Filename (remote): " << remote_name << std::endl;
	    const std::string::size_type length = file -> getDataLength();
	    LogFile << "File length = " << length << std::endl;
	    std::cout << p() << "You have uploaded file " << remote_name << "." << br() << "\n";
	    std::cout << "The file has " << length << " bytes." << p() << "\n";
	    if (length > max_file_length) {
	      LogFile << "ERROR: File too long!\nCurrent maximal length is " << max_file_length << " bytes." << std::endl;
	      std::cout << strong() << "You are not allowed to upload files of this size!" << strong() << "\n";
	      std::cout << p() << "Perhaps you have chosen the wrong file. Please check and try again." << p() << "\n";
	    }
	    else { // file is not too large
	      {
		const std::string& full_name = identificationfunction(public_f);
		std::ofstream f(full_name.c_str());
		file -> writeToStream(f);
		if (! f)
		  error_handling(save_program_file("Can not save " + full_name));
		LogFile << "Saved file " << full_name << std::endl;
		f.close();
		// user read, others read, group nothing
		if (chmod(full_name.c_str(), S_IRUSR | S_IROTH))
		  error_handling(chmod_program_file("Can not change mode of " + full_name));
	      }
	      bool continuation = true;
	      try {
		transferfunction(public_f, "");
		// no difference between hidden and public here;
	      }
	      catch (const Libc& e) {
		LogFile << "Libc found!" << std::endl;
		error_handling(e);
	      }
	      catch (const compilation_error& e) {
		LogFile << "Compilation error in transferfunction;\n" << e.what() << std::endl;
		continuation = false;
	      }
	      catch (const non_critical_program_abortion& e) {
		LogFile << "Non-critical program abortion in transferfunction;\n" << e.what() << std::endl;
		continuation = false;
	      }
	      catch (const critical_program_abortion& e) {
		LogFile << "Critical program abortion in transferfunction;\n" << e.what() << std::endl;
		error_handling(e);
	      }
	      catch (const different_outputs& e) {
		LogFile << "Different outputs: " << e.what() << std::endl;
		LogFile << "User output:\n" << e.u << std::endl;
		LogFile << "Result under ignoration: " << e.ignore << std::endl;
		continuation = false;
	      }
	      catch (const error_output& e) {
		LogFile << "Error in computation. Test output:\n" << e.what() << "\n";
		continuation = false;
	      }
	      catch (const database_error& e) {
		LogFile << "Error with database" << std::endl;
		error_handling(e);
	      }
	      catch (const Error& e) {
		error_handling(e);
	      }
	      catch (const ErrorHandling::Error& e) {
		LogFile << "Exception from own library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
		error_handling(error_from_own_library("Exception thrown by transferfunction!"));
	      }
	      catch (const std::exception& e) {
		LogFile << "Exception from standard library thrown by transferfunction:\n" << ErrorHandling::Error2string(e) << std::endl;
		error_handling(error_from_std_library("Exception thrown by transferfunction!"));
	      }
	      catch (...) {
		LogFile << "Error with transferfunction" << std::endl;
		error_handling(unknown_transfer_error("Unknown error in submission procedure"));
	      }
	      if (continuation)
		LogFile << "Task successfully submitted.\n";
	      else
		LogFile << "Submission unsuccessful.\n";
	    }
	  }
	}
      }
      
      else // error with formular type
	error_handling(type_error("Wrong value of type field " + name_type_field + ": " + **type));
      
      // Close the HTML document
      std::cout << body() << html() << std::endl;

      LogFile << "Completed without system errors." << std::endl;
      if (! LogFile)
	error_handling(write_log_file_error("Can't write to log file!"));
    }
    catch (const error_handling_done& e) {
      LogFile << "Exit after successful error handling." << std::endl;
      throw e;
    }
    catch (const Error& e) {
      LogFile << "Exit after UNSUCCESSFUL error handling." << std::endl;
      throw e;
    }
    catch (const ErrorHandling::Error& e) {
      const std::string& content = ErrorHandling::Error2string(e);
      LogFile << "Exit via unhandled error from own library (" << TimeHandling::currentDateTime() << "):\n" << content << std::endl;
      throw error_from_own_library("Unhandled error from own library: " + content);
    }
    catch (const std::exception e) {
      const std::string& content = ErrorHandling::Error2string(e);
      LogFile << "Exit via unhandled error from standard library (" << TimeHandling::currentDateTime() << "):\n" << content << std::endl;
      throw error_from_std_library("Unhandled error from standard library: " + content);
    }
    catch (...) {
      LogFile << "Exit via unknown error (" << TimeHandling::currentDateTime() << ")." << std::endl;
      throw unknown_error("Submission: Unknown error!");
    }
    return;
  } // end constructor submission


  class HTML_Output {
  public :
    HTML_Output(const std::string& page_title) {
      using namespace cgicc;
      using namespace std;
      cout << HTTPHTMLHeader() << "\n";
      cout << html() << head(title(page_title)) << "\n";
      cout << body() << "\n";
    }
    ~HTML_Output() {
      using namespace cgicc;
      using namespace std;
      cout << body() << html() << endl;
    }
  };

}


#endif
