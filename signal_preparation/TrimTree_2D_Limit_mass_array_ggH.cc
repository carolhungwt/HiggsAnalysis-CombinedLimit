//TrimTree_2D_Limit.cc
//#include "external_cConstants.h"
#include "TrimTree_2D_Limit_mass_array_ggH.h"

using namespace std;
using namespace RooFit;

void TrimTree_2D_Limit_mass_array_ggH(){
//	TString f_org = "~/work/public/Data_Sample/170222/AllData/ZZ4lAnalysis.root";
//	TString f_org = "~/work/public/Data_Sample/ggH125/ZZ4lAnalysis.root";
	TChain* tc = new TChain(treeAddress);
	for(int i=0; i<sizeof(masspts)/sizeof(int); i++){
//		tc->Add(Form(FileDir+"VBFHmH%d_2e2mu_gluon_all.root", masspts[i]));
		tc->Add(Form(FileDir+"ggHmH%d_2e2mu.root", masspts[i]));
	}
//	TFile* fnew = new TFile("cond_hist.root", "recreate");
	TFile* fnew = new TFile(outputFile, "recreate");
	TTree* t1 = new TTree("t1", "tree with cond");
	nEntry = tc->GetEntries();
//	tc->SetBranchAddress("Z1Mass", &Z1Mass);
//	tc->SetBranchAddress("Z2Mass", &Z2Mass);	
	tc->SetBranchAddress("Z1Flav", &Z1Flav);
	tc->SetBranchAddress("Z2Flav", &Z2Flav);
	tc->SetBranchAddress("ZZMass", &ZZMass);
	tc->SetBranchAddress("ZZMassErrCorr", &ZZMassErrCorr);
	tc->SetBranchAddress("p_GG_SIG_ghg2_1_ghz1_1_JHUGen", &siggen);
	tc->SetBranchAddress("p_QQB_BKG_MCFM", &bkggen);
	tc->SetBranchAddress("GenHMass", &genHMass);
//	tc->SetBranchAddress("overallEventWeight", &overallEventWeight);
	tc->SetBranchAddress("weightH", &weightH);
	tc->SetBranchAddress("weightHBI",&weightHBI);
	tc->SetBranchAddress("weight2",&weight2);
	tc->SetBranchAddress("dbkg_kin",&dbkg_kin_check);
	tc->SetBranchAddress("chan", &chan);


	t1->Branch("Channel", &chan, "Channel/I");
	t1->Branch("Event Number", &event, "Event Number/I");
	t1->Branch("mreco", &ZZMass, "mreco/F");
//	t1->Branch("mreco", &mreco, "mreco/F");	
	t1->Branch("ZZMassErrCorr", &ZZMassErrCorr, "ZZMassErrCorr/F");
	t1->Branch("DMass", &DMass, "DMass/F");
	t1->Branch("Z1Flav",&Z1Flav, "Z1Flav/S");
	t1->Branch("Z2Flav", &Z2Flav, "Z2Flav/S");
	t1->Branch("DBkgKin", &DBkgKin, "DBkgKin/F");
	t1->Branch("overallEventWeight", &overallEventWeight);
	t1->Branch("weightH", &weightH, "weightH/F");
	t1->Branch("weightHBI",&weightHBI, "weightHBI/F");
	t1->Branch("weight2",&weight2, "weight2/F");
	t1->Branch("GenHMass", &genHMass,"genHMass/F");
	for(int i=0; i<tc->GetEntries(); i++){
		tc->GetEntry(i);
		if(Z1Flav==Z2Flav && abs(Z1Flav) == 121)	chan = 1; 			// 4mu channel
		else if(Z1Flav==Z2Flav && abs(Z1Flav) == 169) chan = 2; 		// 4e channel
		else chan = 3 ;													// 2e2mu channel
		if(ZZMass>140||ZZMass<105./*||abs(genHMass-125.)>0.5*/)	continue;
		event = i;
		ZZflav = Z1Flav*Z2Flav;
		DBkgKin = getDBkgKin(ZZflav, siggen, bkggen);
		if(i%30000==0){
		cout<<genHMass<<endl;
//		cout<<"my calc: "<<DBkgKin<<"   in tree: "<<dbkg_kin_check<<endl;
		}

		DMass = ZZMass/ZZMassErrCorr;
//		mreco = Z1Mass+Z2Mass;
		t1->Fill();
	}
	cout<<"Total number of entries: "<<t1->GetEntries()<<endl;
	fnew->cd();
	t1->Write();
	fnew->Close();

}

float getDBkgKin(int ZZflav, float siggen, float bkggen){
	// siggen: p_GG_SIG_ghg2_1_ghz1_1_JHUGen, bkggen: p_QQB_BKG_MCFM
	float KD = siggen / ( siggen + bkggen *getDbkgkinConstant(ZZflav,ZZMass));
	return KD;
}

float getDbkgkinConstant(int ZZflav, float ZZMass){ // ZZflav==id1*id2*id3*id4
   float par[14]={
      0.775,
      -0.565,
      70.,
      5.90,
      -0.235,
      130.1,
      13.25,
      -0.33,
      191.04,
      16.05,
      187.47,
      -0.21,
      1700.,
      400.
   };  
   if (abs(ZZflav)==121*121 || abs(ZZflav)==121*242 || abs(ZZflav)==242*242) par[11]=-0.42; // 4e
   float kappa =
   par[0]
   +par[1]*exp(-pow(((ZZMass-par[2])/par[3]), 2)) 
   +par[4]*exp(-pow(((ZZMass-par[5])/par[6]), 2)) 
   +par[7]*(
            exp(-pow(((ZZMass-par[8])/par[9]), 2))*(ZZMass<par[8])
            + exp(-pow(((ZZMass-par[8])/par[10]), 2))*(ZZMass>=par[8])
            )   
   + par[11]*exp(-pow(((ZZMass-par[12])/par[13]), 2));
   
   float constant = kappa/(1.-kappa);
   return constant;
}


