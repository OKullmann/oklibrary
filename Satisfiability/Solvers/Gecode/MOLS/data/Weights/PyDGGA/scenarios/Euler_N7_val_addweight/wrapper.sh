#!/usr/bin/env sh

RET_OK=0
RET_TIMEOUT=124   # timeout tool man page

BINARY="Euler"

#if [ ! -f "${BINARY}" ]; then
#    echo "${BINARY} file does not exist"
#    echo "GGA ABORT"
#    exit 1
#fi

#if [ ! -x "${BINARY}" ]; then
#    echo "${BINARY} is not an executable file"
#    echo "GGA ABORT"
#    exit 1
#fi

# process arguments
instance=${1}
cutoff=${2}
seed=${3}
shift 2

weights=`echo $@ | sed s/" "/,/g | cut -d "," -f2-`

# penalty=`echo "10 * ${cutoff}" | bc`
penalty=${cutoff}

# cutoff command
#
# We use GNU core utils timeout tool or the Homebrew equivalent in OSX
# to terminate a program after cutoff seconds. The additional flag
# foreground creates the process within the same process group so that
# it can directly handle signals.
#
uname_out="$(uname -s)"
case "${uname_out}" in
    Linux*)     cutoff_cmd="timeout --foreground ${cutoff}"
    ;;
    Darwin*)    cutoff_cmd="gtimeout --foreground ${cutoff}"
    ;;
    *)          cutoff_cmd=""
esac

# limit resources
ulimit -v 2097152  # 2 GB
# ulimit -t ${cutoff}  # use gnu coreutils timeout program

# run solver
cat ${instance} | ${cutoff_cmd} ${BINARY} 0 0 la,val -sol dom $weights
ret=$?

echo ""  # Make sure the GGA message goes into its own line.
case "${ret}" in
    ${RET_OK}) echo "GGA SUCCESS RUNTIME" ;;
    ${RET_TIMEOUT}) echo "GGA TIMEOUT ${penalty} it could not be solved :(" ;;
    *) echo "GGA CRASHED ${penalty}" ;;
esac

exit ${ret}
