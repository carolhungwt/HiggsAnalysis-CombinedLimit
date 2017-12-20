workdir=$1
tag=$2
cat=$3
quad=$4
cate_vbf=$5
prod_cate=$6
echo $cat
echo $quad
cd ${workdir}
root -l -q -b loadLib.C make_rpdfWS_withCat.cc++\(\"${workdir}\",\"${tag}\",${cat},${quad},${cate_vbf},$prod_cate\)
