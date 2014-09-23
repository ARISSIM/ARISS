#!/bin/bash

tput reset

echo -e "\n----------------------------------------------------"
echo -e "Welcome to Display-Function 1.0 for Raspberry AGILE."
echo -e "----------------------------------------------------\n"

while (true)
do
	nb=`ls | grep '\.zip' | wc -l`
	
	if [ "$nb" -ne 0 ]
	then
		echo -e $nb" compressed images received! Deflating ..."
		sleep 2
		
		for archive in *.zip
		do
			unzip -qq $archive
			mv $archive "Old/"$archive
		done

		echo -e "Displaying the images..."
		
		for image in *.jpg
		do
			open -a preview $image &
			sleep 10
			rm $image
		done
		
		preview_pid=`ps -A | grep -m1 Preview | awk '{print $1}'`
		kill -9 $preview_pid
		
		echo -e "Done !\n"
	fi
done

