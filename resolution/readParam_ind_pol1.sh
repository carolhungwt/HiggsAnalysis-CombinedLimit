#!/bin/bash          


ch=$1
cat=$2
paramIndex=0
for var in {a1,a2,mean,n1,n2,sigma}
do
((paramIndex++))
FitParamStr[$paramIndex]=""

indParam_p0[$paramIndex]="`awk  '/'$ch'_cat'$cat'_'$var'_param_0/{print $2}' params/individual_fit_param_${ch}_cat${cat}.txt`"
indParam_p1[$paramIndex]="`awk  '/'$ch'_cat'$cat'_'$var'_param_1/{print $2}' params/individual_fit_param_${ch}_cat${cat}.txt`"

indFitParamStr[$paramIndex]="${indParam_p0[$paramIndex]}","${indParam_p1[$paramIndex]}"


echo "${var}_indFitParam_${ch}_cat${cat}"
echo "${indFitParamStr[$paramIndex]}"
echo
echo

done


#echo "${indFitParamStr[1]}"

sed  -e 's|<ch>|'${ch}'|g' -e 's|<cat>|'${cat}'|g' -e 's|<mean_param>|'${indFitParamStr[3]}'|g' -e 's|<sigma_param>|'${indFitParamStr[6]}'|g' -e 's|<a1_param>|'${indFitParamStr[1]}'|g' -e 's|<a2_param>|'${indFitParamStr[2]}'|g' -e 's|<n1_param>|'${indFitParamStr[4]}'|g' -e 's|<n2_param>|'${indFitParamStr[5]}'|g'  < simfit_tmpl_pol1.cc > simfit_${ch}_cat${cat}.cc

#mv temp2.cc simfit.cc


