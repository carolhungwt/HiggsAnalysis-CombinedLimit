#/bin/env bash
echo $PWD
workdir=$PWD
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`
cd $workdir

#for quad in 1 2 
ch=$1
#do
quad=9
if [ ! -d Quad${quad}_tree ]; then mkdir "${workdir}/Quad${quad}_tree"; fi
if [ ! -d ${workdir}/singleMassFit_Quad$quad ]; then mkdir "${workdir}/singleMassFit_Quad$quad"; fi

sed -e 's|<maxCat>|'$quad'|g' < resofit_temp.h > resofit.h
#for ch in 2e2mu #4e 4mu
#do
#root -l -n -q -b WriteTree_WriteCutoff.C\(\"$ch\",$quad\)
root -l -n -q -b WriteTree.C++\(\"$ch\",$quad\)
#done

#rm -rf ${workdir}/Quad${quad}_tree/*.root
#rm ${workdir}/resofit.h
#done
