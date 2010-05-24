// Matthew Gwynne 24.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LogicalDataAnalysis/plans/Marx2008.hpp
  \brief Plans regarding investigations of a case-study for the logical analysis of data

  The underlying article is [Limits to non-state market regulation: A 
  qualitative comparative analysis of the international sport footwear 
  industry and the Fair Labor Association, Regulation and Governance , 2008].

  
  \todo Create milestones

  
  \todo Data
  <ul>
   <li> The paper tries to determine the causes for a range of
   multinational footwear companies joining the Fair Labor Association.
   </li>
   <li> The underlying QCA truth table data given in the paper is
   summarised in the following combinatorial matrix.
   \verbatim
Marx_CM : mrc2ocom(matrix(
 [0,1,1,1,1],
 [1,1,1,1,1],
 [1,1,1,1,1],
 [0,1,1,1,1],
 [0,1,0,1,0],
 [1,0,0,0,0],
 [1,1,1,0,0],
 [1,0,0,0,0],
 [undef,0,0,0,0],
 [1,0,1,0,0],
 [0,0,1,0,0],
 [1,1,1,1,1],
 [0,0,0,0,0],
 [1,0,0,1,0],
 [1,0,0,0,0],
 [1,0,1,1,0],
 [0,1,1,0,1]),
 ["Nike","Adidas", "Puma", "Reebok", "New balance", "Diadora", "Fila", "Karhu", "Kelme", "Mizuno", "Saucony", "Asics", "Brooks", "Decathlon", "Lotto", "Kappa", "Umbro"],
 ["Union", "NGO", "Public", "Change", "FLA"])$
   \endverbatim
   </li>
   <li> Note here that the variables have the following (to be elaborated)
   definitions
    <ol>
     <li> "Union" - represents the presence or absence of "institutional 
     embeddedness", such as the presence of strong union forces within the 
     company. </li>
     <li> "NGO" - represents the presence or absence of "non-governmental
     organisation" pressure on the company. </li>
     <li> "Public" - represents whether the company is publicly traded
     or not. </li>
     <li> "Change" - represents whether the company has previously
     started to change prior to joining the FLA, introducing codes of
     conduct and review etc. </li>
    </ol>
   </li>
   <li> Also note that for "Kelme", "Union" is undefined, and the value
   is not given in the paper. This seems be to be due to the fact that
   there is no available data for Spain (the country in which Kelme
   operates). How to treat such "unknown" conditions in our system? </li>
  </ul>

*/

