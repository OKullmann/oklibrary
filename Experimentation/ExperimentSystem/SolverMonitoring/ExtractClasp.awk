# Oliver Kullmann, 29.2.2024 (Swansea)
# Copyright 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.1

BEGIN {
  interrupted=0; wt=0; t=0; sat=2; sol="NA"; complete=2; vers=""; file=""
}

/^s UNSATISFIABLE/ { sat=0 }
/^s SATISFIABLE/ { sat=1 }
/^s UNKNOWN/ { sat=2 }

/ clasp version / { vers=$4 }
/^c Reading from / { file=$4 }
/^c INTERRUPTED / { interrupted=$4 }
/^c Time / { wt=substr($4, 1, length($4)-1) }
/^c CPU Time / { t=substr($5, 1, length($5)-1) }
/^c Models / {
  modstr=$4; lc=substr(modstr, length(modstr));
  if (lc == "+") { complete=0; sol=substr(modstr, 1, length(modstr)-1) }
  else { complete=1; sol=modstr }
}

END {
  print interrupted " " wt " " t " " sat " " sol " " complete " \"" vers "\" \"" file "\""
}
