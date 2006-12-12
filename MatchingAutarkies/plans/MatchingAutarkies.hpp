// Oliver Kullmann, 11.1.2006 (Swansea)

/*!
  \file MatchingAutarkies/plans/MatchingAutarkies.hpp
  \brief Algorithms regarding matching autarkies

  \todo Literature review:
  
  Regarding the lean kernel: In the paper {Kullmann 2003 (DAM)]
  the algorithm in Lemma 8.6 reduces it to the
  decision problem, for which one can use Theorem 7.5 or Lemma 7.11;
  Lemma 7.11 gives also directly the matching lean kernel in Corollary 7.12.

  Likely these algorithms are not most efficient (at least in the simple form
  in which they are stated). One should exploit matching algorithms more directly.
  Such algorithms are implicitly contained in the joined paper with Szeider/Fleischner.

  \todo Algorithms:
  How to efficiently find matching autarkies and computing
  the lean kernel (pseudo-code)?

  \todo Implementation:
  A direct implementation of the computation
  of the matching lean kernel based on a matching algorithm.

  \todo Application:
  Write an application for computing the lean kernel,
  the largest matching autarky etc.

  \todo Incremental:
  Then one has to think about integration into a SAT solver.
*/

/*!
  \namespace OKlib::MatchingAutarkies
  \brief Components related to finding matching autarkies
*/

namespace OKlib {
  namespace MatchingAutarkies {
  }
}

