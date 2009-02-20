// Oliver Kullmann, 10.10.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/General/SystemHandling.hpp
  \brief System calls
*/

#ifndef SYSTEMHANDLINGWAECHTER
#define SYSTEMHANDLINGWAECHTER

#include <string>
#include <fstream>
#include <istream>
#include <ostream>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

// #include <iostream> // debugging

#include <cstdlib> // for system, getenv
#include <cstdio> // for tmpnam and remove
#include <sys/wait.h> // for return value of std::system, and for waitpid
#include <sys/types.h>
#include <unistd.h> // for chdir
#include <fcntl.h>
#include <sys/resource.h> // for rlimit ...
#include <dirent.h> // for opendir ...
#include <cstring> // stdcmp ...
#include <signal.h> // for signal macros
#include <sys/stat.h> // for file mode bits

#include <OKlib/General/StringHandling.hpp>
#include <OKlib/General/StreamHandling.hpp>
#include <OKlib/General/ErrorHandling.hpp>
#include <OKlib/General/MessageHandling.hpp>


namespace SystemHandling {

  struct SystemError : ErrorHandling::Error {
  public :
    SystemError(const std::string& what) : ErrorHandling::Error(what) {}
  };


  // --------------------------------------------
  // File handling
  // --------------------------------------------

  inline std::string base_name(const std::string& s) {
    // base_name("/abc/dk/file.uvw") = "file.uvw"
    const std::string::size_type p = s.rfind('/');
    if (p == std::string::npos)
      return s;
    else
      return s.substr(p+1, std::string::npos);
  }
    

  // --------------------------------------------
  // Directory handling
  // --------------------------------------------

  struct open_directory : SystemError {
    open_directory(const std::string& s) : SystemError(s) {}
  };

  inline std::vector<std::string> dir2filelist(const std::string& dirname) {
    std::vector<std::string> V;
    DIR* const p = opendir(dirname.c_str());
    if (p == NULL)
      throw open_directory(dirname);
    const dirent* entry;
    while ((entry = readdir(p)) != NULL) {
      const char* const name = entry -> d_name;
      if (strcmp(name, ".") and strcmp(name, ".."))
	V.push_back(name);
    }
    sort(V.begin(), V.end());
    return V;
  }

  bool create_dir(const std::string& name) {
    return mkdir(name.c_str(), S_IRWXU) == 0;
  }


  // --------------------------------------------
  // System calls
  // --------------------------------------------

  struct create_file : SystemError {
    create_file(const std::string& s) : SystemError(s) {}
  };
  struct open_file : SystemError {
    open_file(const std::string& s) : SystemError(s) {}
  };
  struct system_error : SystemError {
    system_error(const std::string& s) : SystemError(s) {}
  };


  class SystemOutput {

  public :

    SystemOutput() :
      stdout(new std::stringstream),
      stderr(new std::stringstream)
    {}
    SystemOutput(const SystemOutput& O) {
      stdout = O.stdout; stderr = O.stderr; c = O.c;
      O.stdout = 0; O.stderr = 0;
    }
    ~SystemOutput() { delete stdout; delete stderr; }

    operator bool () const { return WIFEXITED(c); }
    operator std::string () const { return stdout -> str(); }
    std::string err() const { return stderr -> str(); }
    int code() const { return c; }
    bool aborted() const { return WIFSIGNALED(c); }

  private :

    friend class System_sh;
    friend class System_bash;
    mutable std::stringstream* stdout;
    mutable std::stringstream* stderr;
    int c;
    SystemOutput& operator = (const SystemOutput&); // not allowed
  };


// Use as follows:

// #include <OKlib/General/SystemHandling.hpp>
// SystemHandling::System_sh S;
// SystemHandling::SystemOutput out = S("the command");

// Now you have the following operations:
// bool(out) is false if the operation was unsuccesful;
// out() is what the operation has put on standard output
// out.err() is what the operation has put on standard error
// out.code() is the return code
// out.aborted() is true if the operation was aborted.

  class System_sh {
    // executes a system command and returns SystemOutput

  public :

