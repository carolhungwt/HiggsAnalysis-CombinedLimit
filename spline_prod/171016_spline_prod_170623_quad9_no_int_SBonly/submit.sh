workdir=$PWD
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`
cd $workdir

if [ ! -d "${workdir}/rpdfWS_withCat" ];
then
mkdir -p ${workdir}/rpdfWS_withCat
fi

quad=9
cat=0

while [ ${cat} -lt ${quad} ]

do
for ch in 4e 2e2mu 4mu
do
for prodcate in 0 1 2
do
for cate_vbf in 0 1 2
do
bsub -q 1nd -C 0 submit.lsf $workdir $ch $cat $quad $cate_vbf $prodcate
sleep 60
done
done
done
((cat++))
done
echo $workdir
