// Rui Wang, 28.09.2009 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/ReingoldTilford.hpp
  \brief Plans regarding Reingold-Tilford algorithm in Maxima/Lisp
  
  <li> A Maxima example of the mirror image of binary trees. 
       mt(T) := reverse(makelist(mt(T[i]),i,1,length(T)))</li>