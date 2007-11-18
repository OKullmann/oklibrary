// Oliver Kullmann, 20.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/CrispCSP/plans/general.hpp
  \brief General plans for the module on "crisp CSP"


  \todo Direct translations
  <ul>
   <li> The CSP formats
    <ol>
     <li> http://cpai.ucc.ie/05/xml.html </li>
     <li> http://cpai.ucc.ie/05/table.html </li>
    </ol>
    are to be transformed into alliances of active P-clause-sets.
   </li>
   <li> Specialised versions are relevant of course; perhaps just one
   P-clause-set per constraint is also alright. </li>
   <li> "Mono-signed" full clauses per constraint are exactly enough. </li>
   <li> The benchmarks from http://cpai.ucc.ie/05/CallForSolvers.html should
   all be tried! </li>
  </ul>


  \todo Overview
  <ul>
   <li> To get an overview on what has been done, we should install
   under ExternalSources the available open source approaches. </li>
  </ul>


  \todo Preprocessing
  <ul>
   <li> The "relations" are just "monosigned" full clause-sets, to be used
   to specify the constraints (which each just specify a specific set
   of variables). </li>
   <li> So it should be worth using some preprocessing
   for the relations. </li>
   <li> We could use sat-equivalence-simplifications. </li>
   <li> And we should exploit that for us constraints can be arbitrary
   P-clause-sets. </li>
   <li> List of reductions:
    <ul>
     <li> resolution clause-reductions </li>
    </ul>
   </li>
  </ul>


  \todo Good representations
  <ul>
   <li> One should study the problem, which clause-sets are especially
   suitable for the representation of relations. </li>
   <li> If the relation is small, then for example all prime implicants can
   be computed. </li>
   <li> And all symmetries (but perhaps this doesn't help much?!). </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Transformers::CrispCSP
  \brief Module with components for transformers of crisp CSP problems into SAT

  Alias "CrC".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Transformers {
      namespace CrispCSP {
      }
      namespace CrC = CrispCSP;
    }
  }
}

