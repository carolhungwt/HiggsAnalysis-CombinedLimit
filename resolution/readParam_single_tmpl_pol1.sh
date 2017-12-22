#!/bin/bash          

ch=$1
cat=$2
MassStr=""
paramIndex=0
massIndex=0
#masslist=( 140 145 150 155 160 170 175 180 200 )
masslist=(<massarray>)
for mass in "${masslist[@]}"
do
MassStr+="$mass,"
((massIndex++))
done
MassStr=<MassStr>
echo "$MassStr"
paramdir=<paramdir>
#paramdir=/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/resolution_pkg/resolution_Quad9
for var in {a1,a2,mean,n1,n2,sigma}

#for var in n1
do
((paramIndex++))
ParamStr[$paramIndex]=""
ErrStr[$paramIndex]=""

for mass in "${masslist[@]}"
do
ParamStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $3}' ${paramdir}/singleMassFit_Quad9/SingleMassFit_ResoParam_MH${mass}_${ch}_Category_${cat}.txt`"
if [ $mass -eq 125 ]; then ErrStr[$paramIndex]+=0,;else
ErrStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $5}' ${paramdir}/singleMassFit_Quad9/SingleMassFit_ResoParam_MH${mass}_${ch}_Category_${cat}.txt`"; fi
done

echo "${var}"
echo "${ParamStr[$paramIndex]}"
echo "${var}_Error"
echo "${ErrStr[$paramIndex]}"

done

echo "Number of mass points = $massIndex"
echo "Number of parameters = $paramIndex"

sed -e 's|<cat>|'$cat'|g' -e 's|<massArray>|'$MassStr'|g' -e 's|<nArray>|'$massIndex'|g'  -e 's|<meanArray>|'${ParamStr[3]}'|g'  -e 's|<sigmaArray>|'${ParamStr[6]}'|g'  -e 's|<a1Array>|'${ParamStr[1]}'|g'  -e 's|<a2Array>|'${ParamStr[2]}'|g'  -e 's|<n1Array>|'${ParamStr[4]}'|g'  -e 's|<n2Array>|'${ParamStr[5]}'|g'  -e 's|<meanErrArray>|'${ErrStr[3]}'|g'  -e 's|<sigmaErrArray>|'${ErrStr[6]}'|g' -e 's|<a1ErrArray>|'${ErrStr[1]}'|g' -e 's|<a2ErrArray>|'${ErrStr[2]}'|g' -e 's|<n1ErrArray>|'${ErrStr[4]}'|g' -e 's|<n2ErrArray>|'${ErrStr[5]}'|g' -e 's|<channel>|'$ch'|g' < plotVar_fitInd_tmpl_pol1.C > plotVar_fitInd_${ch}_cat${cat}.C


