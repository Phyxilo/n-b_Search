#!/bin/bash

clsSize=4
rank=1
outFolder="ClusterSet1"
Dirs=("5E7/1/pos.dat" "5E7/1/en.dat")

OutDatName=("ClusterOutPos_"$clsSize".txt" "ClusterOutEn_"$clsSize".txt")
OutPDFName=ClusterFit
DatOuts=()

if [ ! -d $outFolder ]
then
	mkdir $outFolder
else
	echo Out folder exists
fi

for dir in ${!Dirs[@]}; 
do
	inDir=${Dirs[$dir]}
	outFile=$outFolder"/"${OutDatName[$dir]}

	echo "Input File: "$inDir

	root -l -b -q 'ClusterGen.C('\"$inDir\"','$clsSize')'
	mv ClusterOut.txt $outFile

	echo "Out file: "$outFile
	DatOuts+=($outFile)
done

root -l -b -q 'MCFit.C('\"${DatOuts[0]}\"', '\"${DatOuts[1]}\"', '$rank')'
mv ClusterFit.pdf $outFolder"/"$OutPDFName"_R"$rank".pdf"