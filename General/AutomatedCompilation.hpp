// Oliver Kullmann, 17.11.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef AUTOMATEDCOMPILATIONWAECHTER

#define AUTOMATEDCOMPILATIONWAECHTER

#include <string>
#include <vector>

// #include <iostream> // DEBUG

#include <OKlib/General/ErrorHandling.hpp>
#include <OKlib/General/StreamHandling.hpp>
#include <OKlib/General/SystemHandling.hpp>


namespace AutomatedCompilation {

  // Error classes

  struct Error : ErrorHandling::Error {
    Error(const std::string& what) : ErrorHandling::Error(what) {}
  };

}


namespace AutomatedCompilation {

  // Policy classes to implement specific compilers
  // -----------------------------------------------------

  typedef std::vector<std::string> Parameter_list;

  class Compiler_default {
  protected :
    ~Compiler_default() {}
    std::string path_compiler_executable() const;
    template <class Executable >
    void set_parameter(Parameter_list& P, const Executable& exe);
  };
  

  class gpc_basic : protected Compiler_default {
  protected :
    ~gpc_basic() {}
    std::string path_compiler_executable() const { return "/usr/bin/gpc"; }
  };

  class gpc_delphi_new : protected gpc_basic {
    // uses a new name for the executable
    // "delphi": Mit den Typen cardinal und string (und auch #include)
  protected :
    ~gpc_delphi_new() {}
    template <class Executable >
    void set_parameter(Parameter_list& P, const Executable& exe) {
      P.push_back("--delphi");
      P.push_back("-o"); P.push_back(exe.get_executable_directory() + "/" + exe.get_basename());
    }
  };

  class gpc_delphi_transfer : protected gpc_basic {
    // transfers the name for the executable from the source file
    // "delphi": Mit den Typen cardinal und string (und auch #include)
  protected :
    ~gpc_delphi_transfer() {}
    template <class Executable >
    void set_parameter(Parameter_list& P, const Executable& exe) {
      P.push_back("--delphi");
      P.push_back("--executable-file-name");
      P.push_back("--executable-path" + exe.get_executable_directory());
    }
  };


  class dcc : protected Compiler_default {
    // the executable gets its base name from the source file
    // Attention: It must be possible to create a temporary file ProgramName.drf in the working directory, where ProgramName is the name of the program.
  protected :
    ~dcc() {}
    bool has_transfer_name() const { return true; }
    
    std::string path_compiler_executable() const { return "/usr/X11R6/bin/dcc"; }
    
    template <class Executable >
    void set_parameter(Parameter_list& P, const Executable& exe) {
      P.push_back("-E" + exe.get_executable_directory());
    }
    
  };
}


namespace AutomatedCompilation {

  // Policy classes to set the environment
  // ---------------------------------------

  class Environment_basis {
  protected :
    ~Environment_basis() {}
    Parameter_list Env;
  };

  struct Environment_wwwrun : protected Environment_basis {
  protected :
    ~Environment_wwwrun() {}
    Environment_wwwrun() { Env.push_back("HOME=/var/lib/wwwrun"); }
  };

  struct Environment_kullmann : protected Environment_basis {
  protected :
    ~Environment_kullmann() {}
    Environment_kullmann() { Env.push_back("HOME=/home/kullmann"); }
  };

  

}
    

namespace AutomatedCompilation {

  // Policy classes to implement different handling of compiler messages
  // --------------------------------------------------------------------

  class ignore_output {
  protected :
    ~ignore_output() {}
    std::string std_output_file() const { return "/dev/null"; }
    std::string err_output_file() const { return "/dev/null"; }
  };

  class both_outputs {
  public :
    std::string get_std_output() const {
      return StreamHandling::file2string(StdOutName);
    }
    std::string get_err_output() const {
      return StreamHandling::file2string(StdErrName);
    }
    
  protected :
    ~both_outputs() {}
    both_outputs()
      : StdOut("both_outputs"), ErrOut("both_outputs"),
	StdOutName(StdOut.name()), StdErrName(ErrOut.name()) {}
    std::string std_output_file() const { return StdOutName; }
    std::string err_output_file() const { return StdErrName; }
    
  private :
    StreamHandling::TemporaryFile_basename_template StdOut;
    StreamHandling::TemporaryFile_basename_template ErrOut;
    const std::string StdOutName;
    const std::string StdErrName;
    // ACHTUNG: Verwendet man Referenzen fuer die beiden letzten Zeichenketten, so gibt es alle moeglichen Fehler, die ich mir nicht erklaeren kann! (g++ Version "gcc version 3.0.4 (SuSE)").
  };

}

