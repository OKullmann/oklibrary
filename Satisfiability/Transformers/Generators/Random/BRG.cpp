// Oliver Kullmann, 22.6.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

USAGE:

> ./BRG -v | --version

for information on the program, the version, and the environment.

> ./BRG -h | --help

for basic help-information.

> ./BRG [clauses] [options] [seeds] [output]

[output] format : [+- prefix]NAME[x,y suffix]

for creation of random CNFS.

For the complete documentation, see
  docus/BRG.txt


TODOS:

1 Different verbosity levels, and handling of p-line
  - The following outlines proposed changes (to be discussed with AS, OZ).
  - Basically, using prefixes "-" or "--" to filenames to control output
    of comments and p-line.
  - Using "cout" instead of "-cout" as currently.
  - A prefix "-" for "cout" or "FILENAME" shall mean "no comments".
  - Additional one or two postfixes, separated by commas, for
    changing the p-line: first argument for n, second for c, with
    prefix "+" for addition, "-" for subtraction, and no prefix for
    replacement.
  - E.g. "-cout" shall mean without comments, while "-cout,+10" means
    also without comments, and additionally adding 10 to the n-value.
  - "--" shall mean without comments and without p-line.
  - Default filenames are not affected by such prefixes (they should be
    simple and stable).
  - This shall be a new standard for all such generators.
  - These options are not "observable", and thus are not encoded into the
    seeds.

2 Should all output-options influence the seeds?
  - The two main option-dimensions, "sorting" and "renaming", are
    "observable", and thus encoded.
  - Is this reasonable for the renaming-dimensions (with its three options)?
    Seems so, since the behaviour of SAT-solvers might depend on it.

3 For building the program one has to allow also building outside of
the context of the OKlibrary. Then the Git-id is just hardcoded.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <ProgramOptions/Environment.hpp>

#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.4",
        "26.8.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/BRG.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  

  using namespace RandGen;

  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    return default_filename(MainType::block_uniform_cnf, extract_parameters(par.vp), s);
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [clauses] [options] [seeds] [output]\n\n"
    "   clauses : \"B1; ...; Bs\", with s >= 0 clause-blocks Bi\n"
    "     Bi    : \"C * P1 | ... | Pm\", with m >= 1 clause-parts Pi and C=#clauses\n"
    "     Pi    : \"N, W [, P]\", where\n"
    "              N=variable-range, W=#literals, P=#sign-probability\n"
    "   options : \"c1, ..., cl\", with l >= 0 option-choices ci from\n"
    "               " << Environment::WRP<option_t>{} << ";\n"
    "             defaults are the first values for both options\n"
    "   seeds   : \"s1, ..., sp\", with p >= 0 seed-values si, which are\n"
    "             unsigned 64-bit integers, \"r\" (for \"random\"), or \"t\" (for \"timestamp\")\n"
    "   output  : \"-cout\" (standard output) or \"\"[-]\"\" (default filename) or \"FILENAME\"\n\n"
    " computes the random CNF:\n\n"
    "  - The arguments are positional, not named (the names are used here only"
    " for communication).\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Spaces are optional (if present, quotation is needed for arguments).\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
    "  - The optional \"-\" for the default-filename means \"don't print filename\" (which otherwise happens).\n"
;
    return true;
  }

  struct adjust_output_parameters {
    std::string out_channels = "";
    int var_num = -1;
    std::string var_sign = ""; // '', -, or +
    int cla_num = -1;
    std::string cla_sign = "";
    const int OUT_CHANNELS_NUM = 4;
    bool is_suffix() {
      return ((var_num > 0) && (cla_num > 0));
    }
    std::string to_str() {
      std::stringstream sstream;
      sstream << "out_channels: " << out_channels << std::endl <<
                 "var_sign: " << var_sign << std::endl <<
                 "var_num: " << var_num << std::endl <<
                 "cla_sign: " << cla_sign << std::endl <<
                 "cla_num: " << cla_num;
      return sstream.str();
    }
    int parse_prefix_suffix(std::string &output_par_s) {
      unsigned prefix_len = 0;
      while ((output_par_s[prefix_len] == '-') || (output_par_s[prefix_len] == '+'))
        prefix_len++;
      if (prefix_len) {
        out_channels = output_par_s.substr(0, prefix_len);
        output_par_s = output_par_s.substr(prefix_len, output_par_s.size() - out_channels.size());
      }
      if (output_par_s[output_par_s.size()-1] == ']') {
        std::size_t pos = output_par_s.find("[");
        if (pos == std::string::npos) {    
          std::cerr << error << "[ is not found while ] exsists in output " << output_par_s << "\"\n";
          return int(Error::except);
        }
        std::string output_par_suffix = output_par_s.substr(pos + 1, output_par_s.size() - pos - 2);
        output_par_s = output_par_s.substr(0, pos);
        pos = output_par_suffix.find(",");
        if (pos == std::string::npos) {
          std::cerr << error << ", does not exist in output suffix" << output_par_suffix << "\"\n";
          return int(Error::except);
        }
        std::string var_suf_s = output_par_suffix.substr(0,pos);
        std::string cla_suf_s = output_par_suffix.substr(pos + 1, output_par_suffix.size() - pos - 1);
        if ((var_suf_s.size() == 0) || (cla_suf_s.size() == 0)) {
          std::cerr << error << "incorrect output suffix" << output_par_suffix << "\"\n";
          return int(Error::except);
        }
        if ( (var_suf_s[0] == '-') || (var_suf_s[0] == '+') ) {
          var_sign = var_suf_s[0];
          var_suf_s.erase(0,1);
        }
        if ( (cla_suf_s[0] == '-') || (cla_suf_s[0] == '+') ) {
          cla_sign = cla_suf_s[0];
          cla_suf_s.erase(0,1);
        }
        std::istringstream(var_suf_s) >> var_num;
        std::istringstream(cla_suf_s) >> cla_num;
      }
      return 0;
    }

    void fill_channels(const bool is_default_filename) {
      // for the default output file the default-+- prefix is "++++", while otherwise it is "-+++"
      if (out_channels == "")
        out_channels = (is_default_filename) ? "+" : "-";
      int add_default_channels_num = OUT_CHANNELS_NUM - out_channels.size();
      if (add_default_channels_num > 0)
      for (int i = 0; i < add_default_channels_num; i++)
        out_channels += '+';
      assert(out_channels.size() == OUT_CHANNELS_NUM);
      // if the first channel symbol is +, then print the output message
      
      // If we have disabled the p-line in the prefix, but have the x,y-suffix, then the suffix re-enables the p-line
      out_channels[2] = is_suffix() ? out_channels[2] : '+';
    }
    int modify_n(const int n) {
      int mod_n = n;
      if (var_num > 0) {
        if (var_sign == "+")
          mod_n += var_num;
        else if (var_sign == "-")
          mod_n -= var_num;
        else if (var_sign == "")
          mod_n = var_num;
      }
      return mod_n;
    }
    int modify_c(const int c) {
      int mod_c = c;
      if (cla_num > 0) {
        if (cla_sign == "+")
          mod_c += cla_num;
        else if (cla_sign == "-")
          mod_c -= cla_num;
        else if (cla_sign == "")
          mod_c = cla_num;
      }
      return mod_c;
    }
  };

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

