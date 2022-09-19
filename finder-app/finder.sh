#!/bin/bash

filesdir=$1
searchstr=$2

if [ $# -ne 2 ]; then
    echo "ERROR: $# arguments received. This function requires 2."
    exit 1
fi

if [ -d "$filesdir" ]; then
    : # NOP
else
    echo "ERROR: $filesdir doesnt exist."
    exit 1
fi

X=$(find $filesdir -mindepth 1 -type f | wc -l)
err_code=$?

Y=$(grep -r $filesdir -e $searchstr | wc -l)
err_code=$(($?+$err_code))

echo "The number of files are $X and the number of matching lines are $Y"
exit $err_code
