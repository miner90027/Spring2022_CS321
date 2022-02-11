# Verify that at least a parameter
if(( $# < 1))
then
	echo "Please enter at least one file to compile"
else
	# loop over all the vallues passed
	for file in "$@"
	do
		# verify that the file passed is a readable c file
		if [[ -r "$file.c" ]] || [[ -r "$file.cpp" ]]
				then
			# compile the passed file
			# create an exacutable for the file passed
			g++ "$file.c" -o "$file"
		else
			# Printe error statements if cannot read the file
			echo "Invalid file: $file"
			echo "Please enter the name without the file extension"
		fi
	done
fi
