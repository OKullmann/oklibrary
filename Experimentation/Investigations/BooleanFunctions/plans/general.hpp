// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/general.hpp
  \brief On investigations regarding boolean functions


  \bug analyse_random_permutations does not create experiment-directories
  <ul>
   <li> Every experiment takes places in its own directory. </li>
   <li> This standard has been explained and used many times. </li>
   <li> Perhaps MG would finally take notice of it. </li>
   <li> Also the weak Bash-usage has to be corrected. </li>
   <li> And the other scripts in this directory need also to be corrected.
   </li>
  </ul>


  \todo Add milestones


  \bug (DONE Added modules to buildsystem makefile)
  Application tests not run
  <ul>
   <li> Yet no recursive descend takes place in module BooleanFunctions. </li>
  </ul>


  \todo DONE (see BooleanFunctions.hpp)
  Plans for investigating random boolean functions
  <ul>
   <li> We need to investigate random boolean functions to
   compare with the AES investigations (see 
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp).
   </li>
   <li> Likely, we should create a new plans file for arbitrary boolean
   functions which may not fit into other categories? </li>
   <li> See "First considerations of random permutation" in 
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp for an
   example of similar investigations into permutations. </li>
  </ul>



*/

