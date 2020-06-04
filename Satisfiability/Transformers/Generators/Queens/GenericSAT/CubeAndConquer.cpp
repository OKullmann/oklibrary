// Oliver Kullmann, 3.5.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Cube-and-Conquer for N-Queens

  Three phases:

  (I)   Initial symmetry breaking

        See data/CubeAndConquer/InitialSymmetryBreaking

  (II)  Cube-splitting

  (III) Conquering

  Phases (I), (II) use complete symmetric caching.
  It seems best to cache all nodes in the cube-phase (not just the final nodes
  for conquering), and not to expand a node already seen:
   - This seems to have no disadvantages.
   - While only considering the final layer risks missing caching-opportunities
     in case nodes were not symmetrically expanded.
   - When using say the tawHeuristics for splitting, then full-board caching
     needed to be used.
*/