    System_sh() // using a temporary file for stdout named by the system
      : name_out(std::tmpnam(0)), name_err(std::tmpnam(0))
    { create_files(); }

    System_sh(const std::string& name) // using a temporary file named name for
      // stdout
      : name_out(name), name_err(std::tmpnam(0))
    { create_files(); }

    SystemOutput operator () (const std::string& command) const {
      // returns the output of the command
      SystemOutput O;
      const std::string& full_command = command + " > " + name_out + " 2> " + name_err;
      O.c = std::system(full_command.c_str());
      {
	std::ifstream filestdout(name_out.c_str());
	if (! filestdout)
	  throw open_file(name_out);
	*O.stdout << filestdout.rdbuf();
      }
      {
	std::ifstream filestderr(name_err.c_str());
	if (! filestderr)
	  throw open_file(name_err);
	*O.stderr << filestderr.rdbuf();
      }
      return O;
    }

    ~System_sh() {
      std::remove(name_out.c_str());
      std::remove(name_err.c_str());
    }

  private :

    std::string name_out;
    std::string name_err;

    void create_files() const {
      std::ofstream filestdout(name_out.c_str());
      if (! filestdout)
	throw create_file(name_out);
      std::ofstream filestderr(name_err.c_str());
      if (! filestderr)
	throw create_file(name_err);
    }

    System_sh(const System_sh&); // not allowed
    System_sh& operator = (const System_sh&); // not allowed

  };


  class System_bash : public System_sh {
    // Example for a small process:
    //   System_bash S;
    //   S.set_time(1);
    //   S.set_v_mem(2650); // necessary for bash needs
    //   S.set_core(0);
    //   S.set_file(3);
    //   S.set_n_files(15);

  public :

    System_bash() // using a temporary file named by the system
      : System_sh(), name_return_status(std::tmpnam(0)),
	use_core(false), use_data(false), use_file(false), use_mem(false), use_stack(false), use_time(false), use_n_files(false), use_n_proc(false), use_v_mem(false)
    { create_files(); }
    System_bash(const std::string& name) // using a temporary file named name for stdout
      : System_sh(name), name_return_status(std::tmpnam(0)),
	use_core(false), use_data(false), use_file(false), use_mem(false), use_stack(false), use_time(false), use_n_files(false), use_n_proc(false), use_v_mem(false)
    { create_files(); }
    ~ System_bash() {
      std::remove(name_return_status.c_str());
    }

    SystemOutput operator () (const std::string& command) const {
      // returns the output of the command
      const std::string& extended_command = bash_ulimit(command);
      SystemOutput O = System_sh::operator ()(extended_command);
      O.c = StringHandling::fromString<int>(StreamHandling::file2string(name_return_status));
      return O;
    }
    // ToDo: what about end-of-line symbols in the output?

    void set_core(unsigned int c) {
      use_core = true; core = c;
    }
    void unset_core() { use_core = false; }
    void set_data(unsigned int d) {
      use_data = true; data = d;
    }
    void unset_data() { use_data = false; }
    void set_file(unsigned int f) {
      use_file = true; file = f;
    }
    void unset_file() { use_file = false; }
    void set_mem(unsigned int m) {
      use_mem = true; mem = m;
    }
    void unset_mem() { use_mem = false; }
    void set_stack(unsigned int s) {
      use_stack = true; stack = s;
    }
    void unset_stack() { use_stack = false; }
    void set_time(unsigned int t) {
      use_time = true; time = t;
    }
    void unset_time() { use_time = false; }
    void set_n_files(unsigned int nf) {
      use_n_files = true; n_files = nf;
    }
    void unset_n_files() { use_n_files = false; }
    void set_n_proc(unsigned int np) {
      use_n_proc = true; n_proc = np;
    }
    void unset_n_proc() { use_n_proc = false; }
    void set_v_mem(unsigned int v) {
      use_v_mem = true; v_mem = v;
    }
    void unset_v_mem() { use_v_mem = false; }

  private :

    std::string name_return_status; 

    void create_files() const {
      std::ofstream filereturn(name_return_status.c_str());
      if (! filereturn)
	throw create_file(name_return_status);
    }

