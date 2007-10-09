// Oliver Kullmann, 27.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file HypergraphColouring/plans/StrongColouring.hpp
  \brief Plans on strong hypergraph colouring (all vertices in any
  hyperedges get different colours)

  \todo InjectivityConstraints:
  Given a hypergraph G, the most natural way of handling the
  strong k-colouring problem for G is to use for every hyperedge H
  an injectivity constraint resp. a bijectivity constraint in case |H| = k.
  See module InjectivityConstraints.
  The question is, whether something special can be done here?

  \todo Graph colouring conversion:
  Of course, the strong hypergraph colouring problem for G is the same
  as the graph colouring problem for G_{[2]} (the 2-section of G).
  For longer hyperedges the hypergraph treatment should be superior.
  (What is the boundary for hyperedge sizes? Can for example 3-hyperedges
  be efficiently translated into 3 2-edges? It seems quite sure that only for
  a certain minimal hyperedge length the additional effort for strong
  treatment of injectivity constraint is vindicated.)

*/

