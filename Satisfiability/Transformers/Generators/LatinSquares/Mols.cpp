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
        "0.1.1",
        "21.12.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Mols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " [N] [k] [output]\n"
    " computes the random CNF.\n"
    " Trailing arguments can be left out, using their default-values.\n"
    " The default-values are also activated by using \"\" for the argument,\n"
    "  except in case of output, where the default-value is activated by \"-cout\",\n"
    "  while \"\" means here the default output-filename.\n"
;
    return true;
  }


  typedef std::uint16_t dim_t;
  typedef std::uint64_t var_t;

  constexpr dim_t N_default = 6;
  constexpr dim_t k_default = 2;

  struct Param {
    dim_t N;
    dim_t k;
  };
  std::ostream& operator <<(std::ostream& out, const Param p) {
    return out << p.N << "," << p.k;
  }

  struct NumVars {
    var_t nls, nes, n;
  };
  std::optional<NumVars> numvars(const Param p) noexcept {
    const FloatingPoint::float80 N = p.N;
    const auto N3 = N*N*N;
    const auto nls = N3 * p.k;
    const auto nes = N3 * N * FloatingPoint::binomial_coeff(p.k, 2);
    const auto n = nls + nes;
    if (n >= FloatingPoint::P264) return {};
    else return NumVars{var_t(nls), var_t(nes), var_t(n)};
  }

  struct Encoding {
    const Param p;
    const NumVars nv;

    using float80 = FloatingPoint::float80;

    Encoding(const Param p, const NumVars nv) noexcept : p(p), nv(nv) {}
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


  enum class Error {
    conversion = 11,
    too_big = 12,
    too_small = 13,
    file_open = 14,
  };


  dim_t read_dim(const std::string arg) {
    unsigned long d;
    try { d = std::stoul(arg); }
        catch (const std::invalid_argument& e) {
      std::cerr << error << "The argument \"" << arg << "\" is not a valid integer.\n";
      std::exit(int(Error::conversion));
    }
    catch (const std::out_of_range& e) {
      std::cerr << error << "The argument \"" << arg << "\" is too big for unsigned long.\n";
      std::exit(int(Error::too_big));
    }
    if (d == 0) {
      std::cerr << error << "The argument is 0.\n";
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

  const std::optional<NumVars> onv = numvars(p);
  if (not onv) {
    std::cerr << error << "Parameters " << p << " too big.\n";
    return int(Error::too_big);
  }
  const NumVars nv = onv.value();
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