    unsigned int core; bool use_core; // in blocks
    unsigned int data; bool use_data; // in kilobytes
    unsigned int file; bool use_file; // in blocks
    unsigned int mem; bool use_mem; // in kilobytes
    unsigned int stack; bool use_stack; // in kilobytes
    unsigned int time; bool use_time; // in seconds
    unsigned int n_files; bool use_n_files; // seems to mean only this process
    // and sub-processes
    unsigned int n_proc; bool use_n_proc; // seems to count all processes
    unsigned int v_mem; bool use_v_mem; // in kilobytes

    std::string bash_ulimit(const std::string& com) const {
      using namespace StringHandling;

      std::string res;
      if (use_core)
	res += " -c " + toString(core);
      if (use_data)
	res += " -d " + toString(data);
      if (use_file)
	res += " -f " + toString(file);
      if (use_mem)
	res += " -m " + toString(mem);
      if (use_stack)
	res += " -s " + toString(stack);
      if (use_time)
	res += " -t " + toString(time);
      if (use_n_files)
	res += " -n " + toString(n_files);
      if (use_n_proc)
	res += " -u " + toString(n_proc);
      if (use_v_mem)
	res += " -v " + toString(v_mem);

      if (res.empty())
	return "bash -c '" + com+  ";echo $? > " + name_return_status + "'";
      else
	return "bash -c 'ulimit" + res + "; " + com + ";echo $? > " + name_return_status + "'";
    }

    System_bash(const System_bash&); // not allowed
    System_bash& operator = (const System_bash&); // not allowed

  };

  // ---------------------------------------------------------------
  // System calls without intermediate sh or bash
  // ---------------------------------------------------------------

  class PidType {
  public :
    PidType() : s(0) {}
    bool normal() const { return WIFEXITED(s); }
    // normal termination
    int status() const { return WEXITSTATUS(s); }
    // return code in case of normal termination

    bool aborted() const { return WIFSIGNALED(s); }
    // terminated by unhandled signal
    int a_signal() const { return WTERMSIG(s); }
    // number of signal in case of abortion

    bool stopped() const { return WIFSTOPPED(s); }
    // stopped by a signal
    int s_signal() const { return WSTOPSIG(s); }
    // number of signal if stopped

  private :
    pid_t s;
    friend class System;
  };

  namespace Implementation_pid2string {
    const int AnzahlSprachen = 2;
    const char * const Meldungen_pid [][AnzahlSprachen] = {
      {"Normale Terminierung mit Rueckgabewert = ", // 0
       "Normal termination with return value = "},
      {"Prozess abgebrochen wegen nicht behandeltem Signal mit Nummer = ", // 1
       "Process aborted due to unhandled signal with number = "},
      {"Prozess angehalten durch Signal mit Nummer = ", // 2
       "Process stopped by signal with number = "},
      {"Pid-Typ kann nicht interpretiert werden!", // 3
       "Pid-Type can not be interpreted!"},
      {"Unbekannte Signal-Nummer:", // 4
       "Unknown signal number:"},
    };
  }

  inline std::string signal2string(const int s, MessageHandling::Sprachen L = MessageHandling::English) {

    const MessageHandling::Messages<Implementation_pid2string::AnzahlSprachen> M(Implementation_pid2string::Meldungen_pid, L);

    switch (s) {
    case  SIGHUP : return "SIGHUP";
    case SIGINT : return "SIGINT";
    case SIGQUIT : return "SIGQUIT";
    case SIGILL : return "SIGILL";
    case SIGTRAP : return "SIGTRAP";
    case SIGABRT : return "SIGABRT";
    case SIGBUS : return "SIGBUS";
    case SIGFPE : return "SIGFPE";
    case SIGKILL : return "SIGKILL";
    case SIGUSR1 : return "SIGUSR1";
    case SIGSEGV : return "SIGSEGV";
    case SIGUSR2 : return "SIGUSR2";
    case SIGPIPE : return "SIGPIPE";
    case SIGALRM : return "SIGALRM";
    case SIGTERM : return "SIGTERM";
    case SIGCHLD : return "SIGCHLD;";
    case SIGCONT : return "SIGCONT";
    case SIGSTOP : return "SIGSTOP";
    case SIGTSTP : return "SIGTSTP";
    case SIGTTIN : return "SIGTTIN";
    case SIGTTOU : return "SIGTTOU";
    case SIGURG : return "SIGURG";
    case SIGXCPU : return "SIGXCPU";
    case SIGXFSZ : return "SIGXFSZ";
    case SIGVTALRM : return "SIGVTALRM";
    case SIGPROF : return "SIGPROF";
    case SIGWINCH : return "SIGWINCH";
    case SIGIO : return "SIGIO";
    case SIGPWR : return "SIGPWR";
    case SIGSYS : return "SIGSYS";
    default : return M(4);
    }
  }

