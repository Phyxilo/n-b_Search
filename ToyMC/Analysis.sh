#!/bin/bash

clsSize=(3 4 5)								#Cluster size
rank=(1 2 3 4)								#Cluster rank
outFolder="ClusterSet2"						#Output folder name
Dirs=("5E7/2/pos.dat" "5E7/2/en.dat")		#Data input directories
doClsGen=1									#Boolean for ClusterGen.C macro
doFit=1										#Boolean for MCFit.C macro

OutDatName=ClusterOut						#Data output file name
OutPDFName=ClusterFit						#Fit PDF output file name

if [ ! -d $outFolder ]
then
	mkdir $outFolder
else
	echo Out folder exists
fi

for size in ${clsSize[@]};
do
	DatOuts=()

	OutNames=($OutDatName"Pos_"$size".txt" $OutDatName"En_"$size".txt")
	
	echo "-------------------Cluster Size: "$size"-------------------"

	for dir in ${!Dirs[@]}; 
	do
		inDir=${Dirs[$dir]}
		outFile=$outFolder"/"${OutNames[$dir]}

		echo "Input File: "$inDir

		if [ $doClsGen -eq 1 ]
		then
			root -l -b -q 'ClusterGen.C('\"$inDir\"','$size')'
			mv ClusterOut.txt $outFile
			echo ""
		fi

		echo "Out file: "$outFile
		echo "--------------------------------------"
		DatOuts+=($outFile)
	done

	if [ $doFit -eq 1 ]
	then
		for rnk in ${rank[@]};
		do
			echo "*****************Rank: " $rnk "*****************"
			root -l -b -q 'MCFit.C('\"${DatOuts[0]}\"', '\"${DatOuts[1]}\"', '$rnk')'
			mv ClusterFit.pdf $outFolder"/"$OutPDFName"_CS"$size":R"$rnk".pdf"
		done
	fi
done