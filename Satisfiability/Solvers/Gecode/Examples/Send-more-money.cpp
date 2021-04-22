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

/* TODOS

0. DONE As discussed, rename this example to "Send ...".
    - Since we are substantially modifying this example.

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

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "1.1.0",
        "22.4.2021",
        __FILE__,
        "Christian Schulte, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Send-more-money.cpp",
        "GPL v3"};

  namespace GC = Gecode;

  typedef std::uint64_t count_t;
  count_t inner_nodes = 0, leaves = 0, solutions = 0;


  class SizeMin : public GC::Brancher {
  protected:
    GC::ViewArray<GC::Int::IntView> x;
    mutable unsigned start;
    class PosVal : public GC::Choice {
    public:
      unsigned pos; unsigned val;
      PosVal(const SizeMin& b, unsigned p, unsigned v)
        : Choice(b,2), pos(p), val(v) {}
      virtual void archive(GC::Archive& e) const {
        Choice::archive(e);
        e << pos << val;
      }
    };
  public:
    SizeMin(GC::Home home, GC::ViewArray<GC::Int::IntView>& x0)
      : Brancher(home), x(x0), start(0) {}
    static void post(GC::Home home, GC::ViewArray<GC::Int::IntView>& x) {
      (void) new (home) SizeMin(home,x);
    }
    virtual size_t dispose(GC::Space& home) {
      (void) Brancher::dispose(home);
      return sizeof(*this);
    }
    SizeMin(GC::Space& home, SizeMin& b)
      : Brancher(home,b), start(b.start) {
      x.update(home,b.x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) SizeMin(home,*this);
    }
    virtual bool status(const GC::Space&) const {
      for (unsigned i = start; i < (unsigned)x.size(); ++i)
        if (!x[i].assigned()) {
          start = i; return true;
        }
      return false;
    }
    virtual GC::Choice* choice(GC::Space&) {
      unsigned p = start;
      unsigned s = x[p].size();
      for (unsigned i = start + 1; i < (unsigned)x.size(); ++i)
        if (!x[i].assigned() && (x[i].size() < s)) {
          p = i; s = x[p].size();
        }
      return new PosVal(*this,p,x[p].min());
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive& e) {
      unsigned pos, val;
      e >> pos >> val;
      return new PosVal(*this, pos, val);
    }
    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  unsigned a) {
      const PosVal& pv = static_cast<const PosVal&>(c);
      unsigned pos = pv.pos, val = pv.val;
      if (a == 0) return GC::me_failed(x[pos].eq(home,(int)val)) ?
                         GC::ES_FAILED : GC::ES_OK;
      else return GC::me_failed(x[pos].nq(home,(int)val)) ?
                  GC::ES_FAILED : GC::ES_OK;
    }
    virtual void print(const GC::Space&, const GC::Choice& c, unsigned a,
                       std::ostream& o) const {
      const PosVal& pv = static_cast<const PosVal&>(c);
      unsigned pos=pv.pos, val=pv.val;
      if (a == 0) o << "x[" << pos << "] = " << val;
      else o << "x[" << pos << "] != " << val;
    }
  };

  void sizemin(GC::Home home, const GC::IntVarArgs& x) {
    if (home.failed()) return;
    GC::ViewArray<GC::Int::IntView> y(home,x);
    SizeMin::post(home,y);
  }

  class SendMoreMoney : public GC::Space {
  protected:
    GC::IntVarArray l;

  public:
    SendMoreMoney() : l(*this, 8, 0, 9) {
      GC::IntVar s(l[0]), e(l[1]), n(l[2]), d(l[3]),
        m(l[4]), o(l[5]), r(l[6]), y(l[7]);

      // no leading zeros:
      GC::rel(*this, s, GC::IRT_NQ, 0);
      GC::rel(*this, m, GC::IRT_NQ, 0);

      // all letters distinct:
      GC::distinct(*this, l);

      // linear equation:
      GC::IntArgs c(4+4+5); GC::IntVarArgs x(4+4+5);
      c[0]=1000; c[1]=100; c[2]=10; c[3]=1;
      x[0]=s;    x[1]=e;   x[2]=n;  x[3]=d;
      c[4]=1000; c[5]=100; c[6]=10; c[7]=1;
      x[4]=m;    x[5]=o;   x[6]=r;  x[7]=e;
      c[8]=-10000; c[9]=-1000; c[10]=-100; c[11]=-10; c[12]=-1;
      x[8]=m;      x[9]=o;     x[10]=n;    x[11]=e;   x[12]=y;
      GC::linear(*this, c, x, GC::IRT_EQ, 0);

      // post branching
      sizemin(*this, l);
    }

    SendMoreMoney(SendMoreMoney& s) : GC::Space(s) {
      l.update(*this, s.l);
    }
    virtual GC::Space* copy() {
      ++inner_nodes;
      return new SendMoreMoney(*this);
    }
    void print() const {
      ++solutions; // XXX
      std::cout << l << "\n";
    }
  };

  constexpr int def_width = 10;
  using std::setw;
  const auto w = setw(10);
}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  typedef std::unique_ptr<SendMoreMoney> node_ptr;
  const node_ptr m(new SendMoreMoney);
  GC::DFS<SendMoreMoney> e(m.get());
  // Do not count copy() called to initialise a search engine:
  assert(inner_nodes > 0);
  --inner_nodes;
  while (const node_ptr s{e.next()}) s->print();

  GC::Search::Statistics stat = e.statistics();
  assert(inner_nodes == stat.node - stat.fail - solutions);

  std::cout << stat.node << w << inner_nodes << w << leaves << w
            << solutions << "\n";
}
