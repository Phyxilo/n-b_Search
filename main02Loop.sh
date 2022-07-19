#!/bin/bash

make main02

for i in {0..7}

do	
	echo "Event: $i"
	./main02.exe $1 runcard_ds.dat $2 $i
done
