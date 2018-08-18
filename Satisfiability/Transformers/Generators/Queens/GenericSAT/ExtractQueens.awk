# Oliver Kullmann, 16.8.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Extracts the numerical data from output of ExpQueens, plus the result
# of the time-command
# Can be used on a file with many ExpQueens-outputs, and for this assumes the
# line "c user" is the last one of one block.

BEGIN {
  N=0; sol=0; nds=0; h=0; munds=0; msnds=0; hts=0; t="NA"
}

/^c N / { N = $3 }
/^c solutions / { sol = $3 }
/^c nodes / { nds = $3 }
/^c height / { h = $3 }
/^c max_unodes / { munds = $3 }
/^c max_snodes / { msnds = $3 }
/^c HortonStrahler / { hts = $3 }
/^c user / { t = $3; print N " " sol " " nds " " h " " munds " " msnds " " hts " " t; t="NA" }

END {

}
