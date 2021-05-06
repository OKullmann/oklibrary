/*
 *  Authors:
 *    Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *    Christian Schulte, 2008-2019
 *    Oliver Kullmann, 2021
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software, to deal in the software without restriction,
 *  including without limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of the software,
 *  and to permit persons to whom the software is furnished to do so, subject
 *  to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/* TODOS

1. Statistics on nodes
    - Additionally to the statistics offered by Gecode, we must have our own
      counting.
    - An assignment like nodes = stat.node makes no sense: nodes is what is
      counted, stat.node is what Gecode delivers -- we don't know the
      precise relation (and we need full control).
    - Also leaves = failed_nodes + solutions and inner_nodes = nodes - leaves
      makes no sense here: a main point of the statistics, as discussed,
      is to check the framework. So leaves, inner nodes etc. must be properly
      observed and counted.
    - Once a proper understanding has been established, asserts state the
      relations between the direct counts and the Gecode-numbers.
    - Precise definitions of all special node-types are needed (in relation
      to the tree, to be defined).
    - Perhaps there is a call-back for failed leaves with nogood-learning.
    - Improve placement of solutions-count.
    - In binary branching, stat.node - stat.fail - solutions (i.e. the
      number of inner nodes in the search tree) is equal to the number of
      copy() calls in the search engine, but for n-ary branching this is
      not the case.
    - The correctness of the search tree formed by the n-ary branching
      was checked by hand.

4. Add statistics on branching widths.
    - Use Statistics::BasicStats.

5. Develop an explicit representation of branching (as vectors of
   "partial assignments" (power-literals).
    - Can the current domain be explicitly represented?

6. Where is the call-back for actions when "starting" a node?
    - After all propagations have been done, before branching.

*/

#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

#include <cstdint>
#include <cassert>
#include <cstddef>

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <ProgramOptions/Environment.hpp>
#include <Lookahead.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "1.2.10",
        "5.5.2021",
        __FILE__,
        "Christian Schulte, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Send-more-money.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef std::uint64_t count_t;
  count_t inner_nodes = 0, leaves = 0, solutions = 0;

  class SendMoreMoney : public GC::Space {
    GC::IntVarArray L;

  public:
    SendMoreMoney() : L(*this, 8, 0, 9) {

      GC::IntVar
        s(L[0]), e(L[1]), n(L[2]), d(L[3]),
        m(L[4]), o(L[5]), r(L[6]), y(L[7]);

      // no leading zeros:
      GC::rel(*this, s, GC::IRT_NQ, 0);
      GC::rel(*this, m, GC::IRT_NQ, 0);

      // all letters distinct:
      GC::distinct(*this, L);

      // linear equation:
      GC::IntArgs c(4+4+5); GC::IntVarArgs x(4+4+5);
      c[0]=1000; c[1]=100; c[2]=10; c[3]=1;
      x[0]=s;    x[1]=e;   x[2]=n;  x[3]=d;
      c[4]=1000; c[5]=100; c[6]=10; c[7]=1;
      x[4]=m;    x[5]=o;   x[6]=r;  x[7]=e;
      c[8]=-10000; c[9]=-1000; c[10]=-100; c[11]=-10; c[12]=-1;
      x[8]=m;      x[9]=o;     x[10]=n;    x[11]=e;   x[12]=y;
      GC::linear(*this, c, x, GC::IRT_EQ, 0);

      // post branching:
      LA::post_narysizemin(*this, L);
    }

    SendMoreMoney(SendMoreMoney& s) : GC::Space(s) {
      L.update(*this, s.L);
    }

    virtual GC::Space* copy() {
      ++inner_nodes;
      return new SendMoreMoney(*this);
    }
    void print() const {
      ++solutions; // XXX
      std::cout << L << std::endl;
    }
    void print(std::ostream& os) const {
      os << L << std::endl;
    }
  };

  constexpr int def_width = 10;
  using std::setw;
  const auto w = setw(10);
}


int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (LA::show_usage(proginfo, argc, argv)) return 0;

  typedef std::unique_ptr<SendMoreMoney> node_ptr;
  const node_ptr m(new SendMoreMoney);
  GC::DFS<SendMoreMoney> e(m.get());
  // Do not count copy() (called to initialise a search engine):
  assert(inner_nodes > 0);
  --inner_nodes;

  while (const node_ptr s{e.next()}) s->print();

  const GC::Search::Statistics stat = e.statistics();
  // XXX
  /*if (not (inner_nodes == stat.node - stat.fail - solutions)) {
    std::cerr << "ERROR[" << proginfo.prg << "]: inner_nodes=" <<
      inner_nodes << ", stat.node-stat.fail-solutions=" <<
      stat.node - stat.fail - solutions << "\n";
    return 1;
  }*/
  std::cout << stat.node << w << inner_nodes << w << leaves << w
            << stat.fail << w << solutions << "\n";

  Environment::Index index;

  const std::string visual = argc <= index ? "" : argv[index++];

  // Visualise via Gist:
  if (visual == "-gist") {
    GC::Gist::Print<SendMoreMoney> p("Print solution");
    GC::Gist::Options o;
    o.inspect.click(&p);
    GC::Gist::dfs(m.get(),o);
  }

}
