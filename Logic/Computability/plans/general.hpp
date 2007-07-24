// Oliver Kullmann, 9.7.2007 (Swansea)

/*!
  \file Logic/Computability/plans/general.hpp
  \brief General plans regarding computability issues

  Here special computational models are provided, such that from implementations
  in these models (generalised) satisfiability problems can be derived.


  \todo Update namespaces.

  
  \todo Turing machines
  <ul>
   <li> First we need formal grammars for the important types of
   Turing machines (there should be out there some code). </li>
   <li> Then we should (directly) simulate Turing machines. </li>
  </ul>


  \todo Circuits
  <ul>
   <li> Circuits and uniform circuits as computational models
   are especially important for the library. </li>
   <li> General transformations from Turing machines to uniform
   circuits and back are needed. </li>
   <li> The specific uniform circuits for example from [Vollmer 1999]
   should be implemented. </li>
   <li> In the SAT community (see [Biere SAT 2007]) there are
   file formats for boolean circuits. </li>
   <li> We need also all kinds of different types of circuits (different bases,
   more than 2 values, etc.). </li>
  </ul>


  \todo What else?
  <ul>
   <li> Besides Turing machines and circuits, what else is
   there which is related to (generalised) satisfiability
   issues (and does not fall into the domain of module
   ComplexityTheory; see ComplexityTheory/plans/general.hpp) ? </li>
  </ul>

*/

