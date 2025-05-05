// Oliver Kullmann, 31.3.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a graph (undirected, in the general library-format),
  and outputting it in one of the AdjVecUInt-formats.

The input-format is "liberal adjacency lists", that is,
 - arbitrary strings are allowed as vertex-names
 - comments can appear in every line, starting with "#"
 - the lines are adjacency-lists of their first vertex (but not necessarily
   complete -- the adjacencies of the first vertex are the union of all
   the adjacency-lines with that vertex as first vertex)
 - edges/arcs can be repeated
 - empty lines are allowed.
The output is "full-adjacency-lists" (with sorted lists).

If the input is in Dimacs, then the sed-program
  sed 's/^c/#/;s/^p/# p/;s/^e //'
converts this to our format.
This is available as Dimacs2FullAdjList.sed .


TODOS:

0. Add complement-option for first commandline-argument:
    - A new enumeration-type: orig, comp, compwloops.

1. A more comprehensive approach to graph-output is needed,
   incorporating at least the format-options of
   BCC2SAT:
    - We need a handling of comments (+-com).
    - Parameters can always be included or left out (+-dp).


EXAMPLES:

Consider the undirected path of length 3:
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion und "" ""
# 3 2 1
a b
b a c
c b

Note that, as with format "Metis", every edge is listed twice (and that
"und" had to be stated (code 1) -- the default is "dir" (code 0)).
The number of vertices and edges is only stated as a comment.

Now with using indices:
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion und "" false
# 3 2 1
0 1
1 0 2
2 1

Note that they are 0-based.
The Dimacs-format uses 1-based indices, and lists every edge only once.
The chosen edges {u,w} are those with u<w, and the lists are lexicographically
sorted:
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion und dimacs ""
p edge 3 2
e 1 2
e 2 3

Finally the Metis-format is similar to our fulladjlist-format, but 1-based,
the first line (with number of vertices and edges/arcs) is necessary,
and the following lines (the adjacency list) lack the source-vertex:
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion und metis ""
3 2
2
1 3
2


The input can also be interpreted as directed path:
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion "" "" ""
# 3 2 0
a b
b c
c
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion "" "" false
# 3 2 0
0 1
1 2
2
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion "" dimacs ""
p arc 3 2
e 1 2
e 2 3
Bicliques> echo -en "a b\nb c\n" | ./GraphConversion "" metis ""
3 2
2
3

Note the (trailing) empty line here (vertex 3 has no outgoing arcs).

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "2.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GraphConversion.cpp",
        "GPL v3"};

  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " graph-type format-option with-names\n\n"
    " graph-type     : " << Environment::WRP<GT>{} << "\n"
    " format-option  : " << Environment::WRP<GrFo>{} << "\n"
    " with-names     : \"false\" or anything else (default \"true\")\n\n"
    " reads a graph from standard input, and outputs it to standard output:\n\n"
    " - Arguments \"\" (the empty string) yield the default-values.\n"
    " - Default-values for the options are the first possibilities given.\n"
    " - with-names is only relevant for graph-format " << GrFo::fulladjlist
      << ".\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error <<
      "Exactly three arguments (graph-type, format-opt, with-names)"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const auto opt_type = Environment::read<GT>(argv[1]);
  if (not opt_type) {
    std::cerr << error <<
      "Faulty type-argument \"" << argv[1] << "\".\n";
    return 1;
  }
  const GT type = opt_type.value();

  const auto opt_format = Environment::read<GrFo>(argv[2]);
  if (not opt_format) {
    std::cerr << error <<
      "Faulty format-argument \"" << argv[2] << "\".\n";
    return 1;
  }
  const GrFo format = opt_format.value();

  const bool with_names = std::string(argv[3]) != "false";

  std::cout << make_AdjVecUInt(std::cin, type, format, with_names);
}
