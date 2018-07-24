# Oliver Kullmann, 24.7.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of autL1, in a single line.
# Can be used on a file with many autL1-outputs.

BEGIN {
  n=0; c=0
}

/^c n / { n = $3 }
/^c c / { c = $3; print n " " c }

END { 
  
}
