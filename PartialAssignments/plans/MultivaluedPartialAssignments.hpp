// Oliver Kullmann, 28.11.2006 (Swansea)

/*!
  \file PartialAssignments/plans/MultivaluedPartialAssignments.hpp
  \brief Plans regarding set-valued partial assignments

  \todo Implementation OKlib::PartialAssignments::MultiPASS
   - Reflect on the concept as discussed in Concepts/plans/PartialAssignments.hpp,
     and update accordingly.
   - Use literals from module Literals.
   - Perhaps instead of the <Values, Var> there should be one template parameter Literal?
     Seems sensible.
   - The static assert should be easier to formulate (since such concept tests will be used
     at many places; is some macro already in OKlib?).

   \todo Test OKlib::PartialAssignments::MultiPASS

*/

