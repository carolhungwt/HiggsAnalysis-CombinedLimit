#include "resofit.h"
#include <fstream>
#include "ZZAnalysis/AnalysisStep/interface/Category.h"
#include "ZZAnalysis/AnalysisStep/src/Category.cc"
#include "ZZAnalysis/AnalysisStep/interface/Discriminants.h"
#include "ZZAnalysis/AnalysisStep/src/Discriminants.cc"
#include "ZZAnalysis/AnalysisStep/src/cConstants.cc"
 #include "ZZAnalysis/AnalysisStep/interface/cConstants.h"
using namespace std;


void selectchan(int ZZflav, int &chan);
void calculate_dbkgkin(float p_GG_SIG_ghg2_1_ghz1_1_JHUGen, float p_QQB_BKG_MCFM, int ZZflav, float ZZMass, float &dbkg_kin);
void calculate_Dmass(float ZZMassErrCorr, float ZZMass, float &Dmass);

void processTree_<folder>(){

	// Read the original tree


	TChain* oldtree = new TChain("ZZTree/candTree");
	oldtree->Add("root://lxcms03//data3/Higgs/170623/<sample>");
	
	int nEntries = oldtree->GetEntries();
	total_ent = nEntries;

	int prodCate=0;
	float ZZMass, ZZMassErrCorr, overallEventWeight, dbkg_kin, Dmass, genM; Short_t Z1Flav, Z2Flav;
	Float_t  ZZMassRefit;
	float p_GG_SIG_ghg2_1_ghz1_1_JHUGen, p_QQB_BKG_MCFM;
	Short_t nExtraLep, nExtraZ,  nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF;

vector<float> *jetQGLikelihood=0;
vector<float> *jetPhi=0;


float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal;
float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal;
float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal;
float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal;
float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal;
float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal;
float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal;
float PFMET;
bool useVHMETTagged=0;
bool useQGTagging=1;

	float GenHMass=0,genHEPMCweight=0,PUWeight=0,dataMCWeight=0;


	oldtree->SetBranchAddress("genHEPMCweight",&genHEPMCweight);
	oldtree->SetBranchAddress("PUWeight",&PUWeight);
	oldtree->SetBranchAddress("ZZMass",&ZZMass);
	oldtree->SetBranchAddress("ZZMassRefit",&ZZMassRefit);
	oldtree->SetBranchAddress("ZZMassErrCorr", &ZZMassErrCorr);
//	oldtree->SetBranchAddress("overallEventWeight", &overallEventWeight);
	oldtree->SetBranchAddress("ZZsel",&zzsel);
	oldtree->SetBranchAddress("Z1Flav",&Z1Flav);
 	oldtree->SetBranchAddress("Z2Flav",&Z2Flav);
 	oldtree->SetBranchAddress("p_GG_SIG_ghg2_1_ghz1_1_JHUGen", &p_GG_SIG_ghg2_1_ghz1_1_JHUGen);
 	oldtree->SetBranchAddress("p_QQB_BKG_MCFM", &p_QQB_BKG_MCFM);
	oldtree->SetBranchAddress("GenHMass", &genM);
oldtree->SetBranchAddress("nExtraLep",&nExtraLep);
oldtree->SetBranchAddress("nExtraZ",&nExtraZ);
oldtree->SetBranchAddress("nCleanedJetsPt30",&nCleanedJetsPt30);
oldtree->SetBranchAddress("nCleanedJetsPt30BTagged_bTagSF",&nCleanedJetsPt30BTagged_bTagSF);
oldtree->SetBranchAddress("JetQGLikelihood",&jetQGLikelihood);
oldtree->SetBranchAddress("p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",&p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("p_JQCD_SIG_ghg2_1_JHUGen_JECNominal",&p_JQCD_SIG_ghg2_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal",&p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("p_JVBF_SIG_ghv1_1_JHUGen_JECNominal",&p_JVBF_SIG_ghv1_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal",&pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("p_HadWH_SIG_ghw1_1_JHUGen_JECNominal",&p_HadWH_SIG_ghw1_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("p_HadZH_SIG_ghz1_1_JHUGen_JECNominal",&p_HadZH_SIG_ghz1_1_JHUGen_JECNominal);
oldtree->SetBranchAddress("JetPhi",&jetPhi);
oldtree->SetBranchAddress("ZZMass",&ZZMass);
oldtree->SetBranchAddress("PFMET",&PFMET);

	TString outfile = "/eos/user/w/wahung/Mass_Width_Measurement/Data_Sample/170623/<sample>";
//	TString outfile_tree = outfile + ".root";
	TFile* newFile = new TFile(outfile,"recreate");
	std::vector<float> *wt=0;	
	vector<int> entries;
	vector<int> untagged_entries;
	vector<int> vbf2j_entries;
	vector<int> vhlep_entries;
	TTree* newtree = new TTree("candTree","candTree");
	float upcut, lowcut, cconstant;
	int chan, cat;
	int counter=0;
		newtree->Branch("ZZMass", &ZZMass, "ZZMass/F");
		newtree->Branch("ZZMassRefit", &ZZMassRefit, "ZZMassRefit/F");
		newtree->Branch("ZZsel", &zzsel, "ZZsel/S");
		newtree->Branch("Z1Flav", &Z1Flav, "Z1Flav/S");
		newtree->Branch("Z2Flav", &Z2Flav, "Z2Flav/S");
		newtree->Branch("dbkg_kin", &dbkg_kin, "dbkg_kin/F");
		newtree->Branch("Dmass", &Dmass, "Dmass/F");
		newtree->Branch("overallEventWeight", &overallEventWeight, "overallEventWeight/F");
//		newtree->Branch("cat",&cat, "cat/I");
		newtree->Branch("ZZMassErrCorr", &ZZMassErrCorr, "ZZMassErrCorr/F");
		newtree->Branch("chan", &chan, "chan/I");	
		newtree->Branch("GenHMass", &genM, "GenHMass/F");
		newtree->Branch("prodCate",&prodCate,"prodCate/I");
       // newtress[j]->Branch("genFinalState",&genFinalState,"genFinalState/S");
        newtree->Branch("genHEPMCweight",&genHEPMCweight,"genHEPMCweight/F");
      
        newtree->Branch("PUWeight",&PUWeight,"PUWeight/F");
    		TString tag; 

		int VBF2j=0;

		int VHlep=0;
		int untagged=0;
			
		for(int i=0; i<nEntries ; i++){
			oldtree->GetEntry(i);         	
			selectchan(abs(Z1Flav*Z2Flav), chan);

			if(chan==1)		tag="4mu";
			else if (chan==2)	tag="4e";
			else 			tag="2e2mu";	
	        TFile *f1 = new TFile("/afs/cern.ch/work/u/usarica/public/forMELAv206AndAbove/SmoothKDConstant_m4l_Dbkgkin_"+tag+"13TeV.root");
                TSpline *constsp = (TSpline*) f1->Get("sp_gr_varTrue_Constant_Smooth");
			cconstant = constsp->Eval(ZZMass);
			dbkg_kin = p_GG_SIG_ghg2_1_ghz1_1_JHUGen/(p_GG_SIG_ghg2_1_ghz1_1_JHUGen + p_QQB_BKG_MCFM*cconstant);
			calculate_Dmass(ZZMassErrCorr, ZZMass, Dmass);
			prodCate = categoryMor17(nExtraLep,nExtraZ,nCleanedJetsPt30,nCleanedJetsPt30BTagged_bTagSF,jetQGLikelihood->data(),p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,jetPhi->data(),ZZMass,PFMET,useVHMETTagged,useQGTagging);
			if	(prodCate==2)		VBF2j++;
			else if (prodCate==4)		VHlep++;
			else 				untagged++;
			counter++;
			newtree->Fill();
			f1->Close();	
		}
	
//
		newFile->WriteTObject(newtree);

	newFile->Close(); 
	
}
std::runtime_error invalidinput("invalid input check ZZFlav");
void selectchan(int ZZflav, int &chan){

	if(abs(ZZflav)==121*121)		chan=1;		//4mu
	else if(abs(ZZflav)==169*169)		chan=2;		//4e
	else if(abs(ZZflav)==121*169)		chan=3;		//2e2mu
	else{
		chan=0;
//		cout<<"problematic flav: "<<ZZflav<<endl;
//		throw invalidinput;
	}
		return;
}


void calculate_Dmass(float ZZMassErrCorr, float ZZMass, float &Dmass){

	Dmass = ZZMassErrCorr/ZZMass;
	return;
}

void calculate_dbkgkin(float p_GG_SIG_ghg2_1_ghz1_1_JHUGen, float p_QQB_BKG_MCFM, int ZZflav, float ZZMass, float &dbkg_kin){
	dbkg_kin = p_GG_SIG_ghg2_1_ghz1_1_JHUGen / ( p_GG_SIG_ghg2_1_ghz1_1_JHUGen + p_QQB_BKG_MCFM*getDbkgkinConstant(ZZflav,ZZMass) );
	return;
}


