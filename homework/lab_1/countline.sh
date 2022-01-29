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
		#echo "Current line: $line"
		#echo "current count:  $count"
	done < "$1"

}

checkread() {
	if [[ -r "$1" ]]
	then
		countlines "$1"
	else
		echo "$1" >> error.txt 2>&1
		cat "$1" >> error.txt 2>&1
	fi
}


#echo $#
declare -i count=0

if (( $# > 0 ))
then
	for input in "$@"
	do

	# check if value is a file
		if [[ -f "$input" ]] 
		then
		#echo ""
		#echo "Currently reading: $input"

		checkread "$input"

	# check if value is a directory
		else [[ -d "$input" ]]
		#echo "This is a directory"

			while IFS= read -rd '' file;
			do 
				#echo "$file"
				checkread "$file"
			done < <(find ./"$input"/ -type f -print0)
		fi
	done
else
	while IFS= read -rd '' file;
	do 
		#echo "$file"
		checkread "$file"
	done < <(find . -type f -print0)
fi
# print the count to the screen
echo "Total number of lines is: $count"
