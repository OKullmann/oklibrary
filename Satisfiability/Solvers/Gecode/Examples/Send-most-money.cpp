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
#include <iomanip>

#include <cstdint>
#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <ProgramOptions/Environment.hpp>

#include "../Lookahead.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "1.3.7",
        "9.6.2021",
        __FILE__,
        "Christian Schulte, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Send-most-money.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef GC::IntVarArray IntVarArray;
  typedef LA::BrTypeO BrTypeO;
  typedef LA::BrSourceO BrSourceO;
  typedef LA::BrMeasureO BrMeasureO;
  typedef LA::option_t option_t;

  class SendMostMoney : public GC::Space {
    IntVarArray L;
    const BrTypeO brt;
    const BrSourceO brs;
    const BrMeasureO brm;

  public:
    SendMostMoney(const BrTypeO brt, const BrSourceO brs, const BrMeasureO brm) :
      L(*this, 8, 0, 9), brt(brt), brs(brs), brm(brm) {

      assert(valid(L));

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
      LA::post_branching<SendMostMoney>(*this, L, brt, brs, brm);
    }

    SendMostMoney(SendMostMoney& s) : GC::Space(s), brt(s.brt), brs(s.brs), brm(s.brm) {
      assert(valid(s.L));
      L.update(*this, s.L);
      assert(valid(L));
    }
    virtual GC::Space* copy(void) {
      return new SendMostMoney(*this);
    }

    bool valid () const noexcept { return valid(L); }
    static bool valid (const IntVarArray& L) noexcept { return L.size() == 8; }
    bool valid (const LA::size_t i) const noexcept { return i<LA::tr(L.size()); }

    GC::IntVar at(const LA::size_t i) const noexcept {
      assert(valid()); assert(valid(i));
      return L[i];
    }
    IntVarArray at() const noexcept { assert(valid()); return L; }

    BrTypeO branching_type() const noexcept { assert(valid()); return brt; }
    BrSourceO branching_source() const noexcept { assert(valid()); return brs; }
    BrMeasureO branching_measure() const noexcept { assert(valid()); return brm; }

    void print(void) const noexcept {
      assert(valid(L));
      std::cout << L << "\n";
    }
     void print(std::ostream& os) const noexcept {
      assert(valid(L));
      os << L << std::endl;
    }
  };

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (LA::show_usage(proginfo, argc, argv)) return 0;

  Environment::Index index;
  const option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], LA::sep);
  const BrTypeO brt = std::get<BrTypeO>(options);
  const BrSourceO brs = std::get<BrSourceO>(options);
  const BrMeasureO brm = std::get<BrMeasureO>(options);

  typedef std::shared_ptr<SendMostMoney> node_ptr;
  const node_ptr m(new SendMostMoney(brt, brs, brm));
  assert(m->valid());

  // Find and print all solutions:
  LA::SearchStat stat = LA::find_all_solutions<SendMostMoney>(m, true);
  stat.print();

  // Visualise via Gist:
  const std::string v = argc <= index ? "" : argv[index++];
  if (v == "-gist") LA::visualise<SendMostMoney>(m);

}