  std::string pid2string(PidType pt, MessageHandling::Sprachen L = MessageHandling::English) {

    const MessageHandling::Messages<Implementation_pid2string::AnzahlSprachen> M(Implementation_pid2string::Meldungen_pid, L);

    std::ostringstream os;
    if (pt.normal())
      os << M(0) << pt.status();
    else if (pt.aborted())
      os << M(1) << signal2string(pt.a_signal());
    else if (pt.stopped())
      os << M(2) << signal2string(pt.s_signal());
    else
      os << M(3);
    return os.str();
  }



  const std::vector<std::string> Empty;

  class System {

  public :

    System() :
      use_core(false), use_file(false), use_time(false), use_n_files(false), use_v_mem(false) {}

    template <typename StringIt>
    PidType operator() (const std::string& command, const StringIt begin, const StringIt end, const std::string& in_file, const std::string& out_file, const std::string& err_file, const std::vector<std::string>& Env = Empty, const std::string& new_working_dir = "") {
      // Output files are truncated

      using namespace std;

      StringHandling::Vector_c_str Parameters;
      {
	const typename StringIt::difference_type count = distance(begin, end);
	Parameters.v.reserve(count+2);
	Parameters.push_back(base_name(command));
	for (StringIt i = begin; i != end; ++i)
	  Parameters.push_back(*i);
	Parameters.v.push_back(0);
      }

      PidType Pid;
      int communication[2];

      if (pipe(communication) != 0)
	throw system_error("pipe = -1");
      const pid_t process_ident = fork();
      if (process_ident == 0) { // child process

	if (new_working_dir != "" and chdir(new_working_dir.c_str())) {
	  unsigned char c = chdir_new_directory_error;
	  write(communication[1], &c, 1);
	  exit(c);
	}
	
	close(communication[0]);

	if (not in_file.empty())
	{
	  int in_file_desc = open(in_file.c_str(), O_RDONLY);
	  if (in_file_desc == -1) {
	    unsigned char c = open_in_file_error;
	    write(communication[1], &c, 1);
	    exit(c);
	  }
	  if (in_file_desc != STDIN_FILENO) {
	    if (dup2(in_file_desc, STDIN_FILENO) != STDIN_FILENO) {
	      unsigned char c = dup_in_file_error;
	      write(communication[1], &c, 1);
	      exit(c);
	    }
	    close(in_file_desc);
	  }
	}
	{
	  int out_file_desc = open(out_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	  if (out_file_desc == -1) {
	    unsigned char c = open_out_file_error;
	    write(communication[1], &c, 1); 
	    exit(c);
	  }
	  if (out_file_desc != STDOUT_FILENO) {
	    if (dup2(out_file_desc, STDOUT_FILENO) != STDOUT_FILENO) {
	      unsigned char c = dup_out_file_error;
	      write(communication[1], &c, 1);
	      exit(c);
	    }
	    close(out_file_desc);
	  }
	}
	{
	  int err_file_desc = open(err_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	  if (err_file_desc == -1) {
	    unsigned char c = open_err_file_error;
	    write(communication[1], &c, 1); 
	    exit(c);
	  }
	  if (err_file_desc != STDERR_FILENO) {
	    if (dup2(err_file_desc, STDERR_FILENO) != STDERR_FILENO) {
	      unsigned char c = dup_err_file_error;
	      write(communication[1], &c, 1);
	      exit(c);
	    }
	    close(err_file_desc);
	  }
	}
	{
	  unsigned char c = no_error;
	  write(communication[1], &c, 1);
	}

	if (use_core and not set_limit(RLIMIT_CORE, core))
	  throw system_error("can not set limit core = " + StringHandling::toString(core));
	if (use_file and not set_limit(RLIMIT_FSIZE, file))
	  throw system_error("can not set limit file size = " + StringHandling::toString(file));
	if (use_time and not set_limit(RLIMIT_CPU, time))
	  throw system_error("can not set limit cpu time = " + StringHandling::toString(time));
	if (use_n_files and not set_limit(RLIMIT_NOFILE, n_files))
	  throw system_error("can not set limit file number = " + StringHandling::toString(n_files));
	if (use_v_mem and not set_limit(RLIMIT_AS, v_mem))
	  throw system_error("can not set limit total memory = " + StringHandling::toString(v_mem));

	if (not Env.empty()) {
	  if (clearenv())
	    throw system_error("can not clear environment");
	  for (vector<string>::const_iterator i = Env.begin(); i != Env.end(); ++i)
	    if (putenv(const_cast<char*>(i -> c_str())))
	      throw system_error("can not put environment variable");
	}
	execvp(command.c_str(), &(Parameters.v[0]));
	{
	  unsigned char c = no_execvp;
	  write(communication[1], &c, 1);
	  close(communication[1]);
	  exit(c);
	}
      }
      else if( process_ident > 0) { // parent process
	close(communication[1]);
	pid_t return_id = waitpid(process_ident, &Pid.s, 0);
	{
	  unsigned char c[2];
	  read(communication[0], &c, 2);
	  if (c[0] == 0 and c[1] == no_execvp)
	    throw system_error("child, execvp = -1");
	  else switch (c[0]) {
	  case open_in_file_error :
	    throw open_file("child, in: " + in_file);
	  case dup_in_file_error :
	    throw system_error("child, duplicate in-pipe");
	  case open_out_file_error :
	    throw open_file("child, out: " + out_file);
	  case dup_out_file_error :
	    throw system_error("child, duplicate out-pipe");
	  case open_err_file_error :
	    throw open_file("child, err: " + err_file);
	  case dup_err_file_error :
	    throw system_error("child, duplicate err-pipe");
	  case chdir_new_directory_error :
	    throw system_error("child, cd " + chdir_new_directory_error);
	  }
	}
	close(communication[0]);
      }
      else { // error
	throw system_error("fork = -1");
      }

      return Pid;
    }

    void set_core(unsigned int c) { use_core = true; core = c; }
    void unset_core() { use_core = false; }
    void set_file(unsigned int f) { use_file = true; file = f; }
    void unset_file() { use_file = false; }
    void set_time(unsigned int t) { use_time = true; time = t; }
    void unset_time() { use_time = false; }
    void set_n_files(unsigned int nf) { use_n_files = true; n_files = nf; }
    void unset_n_files() { use_n_files = false; }
    void set_v_mem(unsigned int v) { use_v_mem = true; v_mem = v; }
    void unset_v_mem() { use_v_mem = false; }

  private :

    enum error_codes { no_error = 0, no_execvp = 200, open_in_file_error = 201, dup_in_file_error = 202, open_out_file_error = 203, dup_out_file_error = 204, open_err_file_error = 205, dup_err_file_error = 206, chdir_new_directory_error = 207 };

    unsigned int core; bool use_core; // in bytes
    unsigned int file; bool use_file; // in bytes
    unsigned int time; bool use_time; // in seconds
    unsigned int n_files; bool use_n_files; // a number
    unsigned int v_mem; bool use_v_mem; // in bytes

    bool set_limit(__rlimit_resource r, rlim_t l) {
      // instead of __rlimit_resource it should be int, but this seems to
      // be an error in Gnu C
      rlimit limit_p =  {l, l};
      return not setrlimit(r, &limit_p);
    }
  };

}



#endif

