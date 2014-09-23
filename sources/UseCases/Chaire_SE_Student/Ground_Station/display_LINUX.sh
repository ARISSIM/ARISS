#!/bin/bash

echo -e "\n----------------------------------------------------"
echo -e "Welcome to Display-Function 1.0 for Raspberry AGILE."
echo -e "----------------------------------------------------\n"

while (true)
do
	sleep 2
	nb=`ls | grep '\.zip' | wc -l`
	
	if [ "$nb" != "0" ]
	then
		echo -e $nb" compressed images received! Deflating ..."

		for archive in *.zip
		do
			unzip -q $archive
			mv $archive "Old/"$archive
		done

		echo -e "Displaying the images..."

		for image in *.jpg
		do
			display -resize 50% $image &
			sleep 5
			rm $image
		done
		killall -q display
	fi
done

