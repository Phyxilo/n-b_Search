#!/bin/bash

clsSize=(3 4 5)								#Cluster size
rank=(1 2 3 4)								#Cluster rank

#outFolder="ClusterSet2"						#Output folder name
outFolder="ClusterSetPythia"
subFolders=("Cluster" "Fit")				#Sub output folder names
#Dirs=("5E7/2/pos.dat" "5E7/2/en.dat")		#Data input directories
Dirs=("Pythia/1E7/posPythia.dat" "Pythia/1E7/enPythia.dat")

doClsGen=1									#Boolean for ClusterGen.C macro
doFit=1										#Boolean for MCFit.C macro
doGraph=1									#Boolean for MCGraph.C macro

OutDatName=ClusterOut						#Data output file name
OutPDFName=ClusterFit						#Fit PDF output file name
OutRatioPDFName=RatioGraph					#Ratio graph output file name

if [ ! -d $outFolder ]
then
	mkdir $outFolder
else
	echo Output folder exists
fi

cd $outFolder

for folder in ${subFolders[@]};
do
	if [ ! -d $folder ]
	then
		mkdir $folder
	else
		echo "Sub output folder, "$folder" exists"
	fi
done

cd ..

for size in ${clsSize[@]};
do
	DatOuts=()

	OutNames=($OutDatName"Pos_"$size".txt" $OutDatName"En_"$size".txt")
	
	echo "-------------------Cluster Size: "$size"-------------------"

	for dir in ${!Dirs[@]}; 
	do
		inDir=${Dirs[$dir]}
		outFile=$outFolder"/"${subFolders[0]}"/"${OutNames[$dir]}

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

	if [ $doGraph -eq 1 ]
	then
		root -l -b -q 'MCGraph.C('\"${Dirs[0]}\"','\"${Dirs[1]}\"')'
		echo "Ratio Graph Output File: "$
		mv Ratio.pdf $outFolder"/"$OutRatioPDFName".pdf"
	fi

	if [ $doFit -eq 1 ]
	then
		for rnk in ${rank[@]};
		do
			echo "*****************Rank: " $rnk "*****************"
			root -l -b -q 'MCFit.C('\"${DatOuts[0]}\"', '\"${DatOuts[1]}\"', '$rnk')'
			mv ClusterFit.pdf $outFolder"/"${subFolders[1]}"/"$OutPDFName"_CS"$size":R"$rnk".pdf"
		done
	fi
done