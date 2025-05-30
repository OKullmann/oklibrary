#!/bin/bash

# Runs cadical; interface the same as satstd_minisat.bash, except for
# the variable "internaloptions" has now four possible values:
#   - 0 (default) means default cadical-options
#   - 1 means SAT-cadical-options
#   - 2 means UNSAT-cadical-options
#   - 3 means disabling all internal cadical-options.
# As path to cadical just "cadical" is used.
# If another path is needed, set the variable pathcadical.

# EXAMPLES:

# Bicliques> ./satstd_cadical.bash X
# 2.0.0
# Bicliques> echo -e "p cnf 2 2\n1 0\n -2 0" | ./satstd_cadical.bash
# 1 0.00 0.00 0.00 4552
# 1 -2 0
# Bicliques> echo -e "p cnf 2 2\n1 0\n -1 0" | ./satstd_cadical.bash
# 0 0.00 0.00 0.00 4404
# Bicliques> echo -e "p cnf 1 2\n1 0\n -1 0" | ./satstd_cadical.bash
# 0 0.00 0.00 0.00 4508
# Bicliques> echo -e "p cnf 1 2\n1 0\n -1 0" | timeout=1 internaloptions=1 ./satstd_cadical.bash
# 0 0.00 0.00 0.00 4636


set -o errexit
set -o nounset

script_name=$(basename "$0")
version_number="0.3.0"

pathcadical="${pathcadical-cadical}"

if [ $# -eq 1 ]; then
  $pathcadical --version
  exit 0
fi

timeout="${timeout-0}" # "0" means no timeout
inputsource="${inputsource--}" # "-" means standard input

internaloptions=${internaloptions-0}
if [[ $internaloptions = "0" ]]; then
  optionstring="--default"
elif [[ $internaloptions = "1" ]]; then
  optionstring="--sat"
elif [[ $internaloptions = "2" ]]; then
  optionstring="--unsat"
elif [[ $internaloptions = "3" ]]; then
  optionstring="--plain"
else
  echo "ERROR[${script_name}]: wrong internaloptions = \"${internaloptions}\"."
  exit 1
fi

tfstring="ubt %e %U %S %M"

{ /usr/bin/time -f "$tfstring" --output=/dev/stdout --quiet cadical -q -w /dev/stdout -t $timeout "$optionstring" "$inputsource" || true; } | awk 'BEGIN{status=3} /^s SAT/{status=1;skip} /^s UNSAT/{status=0;skip} /^c UNKNOWN/{status=2;skip} /^v /{pass=substr($0, index($0, $2));skip} /^ubt/{wt=$2; ut=$3; st=$4; mem=$5;skip} END{print status, wt, ut, st, mem; if (pass != "") print pass}'
