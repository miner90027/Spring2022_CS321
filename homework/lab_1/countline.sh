#!/bin/bash

# Aleks McCormick
# Spring 2022 CS 321
# 2022/24/01
# countline.sh
# shell that takes a file or directory and returns the total number of lines in the file
#	if input was a directory, then reports total number of lines of all the files in all sub
#	directories
#

#echo "hello" >&2 #prints to std error


# Receive user input file
echo "Please enter a file name"
read -p 'File name: ' file

#echo "$file"

declare -i count=0

((++count))

# count the number of lines in the file

cat $file | while read line; do
	count=$(( count + 1 ))
	echo "$line"
done

# print the count to the screen
echo "Total number of lines is:" $count
