# Oliver Kullmann, 20.4.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

{ 
  if (match($1,"vdw_trans_")) {
    split($1,a,"_")
    x = a[4]
  }
  if (match($5,"satisfiable") == 1) s = 1
  if (match($5,"unsatisfiable")) s = 0
  if (match($5,"NB_BRANCHE")) {
    split($6,b,",")
    y = b[1]
  }
  if (match($1,"satz215") && s == 0) print x, y
}
