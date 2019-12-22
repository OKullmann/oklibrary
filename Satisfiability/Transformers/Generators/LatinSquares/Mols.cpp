// Oliver Kullmann, 20.12.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <optional>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "22.12.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Mols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  enum class Error {
    conversion = 11,
    too_big = 12,
    too_small = 13,
    file_open = 14,
  };


  typedef std::uint16_t dim_t;
  typedef std::uint64_t var_t;

  constexpr dim_t N_default = 6;
  constexpr dim_t k_default = 2;


  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " [N=" << N_default << "] [k=" << k_default << "] [output=-cout]\n"
    " computes the SAT-translation.\n"
    " Trailing arguments can be left out, using their default-values.\n"
    " \"\" for the output means the default output-filename.\n"
;
    return true;
  }


  struct Param {
    dim_t N;
    dim_t k;
  };
  std::ostream& operator <<(std::ostream& out, const Param p) {
    return out << p.N << "," << p.k;
  }

  struct NumVars {
    var_t nbls, nls, nbes, nes, n;
  };
  NumVars numvars(const Param p) noexcept {
    const FloatingPoint::float80 N = p.N;
    const auto nbls = N*N*N;
    const auto nls = nbls * p.k;
    const auto nbes = nbls * N;
    const auto nes = nbes * FloatingPoint::binomial_coeff(p.k, 2);
    const auto n = nls + nes;
    if (n >= FloatingPoint::P264) {
      std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
      std::exit(int(Error::too_big));
    }
    else return NumVars{var_t(nbls), var_t(nls), var_t(nbes), var_t(nes), var_t(n)};
  }

  struct Encoding {
    const Param p;
    const NumVars& nv;

    Encoding(const Param p, const NumVars& nv) noexcept : p(p), nv(nv) {}
  };


  std::string default_filestem() {
    return "MOLS2SAT_BASIC";
  }
  std::string default_param(const Param p) {
    return std::to_string(p.N) + "_" + std::to_string(p.k);
  }
  std::string default_filesuffix() {
    return ".dimacs";
  }
  std::string default_filename(const Param p) {
    return default_filestem() + "_" + default_param(p) + default_filesuffix();
  }


  dim_t read_dim(const std::string arg) {
    unsigned long d;
    std::size_t converted;
    try { d = std::stoul(arg, &converted); }
    catch (const std::invalid_argument& e) {
      std::cerr << error << "The argument \"" << arg << "\" is not a valid integer.\n";
      std::exit(int(Error::conversion));
    }
    catch (const std::out_of_range& e) {
      std::cerr << error << "The argument \"" << arg << "\" is too big for unsigned long.\n";
      std::exit(int(Error::too_big));
    }
    if (converted != arg.size()) {
      std::cerr << error << "The argument \"" << arg << "\" contains trailing characters: \""
        << arg.substr(converted) << "\".\n";
      std::exit(int(Error::conversion));
    }
    if (d == 0) {
      std::cerr << error << "An argument is 0.\n";
      std::exit(int(Error::too_small));
    }
    const dim_t cd = d;
    if (cd != d) {
      std::cerr << error << "The argument \"" << arg << "\" is too big for dim_t.\n";
      std::exit(int(Error::too_big));
    }
    return cd;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  const dim_t N = argc <= index ? N_default : read_dim(argv[index++]);
  const dim_t k = argc <= index ? k_default : read_dim(argv[index++]);
  const Param p{N,k};

  std::ofstream out;
  std::string filename;
  if (index == argc or std::string_view(argv[index]) == "-cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = "-cout";
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = default_filename(p);
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }
    std::cout << "Output to file \"" << filename << "\".\n";
  }
  index++;

  index.deactivate();

  const NumVars nv = numvars(p);
  const Encoding enc(p, nv);


  out << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  out << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(out, argc, argv);
  out << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"k"} << k << "\n"
            << DWW{"output"} << qu(filename) << "\n"
      << DHW{"Sizes"}
            << DWW{"nls"} << nv.nls << "\n"
            << DWW{"nes"} << nv.nes << "\n"
            << DWW{"n"} << nv.n << "\n"
;


}
