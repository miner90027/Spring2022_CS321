# Verify that at least a parameter
if(( $# < 1))
then
	echo "Please enter at least one file to compile"
else
	# loop over all the vallues passed
	for file in "$@"
	do
		# verify that the file passed is a readable c file
		if [[ -r "$file.c" ]]
				then
			# compile the passed file
			g++ -c "$file.c" -lpthread -lrt
			# create an exacutable for the file passed
			g++ -o "$file" "$file" -lpthread -lrt
		else
			# Printe error statements if cannot read the file
			echo "Invalid file: $file"
			echo "Please enter the name without the file extension"
		fi
	done
fi
