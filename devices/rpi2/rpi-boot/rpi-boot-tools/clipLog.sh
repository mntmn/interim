#!/bin/bash
#
# From stdin or a file named on the command-line, read the log until a set of
# ~/s which suggest that we have got to the end of the written portion,
# and store it in the defined directory.

LOG_STORE=~/buildTrees/PI/logs

store=$(date "+%Y-%m-%d%_%H:%M:%S.txt"; echo)
# | read store

echo "Stored at '$store'"

/usr/bin/sed -ne "
s/~~~~~~~~.*/==END==/
#t
p
#n
" $* |\
tee $LOG_STORE/$store

