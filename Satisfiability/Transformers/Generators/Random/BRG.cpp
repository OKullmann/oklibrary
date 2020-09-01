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

for creation of random CNFS.

For the complete documentation, see
  docus/BRG.txt


TODOS:

0 As a first step towards Todo 1, the output-object "out", currently
  of type std::ofstream, should be put into a proper class, so that
  the code starting with the declaration of out, and finishing with "index++",
  is captured.
  - The constructor takes arguments argc, argv, index (via reference), plus
    the default-filename.
  - The object offers access to filename and out.

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

Extended plans (according to e-email from 27.8.2020):

There are four "channels":
1. the output-message
2. the comments
3. the p-line
4. the clauses.

In the case of (D)QCNF we have five of them:
4. the quantifier-prefix
5. the clauses.

When the output string starts with "+" or "-", then 4 resp. 5 leading
such symbols, if existing (if not, then their default-values are used),
are taken, switching on or off the channels (independently).

Additionally, if the final symbol of the output-string is "]", then it
is assumed there is a trailing "[x,y]" (if not it's a syntax-error),
where x,y are unsigned integers with a possible prefix "+" or "-": these
two numbers set resp. adjust the p-line.

While for the "+-"-prefix we only need the initial part (possibly
empty), for the "[]"-postfix we need to have both numbers.

If we switch off the clause-part, then computations are only done as
much as is needed to compute the other parts (that might be a bit
tricky, but should be doable).

So we have for the output part the syntax

 [+- prefix]NAME[x,y suffix]

where NAME can be empty (thus using the default name), "cout" (standard
output), or anything else.

For example if one wants to have just the p-line on standard output:

  --+-cout

If we want to have that into the default file:

  --+-

And if we want to have that into file "XYZ":

  --+-XYZ

If we want the default file, with 5 clauses added to the p-line:

  [+0,+5]

Or if we want to replace the values of the p-line (with default file):

  [123,456]

Note that for the default file the default-+- prefix is "++++", while
otherwise it is "-+++".

So if we want the default file, with 2 variable substracted, no clauses
added, and no message:

  -[-2,+0]


If we have disabled the p-line in the prefix, but have the x,y-suffix,
then the suffix re-enables the p-line (since the instruction is read
from left to right).

It seems we don't need to have the possibility of NAME="nil".


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
  if (index == argc or std::string_view(argv[index]) == "-cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = "-cout";
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = default_filename(par, s);
    assert(not filename.empty());
    const bool output_message = filename[0] != '-';
    if (not output_message) filename.erase(0,1);
    if (filename.empty()) filename = default_filename(par, s);
    assert(not filename.empty());
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }
    if (output_message)
      std::cout << "Output to file \"" << filename << "\".\n";
  }
  index++;

  index.deactivate();

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
            << DWW{"num_clause-blocks"} << par.vp.size() << "\n"
            << DWW{" clause-blocks"} << par.vp << "\n"
            << DWW{"num_e-seeds"} << esize_system << "+" << esize_add << "=" << s.size() << "\n"
            << DWW{" e-seeds"};
  assert(not s.empty());
  out << s[0];
  for (vec_eseed_t::size_type i = 1; i < s.size(); ++i)
    out << " " << s[i];
  out << "\n";

  RandGen_t g(transform(s, SP::split));

  if (gpar == GParam(-1)) rand_clauselist(out, g, par.vp);
  else out << random(g,par).first;

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
