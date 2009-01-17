// Oliver Kullmann, 23.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file RamseyTheory/plans/general.hpp
  \brief Plans for the module on %Ramsey theory (especially
  van der Waerden's problem)

  Here components are located on applying generalised SAT solving for
  solving Ramsey-type problems.


  \todo Update namespaces


  \todo %Ramsey theory
  <ul>
   <li> We need generators for all types of Ramsey-problems;
   for van der Waerden's problem we already started. </li>
   <li> Such translators should only produce a hypergraph-colouring problem
   (to be handled then by the general translations in module
   Hypergraphs/Colourings). </li>
   <li> However, for the "non-diagonal" forms we need to create non-boolean
   clause-sets. </li>
  </ul>


  \todo Transfer:
  <ul>
   <li> Transferring (and updating) generators for
    <ol>
     <li> OKsolver/Experimental/Transformationen/Ramsey </li>
     <li> OKsolver/Experimental/Transformationen/VanderWaerden </li>
    </ol>
   </li>
   <li> Especially  providing active clause-sets is very interesting. </li>
  </ul>

*/

/*!
  \namespace OKlib::RamseyTheory
  \brief Components regarding Ramsey theory
*/

namespace OKlib {
  namespace RamseyTheory {
  }
}

