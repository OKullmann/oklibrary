#!/bin/bash

# Runs minisat, outputs one line

# SAT wall-time user-time system-time max-residual-mem intopt

# with SAT in {0,1,2,3} (unsat, sat, indeterminate, error),
# and intopt in {0,1} (without/without preprocessing).
# If satisfiable, then on a second line the satisfying assignment is
# printed (in clause-format).
# Setting timeout=0 means the maximal possible timeout.


# EXAMPLES:

# Version number of minisat by any argument (normal usage has no arguments):
# Bicliques> ./satstd_minisat.bash X
# 2.2.0

# Bicliques> echo -e "p cnf 2 2\n1 0\n -2 0" | ./satstd_minisat.bash
# 1 0.00 0.00 0.00 5372 0
# 1 -2 0
# Bicliques> echo -e "p cnf 2 2\n1 0\n -1 0" | ./satstd_minisat.bash
# WARNING! DIMACS header mismatch: wrong number of variables.
# 0 0.00 0.00 0.00 4936 0
# Remark: the warning is print on standard error.
# Bicliques> echo -e "p cnf 1 2\n1 0\n -1 0" | ./satstd_minisat.bash
# 0 0.00 0.00 0.00 4920 0


# By default the input-source is standard input, but by setting
# the variable inputsource, a file can be read instead.
# As path to minisat just "minisat" is used.
# If another path is needed, set the variable pathminisat.

# By default time-out is maximal, but by setting variable timeout to a
# time-out in seconds (natural number) the computation will be
# aborted if the time-limit reached.
# The variable "internaloptions" has two possible values:
#  - 0 (default) means default minisat-options
#  - 1 means no preprocessing.

# Examples with both variables set:
# Bicliques> echo -e "p cnf 1 2\n1 0\n -1 0" | timeout=1 internaloptions=1 ./satstd_minisat.bash
# 0 0.00 0.00 0.00 5124 1


set -o errexit
set -o nounset

script_name=$(basename "$0")
version_number="0.2.4"

pathminisat="${pathminisat-minisat}"

if [ $# -eq 1 ]; then
  echo "2.2.0"
  exit 0
fi

default_timeout=2147483647
timeout="${timeout-${default_timeout}}"
if [[ $timeout -eq 0 ]]; then
  timeout=${default_timeout}
fi
inputsource="${inputsource-/dev/stdin}"

internaloptions=${internaloptions-0}
if [[ $internaloptions = "0" ]]; then
  optionstring="-pre"
elif [[ $internaloptions = "1" ]]; then
  optionstring="-no-pre"
else
  echo "ERROR[${script_name}]: wrong internaloptions = \"${internaloptions}\"."
  exit 1
fi

tfstring="ubt %e %U %S %M"

{ /usr/bin/time -f "$tfstring" --output=/dev/stdout --quiet minisat -verb=0 -cpu-lim=$timeout "$optionstring" "$inputsource" /dev/stdout || true; } | awk -v intopt=$internaloptions 'BEGIN{status=3} /^WARNING/{skip} /^SAT/{status=1;skip} /^UNSAT/{status=0;skip} /^INDET/{status=2;skip} / 0$/{pass=$0} /^ubt /{wt=$2; ut=$3; st=$4; mem=$5} END{print status, wt, ut, st, mem, intopt; if (pass != "") print pass}'
