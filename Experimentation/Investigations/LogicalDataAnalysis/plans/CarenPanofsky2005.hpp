// Matthew Gwynne 2.6.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LogicalDataAnalysis/plans/CarenPanofsky2005.hpp
  \brief Plans regarding investigations of a case-study for the logical analysis of data with temporal relations

  The underlying article is [TQCA A technique for adding temporality to 
  Qualitative Comparative Analysis, Sociological Methods & Research, 2005].

  
  \todo Create milestones

  
  \todo Data
  <ul>
   <li> The paper suggests the need for the inclusion of temporal
   relations into QCA analysis, using a hypothetical dataset 
   of "unionisation attempts" by graduates working at research
   universities. </li>
   </li>
   <li> The underlying QCA truth table data given in the paper is
   summarised in the following combinatorial matrix.
   \verbatim
Caren_CM : mrc2ocom(matrix(
  [1,1,1,1,1],
  [1,1,1,1,1],
  [1,1,1,1,1],
  [1,1,1,0,1],
  [1,1,1,0,1],
  [1,1,0,1,1],
  [1,0,1,1,0],
  [1,0,1,1,0],
  [1,0,0,1,0],
  [1,0,0,1,0],
  [1,0,0,1,0],
  [1,0,0,0,0],
  [0,1,1,1,1],
  [0,1,0,0,0],
  [0,0,0,1,0],
  [0,0,0,0,0],
  [0,0,0,0,0],
  [0,0,0,0,0]),
  ["1_1","1_2","1_3","2_1","2_2","3","5_1","5_2","7_1","7_2","7_3","8","9","12","15","16_1","16_2","16_3"],
  ["Public","Elite", "Affiliate", "Strike", "Representation"])$
   \endverbatim
   </li>
   <li> Note here that no names are given for individual cases, and cases
   with similar values are grouped into single entries in the
   truth table (in the paper), and so these have been separated
   and named using the numbers, and indices, provided. </li>
   <li> The variables here have the following meanings
   <ul>
    <li> Public - whether the unionisation struggle takes place
    at a public or private university. </li>
    <li> Elite - whether or not the struggle has elite allies. </li>
    <li> Affiliate - whether the struggle is affiliated with a
    national union. </li>
    <li> Strike - whether the struggle used a strike or threat 
    of strikes to achieve their goal. </li>
    <li> Representation - whether the struggle achieved representation
    at the university level, indicated by "collective contract" allowances
    etc. </li>
   </ul>
   <li> There is also additionally a TQCA table, where also one
   may use a "--" operator to indicate that a given causal
   condition (variable) occurred in that case before (i.e., "a--b"
   means "a then b") another. How to represent this? </li>
  </ul>

  
  \todo Perform analysis using additional variables
  <ul>
   <li> It should be possible to model such temporal relations with
   additional variables. </li>
   <li> We should produce such a model, perform the appropriate
   minimisations etc and compare with the results in the paper. </li>
  </ul>

*/

