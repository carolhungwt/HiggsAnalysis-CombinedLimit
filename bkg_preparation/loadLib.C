{

  TString LIBMCFM="libmcfm_703.so";
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gSystem->AddIncludePath("-I$CMSSW_BASE/interface/");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/ZZMatrixElement/MELA/interface/");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/interface/");
//  gSystem->AddIncludePath("/afs/cern.ch/user/w/wahung/work/CMSSW_8_0_26_patch1/src/HiggsMassConstraint_new/HiggsMassConstraint/interface");
//  gSystem->AddIncludePath("/afs/cern.ch/work/w/wahung/public/CMSSW_7_1_5/tmp/slc6_amd64_gcc481/");
  gSystem->Load("libZZMatrixElementMELA.so");
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");
  gSystem->Load("$CMSSW_BASE/src/ZZMatrixElement/MELA/data/$SCRAM_ARCH/" + LIBMCFM);
  gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
  gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
  gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");

}
