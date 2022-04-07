#!bin/bash

# Lab_4.sh
# Aleks McCormick
# 2022/03/30
# Spring2022 CS 321
# Shell program demonstrating some system admin commands

# 1) Use the command 'chmod' to modify the permissions to files & directories for owner, group, & others.
#	Create a shell script & give the current user full permission and everyone else read & execute permissions only.

	# Verify at least one parameter
	if(($# < 1))
	then 
		echo "Please enter at least one file or directory to set permissions."
	else
		for file in "$@"
			chmod 755 -R file
		done
	fi

# 2) Create a FAT, NTFS, and EXT3 filesystem in your linux box. Mount it in three seperate sub-directories inside /mnt/.

	# FAT
	dd if=/dev/zero of=fat-image.dd bs=1M count=10
	mkfs.fat fat-image.dd
	mkdir /mnt/FAT_Dir
	mount fat-image.dd /mnt/FAT_Dir

	# NTFS
	dd if=/dev/zero of=ntfs-image.dd bs=1M count=10
	mkfs.ntfs ntfs-image.dd --force
	mkdir /mnt/NTFS_Dir
	mount ntfs-image.dd /mnt/NTFS_Dir

	# EXT3
	dd if=/dev/zero of=ext3-image.dd bs=1M count=10
	mkfs.ext3 ext3-image.dd --force
	mkdir /mnt/EXT3_Dir
	mount ext3-image.dd /mnt/EXT3_Dir

# 3) Use the 'df' command to test it.

	sudo df -h

# 4) Now create three different users called 'testuser1', 'testuser2', & 'testuser3'.

	sudo adduser testuser1
	sudo adduser testuser2
	sudo adduser testuser3

# 5) Create a group called 'myfsuser'

	sudo groupadd myfuser

# 6) Add all three of the users created to the group.

	sudo usermod -a -G myfuser testuser1
	sudo usermod -a -G myfuser testuser2
	sudo usermod -a -G myfuser testuser3

# 7) Set the permission of the FAT filesystem directory created so that only members of the group can access it.

	sudo umount /mnt/FAT_Dir && sudo mount -o remount, gid=myfsuser, uid=testuser1 /mnt/FAT_Dir

# 8) Write a program that will check if the current user has write permission to the FAT filesystem directory.
#	Print results.

	
	test -w mnt/FAT_Dir | if [[ $? ]]
	then 
		echo "You have permission to edit the /mnt/FAT_Dir directory."
	else
		echo "You are denied permission to edit the /mnt/FAT_Dir directory."
	fi

# 9) Now modiify the '~./bashrc' file so that the user will automatically get the message right after login.

	test -w mnt/FAT_Dir | if [[ $? ]]
	then 
		echo "You have permission to edit the /mnt/FAT_Dir directory."
	else
		echo "You are denied permission to edit the /mnt/FAT_Dir directory."
	fi
