#!/bin/bash
echo "Please enter the directory of files"
read directory

echo "Please enter type of files to process"
read filetype

FILES=$(find -L $directory -name "*$filetype")
totalFiles=$(echo "$FILES" | wc -l)
echo "Total input files: $totalFiles"
globalVariables=0

for path in $FILES
do
	# echo "Processing $path"
    verdict=$(./QCNFglobal $path)
    if [[ -z $verdict ]]
        then 
            # echo "Global variable!"
            (( globalVariables++ ))
        # else 
            # echo "Oh not a global variable!"
    fi
done

echo "Input files with global variables: $globalVariables"
