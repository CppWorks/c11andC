#!/bin/bash
# Author: Sigurd Solaas, 20 Apr 2011
# Used in ABS Guide with permission.
# Requires version 4.2+ of Bash.
key="no value yet"
case "$key" in
$'\x1b\x5b\x32\x7e') # Insert
	echo Insert Key
;;
esac
exit