workdir=$1
file=$2
folder=$3
eval `scramv1 runtime -sh`
cd $workdir

sed -e 's~<sample>~'${file}'~g' -e 's~<folder>~'${folder}'~g' < processTree_tmpl.C > processTree_${folder}.C
root -q -l -b processTree_${folder}.C++

rm processTree_${folder}.C*
