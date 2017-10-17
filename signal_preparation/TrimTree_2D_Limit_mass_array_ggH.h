//TrimTree_2D_Limit.h

Short_t Z1Flav, Z2Flav;
float ZZMass, ZZMassErrCorr, DMass;
float Z1Mass, Z2Mass, mreco;
int ZZflav;
int nEntry;
int event;
int chan;
float siggen, bkggen;
float DBkgKin, dbkg_kin_check;
float genHMass;
float getDbkgkinConstant(int ZZflav, float ZZMass);
float getDBkgKin(int ZZflav, float siggen, float bkggen);
float weightH, weightHBI, weight2;
float overallEventWeight;

//TString f_org="root://lxcms03//data3/Higgs/170222/ggTo2e2mu_Contin_MCFM701/ZZ4lAnalysis.root";
//TString f_org="/afs/cern.ch/user/w/wahung/work/public/Data_Sample/ggZZ_2e2mu.root";
//int masspts[] = {115, 120, 124, 125, 126, 130, 135, 140, 150, 155, 160, 165, 170, 175, 180, 190, 210, 230, 250, 300, 350, 400, 450, 500, 550, 600, 700, 750, 800, 900, 1500, 2000, 3000};
//int masspts[] = {115, 120, 124, 125, 126, 130, 135, 145, 150};
int masspts[]={125};
TString FileDir = "/afs/cern.ch/work/x/xiaomeng/test/myWorkingArea/CMSSW_8_0_6/src/ZZMatrixElement/MELA/test/4l_Moriond/";
//TString f_org = "ggH_2e2mu.root";
//TString f_org="root://lxcms03//data3/Higgs/170222/VBFTo2e2muJJ_Contin_phantom128/ZZ4lAnalysis.root";
//TString f_org="root://lxcms03//data3/Higgs/170222/VBFTo2e2muJJ_0PMH125Contin_phantom128/ZZ4lAnalysis.root";
//TString outputFile = "cond_hist_ggHmH150_2e2mu_withcut.root";
TString outputFile = "cond_hist_ggH_2e2mu_125_mreco.root";
//TString treeAddress = "ZZTree/candTree";
TString treeAddress = "candTree";
