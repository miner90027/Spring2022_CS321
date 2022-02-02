if(( $# < 1))
then
	echo "Please enter at least one file to compile"
else
	for file in "$@"
	do
		
		if [[ -r "$file.c" ]]
		then
			g++ -c "$file.c"
			g++ -o "run_$file" "$file.o"
		else
			echo "Invalid file: $file"
		fi
	done
fi
