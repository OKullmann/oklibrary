# Oliver Kullmann, 5.4.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

{ 
  if (match($1,"vdw_trans_")) {
    split($1,a,"_")
    x = a[4]
    count = 0
  }
  if (match($2,"sat_status=2") || match($2,"sat_status=1")) count += 1
  if (match($1,"c") && count == 2) {
    split($8,b,"=")
    y = b[2]
    print x, y
  }
    
}
