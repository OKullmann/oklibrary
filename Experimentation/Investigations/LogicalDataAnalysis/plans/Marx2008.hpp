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
   <li> It seems that in the analysis done by Marx that Kelme is considered
   then as two configurations. So we have
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

 [0,0,0,0,0],
 [1,0,0,0,0],

 [1,0,1,0,0],
 [0,0,1,0,0],
 [1,1,1,1,1],
 [0,0,0,0,0],
 [1,0,0,1,0],
 [1,0,0,0,0],
 [1,0,1,1,0],
 [0,1,1,0,1]),
 ["Nike","Adidas", "Puma", "Reebok", "New balance", "Diadora", "Fila", "Karhu", "Kelme1", "Kelme1", "Mizuno", "Saucony", "Asics", "Brooks", "Decathlon", "Lotto", "Kappa", "Umbro"],
 ["Union", "NGO", "Public", "Change", "FLA"])$
   \endverbatim
   </li>
  </ul>
 
  \todo Functional analysis
  <ul>
   <li> With this data, we have the following full DNFs and CNFs representing
   the various functional extensions of the data (for the unspecified 
   configurations / logical remainders)
   \verbatim
/* DNF assuming liberal extension */
Marx_LibDNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change")],
   [{-gv("Change"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")}]$
/* CNF assuming liberal extension */
Marx_LibCNF_FF:
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change")],
   [{-gv("Change"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),gv("NGO"),-gv("Public"),-gv("Union")}]]$
/* DNF assuming conservative extension */
Marx_ConDNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change")],
   [{gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("NGO"),gv("Public"),-gv("Union")}]]$
/* CNF assuming conservative extension */
Marx_ConCNF_FF :
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change")],
   [{-gv("Change"),-gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public"),gv("Union")}]]$
    \endverbatim
    Note that the data doesn't contain any conflicts.
   </li>
   <li> Where for each of the above we get the following 
   minimum representations
   \verbatim
/* Minimal representations of the liberal extensions */
all_minequiv_bvsr_sub_cs(Marx_LibDNF_FF[2],Marx_ConDNF_FF[2]);
  [{{-gv("Change"),gv("NGO"),-gv("Union")},
    {gv("Change"),gv("NGO"),gv("Public")}},
   {{gv("Change"),gv("NGO"),gv("Public")},
    {gv("NGO"),gv("Public"),-gv("Union")}},
   {{gv("Change"),gv("NGO"),gv("Union")},
    {gv("NGO"),gv("Public"),-gv("Union")}}]


all_minequiv_bvsr_sub_cs(Marx_LibCNF_FF[2],Marx_LibCNF_FF[2]);
  [{{-gv("Change"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("NGO")},
    {gv("Change"),-gv("Public"),-gv("Union")},
    {gv("NGO"),-gv("Union")}}]


/* Minimal representations of the conservative extensions */
all_minequiv_bvsr_sub_cs(Marx_ConDNF_FF[2],Marx_ConDNF_FF[2]);
  [{{gv("Change"),gv("NGO"),gv("Public")},
    {gv("NGO"),gv("Public"),-gv("Union")}}]

all_minequiv_bvsr_sub_cs(Marx_ConCNF_FF[2],Marx_LibCNF_FF[2]);
  [{{gv("Change"),-gv("Union")},{gv("NGO")},{gv("Public")}}]
   \endverbatim
   Note here that the conservative DNF and liberal CNF correspond
   to the analysis chosen by Marx in the paper.
   </li>
  </ul>


  \todo Relational analysis
  <ul>
   <li> Other than the basic DNFs and CNFs given in "Functional analysis",
   we also have the relational viewpoint, where we consider the output
   variable as a variable like any other, and then we consider all
   rows in the data table as true. </li>
   <li> The question is: what to consider false? </li>
   <li> If we assume nothing is false, then {{}} is the minimal DNF
   for any such truth table. </li>
   <li> One method for constructing a non-trivial relational
   model is to assume that the behaviour is functional on the rows in
   the truth table (otherwise we would have observed the contradictory
   rows). </li>
   <li> In this case we get
   \verbatim
Marx_RelLibDNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change"),gv("FLA")],
   [{-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),gv("Union")}]]$

Marx_RelLibCNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change"),gv("FLA")],
   [{-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")}]]$
 

Marx_RelConDNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change"),gv("FLA")],
   [{gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")}]]$
 
Marx_RelConCNF_FF : 
  [[gv("Union"),gv("NGO"),gv("Public"),gv("Change"),gv("FLA")],
   [{-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")}]]$
   \endverbatim
   with minimum representations
   \verbatim
/* Minimal representations of the liberal extensions */
all_minequiv_bvsr_sub_cs(Marx_RelLibDNF_FF[2],Marx_RelConDNF_FF[2]);
  [{{-gv("Change"),-gv("FLA"),gv("Union")},
    {-gv("Change"),gv("FLA"),gv("NGO"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public")},
    {-gv("FLA"),-gv("NGO")},{-gv("FLA"),-gv("Public")}},
   {{-gv("Change"),-gv("FLA"),gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public")},
    {-gv("FLA"),-gv("NGO")},{-gv("FLA"),-gv("Public")},
    {gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")}},
   {{-gv("Change"),-gv("FLA"),gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Union")},
    {-gv("FLA"),-gv("NGO")},{-gv("FLA"),-gv("Public")},
    {gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")}}]


all_minequiv_bvsr_sub_cs(Marx_RelLibCNF_FF[2],Marx_RelLibCNF_FF[2]);
  [{{-gv("Change"),-gv("FLA"),-gv("NGO"),gv("Public"),gv("Union")},
    {-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public")},
    {gv("Change"),-gv("FLA"),gv("NGO")},
    {gv("Change"),-gv("FLA"),-gv("Public"),-gv("Union")},
    {-gv("FLA"),gv("NGO"),-gv("Union")},
    {gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")}}]


/* Minimal representations of the conservative extensions */
all_minequiv_bvsr_sub_cs(Marx_RelConDNF_FF[2],Marx_RelConDNF_FF[2]);
  [{{-gv("Change"),-gv("FLA"),-gv("NGO")},
    {-gv("Change"),-gv("FLA"),gv("Public"),gv("Union")},
    {gv("Change"),-gv("FLA"),gv("NGO"),-gv("Public"),-gv("Union")},
    {gv("Change"),gv("FLA"),gv("NGO"),gv("Public")},
    {-gv("FLA"),-gv("NGO"),gv("Union")},
    {gv("FLA"),gv("NGO"),gv("Public"),-gv("Union")}}]


all_minequiv_bvsr_sub_cs(Marx_RelConCNF_FF[2],Marx_RelLibCNF_FF[2]);
  [{{-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public")},
    {gv("Change"),-gv("FLA"),-gv("Union")},
    {gv("Change"),gv("FLA"),-gv("NGO"),gv("Union")},
    {-gv("FLA"),gv("NGO")},{-gv("FLA"),gv("Public")}},
   {{-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Public")},
    {gv("Change"),-gv("FLA"),-gv("Union")},
    {-gv("FLA"),gv("NGO")},{-gv("FLA"),gv("Public")},
    {gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")}},
   {{-gv("Change"),gv("FLA"),-gv("NGO"),-gv("Union")},
    {gv("Change"),-gv("FLA"),-gv("Union")},
    {-gv("FLA"),gv("NGO")},{-gv("FLA"),gv("Public")},
    {gv("FLA"),-gv("NGO"),-gv("Public"),gv("Union")}}]
   \endverbatim
   </li>
  </ul>

*/

