# Matthew Henderson, 3.4.2010 (Berea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can 
# redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation and included in this 
# library; either version 3 of the License, or any later version.

{
 if ( $1 == "p" ) {
  print $1 " " $2 " " $3 " " $4+x
 }
 else {
  print $0
 }
}
