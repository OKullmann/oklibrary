// Oliver Kullmann, 6.6.2007 (Swansea)

/*!
  \file Transitional/plans/Transitional.hpp
  \brief Plans for the Transitional-library which do not belong to a single module

  \todo Upgrade to gcc-4.2.0
  <ul>
   <li> Correct the template-errors:
    <ol>
     <li> Precise problem description: XXX </li>
     <li> Methods to overcome the problem: XXX </li>
     <li> Files affected: XXX </li>
    </ol>
   </li>
  </ul>

  \todo Super-modules:
  <ul>
   <li> Currently 82 modules --- that needs a superstructure.
   So we should introduce sub-directories of Transitional,
   corresponding to "super-modules". </li>
   <li> Problematic the correspondence between paths and namespaces:
    <ol>
     <li> We could be consistent, using full super-module names
     and incorporating them into the namespaces. </li>
     <li> Or we could make an exception, not incorporating
     the super-modules into the namespace-hierarchy. Perhaps
     we start the super-module-names with "_", and introduce the
     general rule that such directories do not participate in the
     namespace-hierarchy? </li>
     <li> Or we could incorporate them, but using 3-letter abbreviations. </li>
     <li> Possible super-modules:
      <ol>
       <li> SAT </li>
       <li> ACS active clause-sets </li>
       <li> GEN general tools </li>
       <li> GRA graphs </li>
       <li> HYP hypergraphs </li>
       <li> ALG algebra </li>
       <li> THE theory (like Turing machines etc.) </li>
       <li> TES test system </li>
       <li> BUI build system </li>
       <li> COS complexity system </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Are there refactoring tools? Or can we finally finish module Refactoring ?!? </li>
  </ul>

  \todo Concepts
  <ul>
   <li> Currently all concepts are collected in module Concepts. Perhaps every module
   better should get its own sub-module "Module/concepts" ?! </li>
  </ul>

*/

/*!
  \namespace OKlib
  \brief All components of the OKlibrary
*/

namespace OKlib {
}





