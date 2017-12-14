
#workdir=$PWD
basedir=$PWD
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`


if [ ! -d "$basedir/rpdfWS_withCat" ];
then
mkdir -p $basedir/rpdfWS_withCat
fi

quad=9
for tag in 4e 2e2mu 4mu
do
cd $basedir
workdir=$(mktemp -d)
cd $workdir
echo $workdir
cd ${basedir}
cp submit_make_rpdfWS.sh submit_make_rpdfWS.lsf make_rpdfWS_withCat.cc loadLib.C ${workdir}
cd ${workdir}
echo $CMSSW_BASE
ls $PWD

bsub -q 1nd -C 0 submit_make_rpdfWS.lsf $basedir $quad $tag $workdir
done
echo $workdir
