#include <fstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TSpline.h"
#include "TChain.h"
#include "ZZAnalysis/AnalysisStep/interface/Category.h"
#include "ZZAnalysis/AnalysisStep/src/Category.cc"
#include "ZZAnalysis/AnalysisStep/interface/Discriminants.h"
#include "ZZAnalysis/AnalysisStep/src/Discriminants.cc"
#include "ZZAnalysis/AnalysisStep/src/cConstants.cc"
 #include "ZZAnalysis/AnalysisStep/interface/cConstants.h"
#include <vector>
using namespace std;
void qqzz_withDmass(){

		TString treename [3]={"ZZTree/candTree","ZZTreelooseEle/candTree","ZZTreetle/candTree"};
		TString newtreename[3]={"","_rse","_tle"};

		TFile *f1 = new TFile("/afs/cern.ch/work/u/usarica/public/forMELAv206AndAbove/SmoothKDConstant_m4l_Dbkgkin_4e13TeV.root");
		TFile *f2 = new TFile("/afs/cern.ch/work/u/usarica/public/forMELAv206AndAbove/SmoothKDConstant_m4l_Dbkgkin_4mu13TeV.root");
		TFile *f3 = new TFile("/afs/cern.ch/work/u/usarica/public/forMELAv206AndAbove/SmoothKDConstant_m4l_Dbkgkin_2e2mu13TeV.root");
				

		TSpline *const4e = (TSpline*) f1->Get("sp_gr_varTrue_Constant_Smooth");
		TSpline *const4mu = (TSpline*) f2->Get("sp_gr_varTrue_Constant_Smooth");
		TSpline *const2e2mu = (TSpline*) f3->Get("sp_gr_varTrue_Constant_Smooth");

		f1->Close();
		f2->Close();
		f3->Close();

		vector<TSpline*> sps;
		sps.push_back(const4e);
		sps.push_back(const4mu);
		sps.push_back(const2e2mu);
		TFile* fnew = new TFile("qqzz_withDmass_quad9.root","recreate");	



//		for(int t =0;t<3;t++){
		for(int t =0;t<1;t++){
	  TChain *tqqzz= new TChain(treename[t]);
	  tqqzz->Add("root://lxcms03://data3/Higgs/170623/ZZTo4lext/ZZ4lAnalysis.root");
          //tqqzz->Add("root://lxcms03://data3/Higgs/160624/ZZTo4l/ZZ4lAnalysis.root");
		TH2F *temp_zz = new TH2F("temp_zz"+newtreename[t],"",289,110,3000,30,0.,1.); 
		TH1F *zz_m125 = new TH1F("zz_m125", "", 20, 0,1);

		float ZZPt,ZZMass,ZZMassErrCorr, dmass;
		float xsec,KFactorEWKqqZZ,overallEventWeight,KFactorQCDqqZZ_M;
		vector<float> *LepPt=new vector<float>;
		short Z1Flav,Z2Flav;
//		short nCleanedJetsPt30;
		float dbkg_kin;
		float pvbf_VAJHU_old;
		float phjj_VAJHU_old;
		float bkg_VAMCFM,p0plus_VAJHU;
		short ZZsel;
		float TLE_dR_Z;
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
		bool useVHMETTagged=1;
		bool useQGTagging=0;
		vector<short> *LepLepId=new vector<short>;
		tqqzz->SetBranchAddress("p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal",&pvbf_VAJHU_old);
		tqqzz->SetBranchAddress("p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",&phjj_VAJHU_old);
		tqqzz->SetBranchAddress("p_GG_SIG_ghg2_1_ghz1_1_JHUGen",&p0plus_VAJHU);
		tqqzz->SetBranchAddress("p_QQB_BKG_MCFM",&bkg_VAMCFM);
		tqqzz->SetBranchAddress("ZZPt",&ZZPt);
		tqqzz->SetBranchAddress("ZZMass",&ZZMass);
		tqqzz->SetBranchAddress("ZZMassErrCorr",&ZZMassErrCorr);
		tqqzz->SetBranchAddress("Z1Flav",&Z1Flav);
		tqqzz->SetBranchAddress("Z2Flav",&Z2Flav);
		tqqzz->SetBranchAddress("KFactor_EW_qqZZ",&KFactorEWKqqZZ);
		//tqqzz->SetBranchAddress("KFactorEWKqqZZ",&KFactorEWKqqZZ);
		tqqzz->SetBranchAddress("xsec",&xsec);
		tqqzz->SetBranchAddress("ZZsel",&ZZsel);
//		tqqzz->SetBranchAddress("TLE_dR_Z",&TLE_dR_Z); 
		tqqzz->SetBranchAddress("LepLepId",&LepLepId);
		tqqzz->SetBranchAddress("LepPt",&LepPt);
		tqqzz->SetBranchAddress("overallEventWeight",&overallEventWeight);
		tqqzz->SetBranchAddress("KFactor_QCD_qqZZ_M",&KFactorQCDqqZZ_M);
		//tqqzz->SetBranchAddress("KFactorQCDqqZZ_M",&KFactorQCDqqZZ_M);
		tqqzz->SetBranchAddress("nCleanedJetsPt30",&nCleanedJetsPt30);
		tqqzz->SetBranchAddress("nExtraLep",&nExtraLep);
		tqqzz->SetBranchAddress("nExtraZ",&nExtraZ);
		tqqzz->SetBranchAddress("nCleanedJetsPt30",&nCleanedJetsPt30);
		tqqzz->SetBranchAddress("nCleanedJetsPt30BTagged_bTagSF",&nCleanedJetsPt30BTagged_bTagSF);
		tqqzz->SetBranchAddress("JetQGLikelihood",&jetQGLikelihood);
		tqqzz->SetBranchAddress("p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",&p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("p_JQCD_SIG_ghg2_1_JHUGen_JECNominal",&p_JQCD_SIG_ghg2_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal",&p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("p_JVBF_SIG_ghv1_1_JHUGen_JECNominal",&p_JVBF_SIG_ghv1_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal",&pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("p_HadWH_SIG_ghw1_1_JHUGen_JECNominal",&p_HadWH_SIG_ghw1_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("p_HadZH_SIG_ghz1_1_JHUGen_JECNominal",&p_HadZH_SIG_ghz1_1_JHUGen_JECNominal);
		tqqzz->SetBranchAddress("JetPhi",&jetPhi);
		tqqzz->SetBranchAddress("ZZMass",&ZZMass);
		tqqzz->SetBranchAddress("PFMET",&PFMET);
		float weight, weight_up, weight_dn;
		float weight_vbf, weight_vbf_up, weight_vbf_dn;
		float weight_inc, weight_inc_up, weight_inc_dn;
		int chan, cat, prodCate;
		int vbfcate;
		float dmassCut[9];
		TTree *tnew = new TTree("SelectedTree"+newtreename[t],"SelectedTree"+newtreename[t]);
		tnew->Branch("mreco",&ZZMass,"mreco/F");
		tnew->Branch("ZZMassErrCorr",&ZZMassErrCorr,"ZZMassErrCorr/F");
		tnew->Branch("DMass",&dmass,"DMass/F");
		tnew->Branch("dbkg_kin",&dbkg_kin, "dbkg_kin/F");
		tnew->Branch("weight",&weight,"weight/F");
		tnew->Branch("weight_up",&weight_up,"weight_up/F");
		tnew->Branch("weight_dn",&weight_dn,"weight_dn/F");
		tnew->Branch("weight_vbf",&weight_vbf,"weight_vbf/F");
		tnew->Branch("weight_vbf_up",&weight_vbf_up,"weight_vbf_up/F");
		tnew->Branch("weight_vbf_dn",&weight_vbf_dn,"weight_vbf_dn/F");
		tnew->Branch("weight_inc",&weight_inc,"weight_inc/F");
		tnew->Branch("weight_inc_up",&weight_inc_up,"weight_inc_up/F");
		tnew->Branch("weight_inc_dn",&weight_inc_dn,"weight_inc_dn/F");
		tnew->Branch("chan",&chan,"chan/I");
		tnew->Branch("vbfcate",&vbfcate,"vbfcate/I");
		tnew->Branch("cat",&cat,"cat/I");
		tnew->Branch("prodCate", &prodCate, "prodCate/I");
		string tag;
		float cconstant;
		cout<<tqqzz->GetEntries()<<endl;
		for(int i=0;i<tqqzz->GetEntries();i++){
//		for(int i=0; i<100;i++){
			tqqzz->GetEntry(i);
//			cout<<"mreco: "<<ZZMass<<" ZZMassErrCorr: "<<ZZMassErrCorr<<endl;
		//Here 1: 4e	2:4mu	3:2e2mu
	if(chan==1){dmassCut[0]=0.005741;dmassCut[1]=0.006264;dmassCut[2]=0.006838;dmassCut[3]=0.007422;dmassCut[4]=0.008004;dmassCut[5]=0.008687;dmassCut[6]=0.00965;dmassCut[7]=0.011305;dmassCut[8]=0.105962;}	
		else if (chan==2){dmassCut[0]=0.009711;dmassCut[1]=0.011752;dmassCut[2]=0.013824;dmassCut[3]=0.015895;dmassCut[4]=0.017836;dmassCut[5]=0.019855;dmassCut[6]=0.022378;dmassCut[7]=0.026321;dmassCut[8]=0.094864;}
		else{dmassCut[0]=0.007478;dmassCut[1]=0.008602;dmassCut[2]=0.009789;dmassCut[3]=0.011213;dmassCut[4]=0.012895;dmassCut[5]=0.014756;dmassCut[6]=0.016974;dmassCut[7]=0.020463;dmassCut[8]=0.135045;}

					
			weight= xsec*KFactorEWKqqZZ * overallEventWeight*KFactorQCDqqZZ_M;
//      double eff = 6.548111e-03 - 5.866523e-06*ZZMass*TMath::Gaus((ZZMass-2.432632e+02)/2.272477e+01);
      double eff = 0.0139011;
			if(t>0)
				eff=0;
			double rho = ZZPt/(LepPt->at(0)+LepPt->at(1)+LepPt->at(2)+LepPt->at(3)); 

			
			if(abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)==121){
				chan=2;
			}
			else if (abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)!=121){
				chan=1;
			}
			else{
				chan=3;		}		
			if (i%50000==0)	cout<<"now at entry : "<<i<<endl;	
			short ZZFlav = Z1Flav*Z2Flav;
			cconstant = sps[chan-1]->Eval(ZZMass);
//			cout<<cconstant<<endl;
			dbkg_kin = p0plus_VAJHU/(p0plus_VAJHU + bkg_VAMCFM*cconstant);
//			cout<<dbkg_kin<<endl;
			dmass = ZZMassErrCorr/ZZMass;
			prodCate = categoryMor17(nExtraLep,nExtraZ,nCleanedJetsPt30,nCleanedJetsPt30BTagged_bTagSF,jetQGLikelihood->data(),p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,jetPhi->data(),ZZMass,PFMET,useVHMETTagged,useQGTagging);

			if(dmass<dmassCut[0])		cat=0;
			else if(dmass<dmassCut[1])	cat=1;
			else if(dmass<dmassCut[2])      cat=2;
			else if(dmass<dmassCut[3])      cat=3;
			else if(dmass<dmassCut[4])      cat=4;
			else if(dmass<dmassCut[5])	cat=5; 
			else if(dmass<dmassCut[6])      cat=6;
			else if(dmass<dmassCut[7])      cat=7;
			else			     	cat=8;
			

			if (ZZMass==125) zz_m125->Fill(dbkg_kin,weight);

			temp_zz->Fill(ZZMass,dbkg_kin,weight);

			bool patle = true;
			if(t==2){
				for (int k=0 ; k<LepLepId->size() ; k++){ 
				if (abs(LepLepId->at(k))==22 && LepPt->at(k)<=30) 
					patle = false;
				}
//							 if(TLE_dR_Z<=1.6)
//								 patle = false;
			if(ZZFlav==29282)
				chan =2;
			else if(Z1Flav*Z2Flav==40898)
				chan =3;
			}
			if(!patle)
				continue;
			//float vbfMela = pvbf_VAJHU_old / ( phjj_VAJHU_old*0.06 + pvbf_VAJHU_old );
			float vbfMela = pvbf_VAJHU_old / ( phjj_VAJHU_old + pvbf_VAJHU_old );
			if(vbfMela>0.5  && nCleanedJetsPt30>=2)
				vbfcate=1;
			else
				vbfcate=0;

				if(rho<0.3){
					weight_dn = weight*(1-abs((KFactorQCDqqZZ_M-1)*(KFactorEWKqqZZ-1))); 
					weight_up = weight*(1+abs((KFactorQCDqqZZ_M-1)*(KFactorEWKqqZZ-1))); 
				}
				else{
					weight_dn = weight*(1-abs((KFactorEWKqqZZ-1))); 
					weight_up = weight*(1+abs((KFactorEWKqqZZ-1))); 
				}

				weight_inc= weight;
				weight_inc_dn= weight_dn;
				weight_inc_up= weight_up;

				weight_vbf= weight*eff;
				weight_vbf_dn= weight_dn*eff;
				weight_vbf_up= weight_up*eff;

				weight*= (1-eff);
				weight_dn*= (1-eff);
				weight_up*= (1-eff);

			
			tnew->Fill();
		}
//		tnew->Draw("ZZMass","weight_up","same");
//		tnew->Draw("ZZMass","weight_dn","same");

/*		TCanvas* c1 = new TCanvas();
		c1->cd();
		tqqzz->SetLineColor(2);
		tqqzz->SetMarkerColor(2);
		tqqzz->Draw("ZZMass","xsec*KFactorEWKqqZZ * overallEventWeight*KFactorQCDqqZZ_M","same");
		tqqzz->Draw("ZZMass");
		c1->Update();
		c1->SaveAs("~/www/mass_width/13TeV_36_8fb/tqqzz.png");

		TCanvas* c2 = new TCanvas();
                c2->cd();
                tnew->SetLineColor(1);
                tnew->SetMarkerColor(1);
                tnew->Draw("ZZMass");
		c2->Update();
                c2->SaveAs("~/www/mass_width/13TeV_36_8fb/tqqzz_new.png");
*/
		fnew->cd();
		zz_m125->Write();
		temp_zz->Write();	
		tnew->Write();
		}
		fnew->Close();
}