try {

  Environment::Index index;
  
  rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  if (not valid(vpar)) {
    std::cerr << error << "Logically invalid clauses-parameter \"" << argv[index-1] << "\"\n";
    return int(Error::invalid_clauses);
  }
  
  const GParam gpar = (argc <= index) ? GParam{} : GParam{Environment::translate<option_t>()(argv[index++], sep)};
  const Param par{gpar, std::move(vpar)};
  
  vec_eseed_t s = seeds(par);
  typedef vec_eseed_t::size_type evec_size_t;
  const evec_size_t esize_system = s.size();
  const evec_size_t esize_add = argc > 3 ? add_seeds(argv[index++], s) : 0;
  
  std::ofstream out;
  std::string filename;
  
  // read +- prefix
  std::string output_par_s = index == argc ? "cout" : argv[index];
  adjust_output_parameters adj_out_par;
  int err_adj_out_par = 0;
  if (output_par_s.size() > 0) {
    err_adj_out_par = adj_out_par.parse_prefix_suffix(output_par_s);
    if (err_adj_out_par) return err_adj_out_par;
  }
  
  filename = output_par_s;
  bool is_default_filename = false;

  if (filename == "cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
  }
  else {
    if (filename.empty()) {
      filename = default_filename(par, s);
      is_default_filename = true;
    }    
    assert(not filename.empty());
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }    
  }
  // set add default channels if they were not set manually
  adj_out_par.fill_channels(is_default_filename);
  
  if (adj_out_par.out_channels[0] == '+')
    std::cout << "Output to file \"" << filename << "\".\n";

  index++;
  index.deactivate();

  // if the second channel symbol is +, then write comments to output file
  if (adj_out_par.out_channels[1] == '+') {
    out << Environment::Wrap(proginfo, Environment::OP::dimacs);
    using Environment::DHW;
    using Environment::DWW;
    using Environment::qu;
    out << DHW{"Parameters"}
              << DWW{"command-line"};
    Environment::args_output(out, argc, argv);
    out << "\n"
              << DWW{"output"} << qu(filename) << "\n"
              << DWW{"options"} << gpar << "\n"
              << DWW{"output channels"} << adj_out_par.out_channels << "\n"
              << DWW{"num_clause-blocks"} << par.vp.size() << "\n"
              << DWW{" clause-blocks"} << par.vp << "\n"
              << DWW{"num_e-seeds"} << esize_system << "+" << esize_add << "=" << s.size() << "\n"
              << DWW{" e-seeds"};
    assert(not s.empty());
    out << s[0];
    for (vec_eseed_t::size_type i = 1; i < s.size(); ++i)
      out << " " << s[i];
    out << "\n";
    
  }

  RandGen_t g(transform(s, SP::split));
  
  DimacsClauseList dimacs_data;
  if (gpar == GParam(-1)) 
    dimacs_data = rand_clauselist(g, par.vp).first;
  else
    dimacs_data = random(g,par).first;

  if (adj_out_par.out_channels[2] == '+') {
    // adjust p line if required
    dimacs_data.first.n = adj_out_par.modify_n(dimacs_data.first.n);
    dimacs_data.first.c = adj_out_par.modify_c(dimacs_data.first.c);
    out << dimacs_data.first; // write p line
  }
  if (adj_out_par.out_channels[3] == '+')
    out << dimacs_data.second; // write clauses
  
}
catch(const std::domain_error& e) {
    std::cerr << error << "Parameters\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::domain);
  }
catch(const std::invalid_argument& e) {
    std::cerr << error << "Parameters: could not perform numeric conversion\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::invalid);
  }
catch(const std::out_of_range& e) {
    std::cerr << error << "Parameters: outside the range of 64-bit unsigned integer\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::out_of_range);
  }
catch(const std::bad_alloc& e) {
    std::cerr << error << "Bad allocation\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::alloc);
  }
catch(const std::exception& e) {
    std::cerr << error << "Unexpected exception\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::except);
  }

}
