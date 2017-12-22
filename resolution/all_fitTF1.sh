cur=$1
cur=$(readlink -f $cur)
echo "Current dir $cur"
ch=$2
echo "current channel ${ch}"
cat=$3
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`
cd $cur

rm plotVar_fitInd*cat*.C

#sed -e 's|<workdir>|'$cur'|g' -e 's|<maxCat>|'$quad'|g' < resofit_temp.h > resofit.h


#rm -r params_${ch}_cat${cat} fixParamFit_${ch}_cat${cat}
root -l -n -q -b readData.cc\(\"$ch\",${cat}\) indfit.cc\(\"$ch\",${cat}\)
bash readParam_single_${ch}_cat${cat}.sh $ch $cat
root -l -n -b -q "plotVar_fitInd_${ch}_cat${cat}.C"
bash readParam_ind_pol1.sh $ch $cat
root -l -n -q -b readData.cc\(\"$ch\",${cat}\) simfit_${ch}_cat${cat}.cc\(\"$ch\",${cat}\)

cp params/indiFit_param_all_${ch}_cat${cat}.png params/indiFit_param_all_${ch}_cat${cat}.pdf ~/www/params_quad9_withMassDep/
cp fixParamFit/Resolution_MH*_${ch}_cat${cat}_fixParam.png ~/www/params_quad9_withMassDep/fixParamFit


