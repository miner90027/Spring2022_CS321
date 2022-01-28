#!/bin/bash

# Aleks McCormick
# Spring 2022 CS 321
# 2022/24/01
# countline.sh
# shell that takes a file or directory and returns the total number of lines in the file
#	if input was a directory, then reports total number of lines of all the files in all sub
#	directories
#


countlines(){
# count the number of lines in the file
	while IFS= read -r line
	do

		((++count))
		echo "Current line: $line"
		echo "current count:  $count"
	done < "$1" 
#	return $count
}



declare -i count=0


for input in "$@"
do

# check if value is a file
	if [[ -f "$input" ]] 
	then
	echo ""
	echo "Currently reading: $input"

	countlines "$input"
	fi


# check if value is a directory
	if [[ -d "$input" ]]
	then
	echo "This is a directory"

		while IFS= read -rd '' file;
		do echo "$file"
			countlines "$file"
		done < <(find ./"$input"/ -type f -print0)
	fi
done

# print the count to the screen
echo "Total number of lines is: $count"
