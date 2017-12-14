#!/bin/env bash
workdir=/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/spline_prod/171016_spline_prod_170623_quad9_no_int_SBonly
quad=9
cp ${workdir}/loadLib.C ${workdir}/make_rpdfWS_withCat.cc .

cd ${workdir}

if [ ! -d "${workdir}/rpdfWS_withCat" ];
then
mkdir -p ${workdir}/rpdfWS_withCat
fi


for tag in 4e 4mu 2e2mu
do
cat=0
while [ $cat -lt $quad ]
do
for cate_vbf in 0 1 2
do
for prod_cate in 0 1 2
do
root -l -q -b loadLib.C make_rpdfWS_withCat.cc++\(\"${workdir}\",\"${tag}\",${cat},${quad},${cate_vbf},$prod_cate\)
done
done
((cat++))
done
done