namespace AutomatedCompilation {

  // Policy classes to implement different constructions of the source code file
  // ------------------------------------------------------------------------

  class unchanged_source_file {
  public :
    typedef std::string Source_file_data;
  protected :
    ~unchanged_source_file() {}
    unchanged_source_file(const Source_file_data& data) :
    source(data) {}

    std::string get_source_file_name() const { return source; }

  private :
    const std::string& source;
  };

  enum Inclusion_methods { true_inclusion, simulated_inclusion};

  template <int inclusion_method>
  class include_into_source {
    // throws exception in case inclusion could not happen

  public :
    struct Source_file_data {
      Source_file_data(const std::string& source_file_name, const std::string& include_file_name, const std::string& new_source_name, const std::string& marker_string):
	source(source_file_name), include( include_file_name), new_name(new_source_name), marker(marker_string) {}
      const std::string& source, include, new_name, marker;
    };
    
  protected :
    ~include_into_source() {}
    include_into_source(const Source_file_data& source_data) :
      data(source_data), new_source(data.new_name)
    {
      new_source.open();
      perform_inclusion();
      new_source.f().close();
    }

    std::string get_source_file_name() const { return data.new_name; }

  private :
    const Source_file_data& data;
    StreamHandling::TemporaryFile_self_named new_source;

    include_into_source(const include_into_source&);
    include_into_source& operator = (const include_into_source&);

    void perform_inclusion();
  };

  template<>
  inline void include_into_source<true_inclusion>::perform_inclusion() {
    std::string source_code = StreamHandling::file2string(data.source);
    StringHandling::replace_substring_throw(source_code, data.marker, data.include);
    new_source << source_code;
    }

  template<>
  inline void include_into_source<simulated_inclusion>::perform_inclusion() {
    std::string source_code = StreamHandling::file2string(data.source);
    std::string include_code = StreamHandling::file2string(data.include);
    StringHandling::replace_substring_throw(source_code, data.marker, include_code);
    new_source << source_code;
  }
}


namespace AutomatedCompilation {

  // Policy classes to implement different naming conventions for the executable file
  // -------------------------------------------------------------------------


  class Naming_conventions {
  public :
    typedef std::string Executable_file_data;
  protected :
    ~Naming_conventions() {};
    std::string get_executable_directory() const;
   };

  class transfer_source_name : public Naming_conventions {
  public :
    ~transfer_source_name() {}
    std::string get_executable_directory() const { return exe_dir; }
  protected:
    transfer_source_name(const Executable_file_data& executable_directory) :
      exe_dir(executable_directory) {}
  private :
    const Executable_file_data& exe_dir;
  };

  class new_executable_name : public Naming_conventions {

  public :
    struct Executable_file_data {
      Executable_file_data(const std::string& executable_directory, const std::string& executable_basename) :
      exe_dir(executable_directory), basename(executable_basename) {}
      const std::string& exe_dir;
      const std::string& basename;
    };
    ~new_executable_name() {}
    std::string get_executable_directory() const { return data.exe_dir; }
    std::string get_basename() const { return data.basename; }

  protected:
    new_executable_name(const Executable_file_data& executable_data) :
      data(executable_data) {}

  private :
    const Executable_file_data& data;
  };


}



namespace AutomatedCompilation {

  // Class for performing compilation
  // -----------------------------------------------------


  template
  < class Compiler, class Environment, class Compiler_Output, class Source, class Executable >
  class Compilation : public Compiler, public Environment, public Compiler_Output, public Source, public Executable {

  public :
    Compilation(const typename Source::Source_file_data& source_data, const typename Executable::Executable_file_data& executable_data, const std::string& new_working_dir = "") :
      Compiler(), Compiler_Output(), Source(source_data), Executable(executable_data) {
      Parameter_list Par;
      set_parameter(Par, static_cast<Executable>(*this));
      Par.push_back(Source::get_source_file_name());
      Pid = SystemHandling::System()(Executable::path_compiler_executable(), Par.begin(), Par.end(), "/dev/null", Compiler_Output::std_output_file(), Compiler_Output::err_output_file(), Environment::Env, new_working_dir);
    }

    operator SystemHandling::PidType() const { return Pid; }

  private :

    SystemHandling::PidType Pid;
  };

}

#endif


