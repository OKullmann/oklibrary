/*
 *  Authors:
 *    Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *    Christian Schulte, 2008-2019
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

#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

#include <cstdint>
#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <ProgramOptions/Environment.hpp>
#include <Lookahead.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "1.1.3",
        "5.5.2021",
        __FILE__,
        "Christian Schulte, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Send-most-money.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef std::uint64_t count_t;
  count_t inner_nodes = 0, leaves = 0, solutions = 0;

  class SendMostMoney : public GC::Space {
    GC::IntVarArray L;

  public:
    SendMostMoney(void) : L(*this, 8, 0, 9) {

      GC::IntVar
        s(L[0]), e(L[1]), n(L[2]), d(L[3]),
        m(L[4]), o(L[5]), t(L[6]), y(L[7]);

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
      x[4]=m;    x[5]=o;   x[6]=s;  x[7]=t;
      c[8]=-10000; c[9]=-1000; c[10]=-100; c[11]=-10; c[12]=-1;
      x[8]=m;      x[9]=o;     x[10]=n;    x[11]=e;   x[12]=y;
      GC::linear(*this, c, x, GC::IRT_EQ, 0);

      // post branching:
      LA::post_narysizemin(*this, L);
    }

    SendMostMoney(SendMostMoney& s) : GC::Space(s) {
      L.update(*this, s.L);
    }

    virtual GC::Space* copy(void) {
       ++inner_nodes;
      return new SendMostMoney(*this);
    }

    void print(void) const {
      ++solutions;
      std::cout << L << "\n";
    }
  };

  constexpr int def_width = 10;
  using std::setw;
  const auto w = setw(10);
}

// main function
int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (LA::show_usage(proginfo, argc, argv)) return 0;

  typedef std::unique_ptr<SendMostMoney> node_ptr;
  const node_ptr m(new SendMostMoney);
  GC::DFS<SendMostMoney> e(m.get());
  assert(inner_nodes > 0);
  --inner_nodes;
  while (const node_ptr s{e.next()}) s->print();

  GC::Search::Statistics stat = e.statistics();
  // XXX
  //assert(inner_nodes == stat.node - stat.fail - solutions);

  std::cout << stat.node << w << inner_nodes << w << leaves << w
            << stat.fail << w << solutions << "\n";

  Environment::Index index;

  const std::string visual = argc <= index ? "" : argv[index++];

  // Visualise via Gist:
  if (visual == "-gist")
    GC::Gist::dfs(m.get());

}
