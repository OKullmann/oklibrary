// Oliver Kullmann, 7.3.2003 (Swansea)
/* Copyright 2003 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp
  \brief Experimental code regarding 3-valued types
  \deprecated Needs to be updated.

  \todo Establishing meaning
  <ul>
   <li> What does "inv" in Auxiliary::Evaluation_status stand for? </li>
  </ul>
*/

#ifndef AUXILIARYWAECHTER_nBd610O
#define AUXILIARYWAECHTER_nBd610O

#include <string>

namespace Auxiliary {

  enum Sat_status {unsatisfiable, satisfiable, unknown};

  enum Evaluation_status {inv_false, inv_true, not_inv};

  // DEPRICATED
  enum Pass_evaluation {satisfied, falsified, undefined};

  const std::string null_variable_tag = "NULL";
}

#endif
