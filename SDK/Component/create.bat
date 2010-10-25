#!/bin/bash

if [ -n "$1" ]
# Test whether command-line argument is present (non-empty).
then
	output=$1
else
	echo "Usage: ./create.sh COMPONENT_NAME"
	exit 1
fi

if [ -d "$output" ]; then
	echo "Directory $output already exists. Choose different name."
	exit 1
fi

if `mkdir $output`; then
	echo "Directory $output created."
else
	echo "Can't create $output directory."
	exit 1
fi

upperoutput=`echo $output | tr '[:lower:]' '[:upper:]'`

sed -e"s/Example/$output/g" -e"s/EXAMPLE/$upperoutput/g" Example/Example.hpp > $output/$output.hpp
sed -e"s/Example/$output/g" -e"s/EXAMPLE/$upperoutput/g" Example/Example.cpp > $output/$output.cpp
sed -e"s/Example/$output/g" -e"s/EXAMPLE/$upperoutput/g" Example/CMakeLists.txt > $output/CMakeLists.txt

#sed -e's/Kernel/Component/g' -e's/kernel/component/g' -e's/KERNEL/COMPONENT/g' $file > $file.tmp && mv $file.tmp $file
