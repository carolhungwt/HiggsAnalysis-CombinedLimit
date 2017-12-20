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

void WriteTree(TString tag = "4e", int quad = 9){

	        TFile *f1 = new TFile("/afs/cern.ch/work/u/usarica/public/forMELAv206AndAbove/SmoothKDConstant_m4l_Dbkgkin_"+tag+"13TeV.root");
                TSpline *constsp = (TSpline*) f1->Get("sp_gr_varTrue_Constant_Smooth");

               	for(int i=0; i<maxMassBin; i++){
	// Read the original tree

	TString types[5]={"ggH","VBFH","WminusH","WplusH","ZH"};
	TChain* oldtree = new TChain("ZZTree/candTree");

	for(int type=0; type<sizeof(types)/sizeof(*types); type++){
	        oldtree->Add(Form("root://lxcms03//data3/Higgs/170623/%s%d/ZZ4lAnalysis.root",types[type].Data(), massBin[i]));
	}

	
	int nEntries = oldtree->GetEntries();
	total_ent = nEntries;

	int prodCate=0;
	float ZZMass, ZZMassErrCorr, overallEventWeight, dbkg_kin, Dmass, genM; Short_t Z1Flav, Z2Flav;
	Float_t  ZZMassRefit;
	float p_GG_SIG_ghg2_1_ghz1_1_JHUGen, p_QQB_BKG_MCFM;
	Short_t nExtraLep, nExtraZ,  nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF;

vector<float> *jetQGLikelihood=0;
vector<float> *jetPhi=0;

//float* jetQGLikelihood;
float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal;
float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal;
float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal;
float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal;
float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal;
float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal;
float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal;
//float* jetPhi;
//float ZZMass;
float PFMET;
bool useVHMETTagged=0;
bool useQGTagging=1;
	//short genFinalState=0,ZZsel=0,Z1Flav=0,Z2Flav=0;
	float GenHMass=0,genHEPMCweight=0,PUWeight=0,dataMCWeight=0;

//	oldtree->SetBranchAddress("genFinalState",&genFinalState);
	oldtree->SetBranchAddress("genHEPMCweight",&genHEPMCweight);

	oldtree->SetBranchAddress("PUWeight",&PUWeight);
	oldtree->SetBranchAddress("ZZMass",&ZZMass);
	oldtree->SetBranchAddress("ZZMassRefit",&ZZMassRefit);
	oldtree->SetBranchAddress("ZZMassErrCorr", &ZZMassErrCorr);
	oldtree->SetBranchAddress("overallEventWeight", &overallEventWeight);
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

	vector<float> cutoff;
	float tempcut;

	fstream cuttxt("./MH125_"+tag+"_quad_"+to_string(quad)+".txt",ios::in);
	for(int it=0; it<quad+1; it++){
		cuttxt>>tempcut;
		if (it==0)	continue;
		else if(it==quad)	cutoff.push_back(1.);
		else 			cutoff.push_back(tempcut);
	}


	TString outfile = "./Quad"+to_string(quad)+"_tree/MH"+to_string(massBin[i])+"_"+tag+"_quad_"+to_string(quad);
	TString outfile_tree = outfile + ".root";
	TFile* newFile = new TFile(outfile_tree,"recreate");
	std::vector<float> *wt=0;	
	vector<int> entries;
	vector<int> untagged_entries;
	vector<int> vbf2j_entries;
	vector<int> vhlep_entries;
	vector<TTree*> newtrees(quad);
	float upcut, lowcut, cconstant;
	int chan, cat;
	int counter=0;
	for (int j = 0; j<quad ; j++){

		char treename[100];
		sprintf(treename, "Category_%d", j);
		newtrees[j] = new TTree(Form("Category_%d", j),Form("Category_%d", j));
		newtrees[j]->SetName(Form("Category_%d", j));
		newtrees[j]->Branch("ZZMass", &ZZMass, "ZZMass/F");
		newtrees[j]->Branch("ZZMassRefit", &ZZMassRefit, "ZZMassRefit/F");
		newtrees[j]->Branch("ZZsel", &zzsel, "ZZsel/S");
		newtrees[j]->Branch("Z1Flav", &Z1Flav, "Z1Flav/S");
		newtrees[j]->Branch("Z2Flav", &Z2Flav, "Z2Flav/S");
		newtrees[j]->Branch("dbkg_kin", &dbkg_kin, "dbkg_kin/F");
		newtrees[j]->Branch("Dmass", &Dmass, "Dmass/F");
		newtrees[j]->Branch("overallEventWeight", &overallEventWeight, "overallEventWeight/F");
		newtrees[j]->Branch("cat",&cat, "cat/I");
		newtrees[j]->Branch("ZZMassErrCorr", &ZZMassErrCorr, "ZZMassErrCorr/F");
		newtrees[j]->Branch("chan", &chan, "chan/I");	
		newtrees[j]->Branch("GenHMass", &genM, "GenHMass/F");
		newtrees[j]->Branch("prodCate",&prodCate,"prodCate/I");
       // newtress[j]->Branch("genFinalState",&genFinalState,"genFinalState/S");
        newtrees[j]->Branch("genHEPMCweight",&genHEPMCweight,"genHEPMCweight/F");
      
        newtrees[j]->Branch("PUWeight",&PUWeight,"PUWeight/F");
     
//		newtrees[j]->Branch("reweightingweights",&wt, "reweightingweights/F");
		cat = j;
		upcut = cutoff[j];
		int VBF2j=0;

		int VHlep=0;
		int untagged=0;
		if (j==0){
			lowcut = 0;}
			else {lowcut = cutoff[j-1];}
		for(int i=0; i<nEntries ; i++){
			oldtree->GetEntry(i);
                	
		//if(tag=="4mu" && Z1Flav*Z2Flav != 28561) continue;
         	//if(tag=="4e" && Z1Flav*Z2Flav != 14641) continue;	
         	//if(tag=="2e2mu" && Z1Flav*Z2Flav != 20449) continue;	
	
		if(((tag=="4mu" && Z1Flav*Z2Flav == 14641)||(tag=="4e" && Z1Flav*Z2Flav == 28561)||(tag=="2e2mu" && Z1Flav*Z2Flav == 20449))&&(/*overallEventWeight*/ ZZMassErrCorr/ZZMass > lowcut && /*overallEventWeight*/ZZMassErrCorr/ZZMass < upcut))
		{
		//	if (overallEventWeight* ZZMassErrCorr/ZZMass > lowcut && overallEventWeight *ZZMassErrCorr/ZZMass < upcut)
			selectchan(Z1Flav*Z2Flav, chan);
//			calculate_dbkgkin(p_GG_SIG_ghg2_1_ghz1_1_JHUGen, p_QQB_BKG_MCFM, Z1Flav*Z2Flav, ZZMass, dbkg_kin);
			cconstant = constsp->Eval(ZZMass);
			dbkg_kin = p_GG_SIG_ghg2_1_ghz1_1_JHUGen/(p_GG_SIG_ghg2_1_ghz1_1_JHUGen + p_QQB_BKG_MCFM*cconstant);
			calculate_Dmass(ZZMassErrCorr, ZZMass, Dmass);
			prodCate = categoryMor17(nExtraLep,nExtraZ,nCleanedJetsPt30,nCleanedJetsPt30BTagged_bTagSF,jetQGLikelihood->data(),p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,jetPhi->data(),ZZMass,PFMET,useVHMETTagged,useQGTagging);
			if	(prodCate==2)		VBF2j++;
			else if (prodCate==4)		VHlep++;
			else 				untagged++;
			counter++;
			newtrees[j]->Fill();
			}
		}
		
//		cout<< "Tag counter: "<<VBF2j<<endl;
		cout<<"cutoff at "<< upcut<< " and the number of entries is "<<newtrees[j]->GetEntries()<<endl;
		entries.push_back(newtrees[j]->GetEntries());
//				cout<<j<<endl;	
		//newtree->Print();
		untagged_entries.push_back(untagged);
		vbf2j_entries.push_back(VBF2j);
		vhlep_entries.push_back(VHlep);	

		cout << "Created tree: "<<newtrees[j]->GetName()<< " stored in " << outfile<<endl;
		newFile->WriteTObject(newtrees[j]);
	}

			entries.push_back(counter);
			untagged_entries.push_back(counter);
			vbf2j_entries.push_back(counter);
			vhlep_entries.push_back(counter);	
		


	// Writing cutoff to txt files for quick access
	std::vector<std::string> cutoff_str;
	for(int k=0; k<4; k++){
		if(k==0){
				cutoff_str.push_back("Total:    ");
				for(int i =0; i<entries.size(); i++)
				cutoff_str.push_back(to_string(entries[i]));
				
			}

		else if(k==1){
				cutoff_str.push_back("Untagged:    ");
				for(int i =0; i<untagged_entries.size(); i++)
				cutoff_str.push_back(to_string(untagged_entries[i]));
			}
		else if(k==2){
				cutoff_str.push_back("VBF2J:    ");
				for(int i =0; i<vbf2j_entries.size(); i++)
				cutoff_str.push_back(to_string(vbf2j_entries[i]));
			}	
		else{
				cutoff_str.push_back("VHlep:    ");
				for(int i =0; i<vhlep_entries.size(); i++){
				cutoff_str.push_back(to_string(vhlep_entries[i]));
			}
		}
		cutoff_str.push_back("\n");
	}

//	if 
	std::ofstream output_txt("./Quad"+to_string(quad)+"_tree/MH"+to_string(massBin[i])+"_"+tag+"_check_entries.txt");
	std::ostream_iterator<std::string> output_iterator(output_txt, " ");
	std::copy(cutoff_str.begin(), cutoff_str.end(), output_iterator);
	newFile->Close(); 
	//delete f;
	//delete newfile;
}	
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


