// Oliver Kullmann, 12.1.2006 (Swansea)

/*!
  \file ComplexityTheory/plans/general.hpp
  \brief Plans for the module on complexity theoretical constructions


  \todo Update namespaces.


  \todo Grammars:
  First we need formal grammars for the important types of
  Turing machines (there should be out there some code).


  \todo Simulation:
  Then we should (directly) simulate Turing machines.


  \todo NP-completeness:
  Implement the construction of SAT problems from
  the NP-completeness proof of SAT (see for example
  [Complexity and Cryptography: An Introduction; John Talbot and
  Dominic Welsh; 2006], Theorem 3.10): Given a Turing machine M and
  natural numbers n and t, construct F_M(n,t) which is satisfiable iff
  there exists an input x of size n such that M accepts x, and this
  in at most t steps.


  \todo NP-completeness 2:
  Implement the construction of circuits from Turing machines
  as in [Complexity and Cryptography: An Introduction; John Talbot and
  Dominic Welsh; 2006], Theorem 4.20: Given an accepting TM M
  (with outputs 0,1) and n in NN, construct a circuit C_n which
  simulates M on inputs of size n. (Translating these circuits into
  SAT problems we get an example for the above SAT-NP-completeness
  construction. On the other hand, from the CNF's constructed there
  by circuit-reconstruction we should be able to get the circuits here!)

*/

/*!
  \namespace OKlib::ComplexityTheory
  \brief Components for constructions from complexity theory (especially
  those related to propositional logic)

  Mainly for educational purposes; but potentially also interesting
  as a provider of SAT problems (and generalised SAT problems).
*/

namespace OKlib {
  namespace ComplexityTheory {
  }
}


