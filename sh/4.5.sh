#!/bin/bash
# Call this script with at least 10 parameters, for example
# ./scriptname 1 2 3 4 5 6 7 8 9 10
MINPARAMS=10
echo
echo "The name of this script is \"$0\"."
# Adds ./ for current directory
echo "The name of this script is \"`basename $0`\"."
echo
if [ -n "$1" ] # Tested variable is quoted.
then
echo "Parameter #1 is $1" # Need quotes to escape #
fi
# -n True if the length of "STRING" is non-zero.
if [ -n "$2" ]
then
echo "Parameter #2 is $2"
fi
if [ -n "$3" ]
then
echo "Parameter #3 is $3"
fi
# ...
if [ -n "${10}" ] # Parameters > $9 must be enclosed in {brackets}.
then
echo "Parameter #10 is ${10}"
fi
echo "-----------------------------------"
echo "All the command-line parameters are: "$*""
# $# is number of argument
if [ $# -lt "$MINPARAMS" ]
then
echo
echo "This script needs at least $MINPARAMS command-line arguments!"
fi
echo


#-z True of the length if "STRING" is zero.
if [ -z $1 ]
then
exit $E_MISSING_POS_PARAM
fi
# However, as Fabian Kreutz points out,
#+ the above method may have unexpected side-effects.
# A better method is parameter substitution:
# ${1:-$DefaultVal}
# See the "Parameter Substition" section
#+ in the "Variables Revisited" chapter.

exit 0