#!/bin/bash

# EXAMPLES:

# Version number of cadical by any argument (normal usage has no arguments):
# Bicliques> ./use_cadical.bash X
# 2.0.0

# Reading from standard input, writing result to standard output
# (but no satisfying assignments output):
# Bicliques> echo -e "p cnf 2 2\n1 0\n -2 0" | ./use_cadical.bash
# s SATISFIABLE
# c time-data 0.00 0.00 0.00 4384
# THe comment-line contains (as with the time-command)
#   wall-clock-time user-time system-time max-residual-memory(kb)
# Bicliques> echo -e "p cnf 2 2\n1 0\n -1 0" | ./use_cadical.bash
# s UNSATISFIABLE
# c time-data 0.00 0.00 0.00 4468

# By default the input-source is standard input, but by setting
# the variable inputsource, a file can be read instead.

# By default no time-out, but by setting variable timeout to a
# time-out in seconds (natural number) the computation will be
# aborted if needed (0 means here no timeout).

# To store the solution in a file, set variable solution to this
# filename. This might be standard output, in which case we have
# the solution-information first (as above, but now possible with
# a v-line), followed by the comment-line as above:
# Bicliques> echo -e "p cnf 2 2\n1 0\n -2 0" | solution=/dev/stdout ./use_cadical.bash
# s SATISFIABLE
# v 1 -2 0
# c time-data 0.00 0.00 0.00 4492
# Bicliques> echo -e "p cnf 2 2\n1 0\n -1 0" | solution=/dev/stdout ./use_cadical.bash
# s UNSATISFIABLE
# c time-data 0.00 0.00 0.00 4452

# The variable "internaloptions" has four possible values:
#  - 0 (default) means default cadical-options
#  - 1 means SAT-cadical-options
#  - 2 means UNSAT-cadical-options
#  - 3 means disabling all internal cadical-options.

# Remarks: As path to cadical just "cadical" is used.
# If another path is needed, set the variable pathcadical.
# A temporary file "use_cadical.bash_$$_basefilename" is created,
# with basefilename=STDIN in case of input from standard input.


set -o errexit
set -o nounset

script_name=$(basename "$0")
version_number="0.2.1"

pathcadical="${pathcadical-cadical}"

if [ $# -eq 1 ]; then
  $pathcadical --version
  exit 0
fi

timeout="${timeout-0}" # "0" means no timeout
inputsource="${inputsource--}" # "-" means standard input
solution="${solution-0}" # "0" means no solution, otherwise the filename

internaloptions=${internaloptions-0}
if [[ $internaloptions = "0" ]]; then
  optionstring=""
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

tfstring="c usrbintime %e %U %S %M"

if [[ $solution == "0" ]]; then
  /usr/bin/time -f "$tfstring" --output="/dev/stdout" --quiet cadical -q -n -t $timeout $optionstring $inputsource; satexit=$?
else
  /usr/bin/time -f "$tfstring" --output="/dev/stdout" --quiet cadical -q -w $solution -t $timeout $optionstring $inputsource; satexit=$?
fi

exit $satexit
