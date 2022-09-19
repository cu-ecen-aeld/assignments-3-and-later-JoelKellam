#!/bin/bash

writefile=$1
writestr=$2

if [ $# -ne 2 ]; then
    echo "ERROR: $# arguments received. This function requires 2."
    exit 1
fi

full_file_dir=$(dirname "$writefile")
if [[ ! -d $full_file_dir ]]
then
   echo "Creating directory $full_file_dir"
   if ! mkdir -p "$full_file_dir" 2>/dev/null
   then
      echo "ERROR: $full_file_dir could not create directory."
      exit 1
   else 
       echo "Directory $full_file_dir created"
   fi
fi

if ! echo "$writestr" > "$writefile"
then
    echo "ERROR: $writefile could not create file."
    exit 1
fi
