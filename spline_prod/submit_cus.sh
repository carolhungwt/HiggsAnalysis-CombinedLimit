workdir=$PWD
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`
cd $workdir

if [ ! -d "${workdir}/rpdfWS_withCat" ];
then
mkdir -p ${workdir}/rpdfWS_withCat
fi

quad=9
cat=$1
ch=$2
prodcate=$3
cate_vbf=$4

#while [ ${cat} -lt ${quad} ]

#do
#for ch in 4mu 2e2mu
#do
#for prodcate in 1
#do
#for cate_vbf in 0 
#do
bsub -q 1nd -C 0 submit.lsf $workdir $ch $cat $quad $cate_vbf $prodcate
#done
#done
#done
#((cat++))
#done
echo $workdir
