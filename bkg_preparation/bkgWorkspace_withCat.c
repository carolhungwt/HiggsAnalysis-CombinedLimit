#include "/afs/cern.ch/work/w/wahung/public/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/src/ProcessNormalization.cc"
#include "HiggsAnalysis/CombinedLimit/interface/ProcessNormalization.h"
//#include "RooTFnBinding.h"
//#include "VerticalInterpPdf.cc+"
#include "bkgWorkspace_withCat.h"
#include "RooExtendPdf.h"
#include "RooRealSumPdf.h"
#include <string>
using namespace RooFit;
//using namespace std;

// highmass = 0 onshell
// highmass = 1 offshell
// highmass = 2 search 
//
//
// cate_vbf =0 ggH
// cate_vbf =1 VBF
// cate_vbf =2 RSE
// cate_vbf =3 TLE
// cate_vbf =4 inclusive 
//

void dosomething(TString chan="2e2mu", int cate_vbf=1, int highmass=0, int cat=0){
//ClassDef(ProcessNormalization, 1);
// double lumi = 9.235;
 double lumi = 35.867;
// double lumi = 10;

	double parzx_all[5][6]={
		//4e
//		0.00399059,151.251,36.6082,7.06054,-0.00497032,
//		//4mu
//		1.84404,134.765,22.2293,0.,0.,
//		//2e2mu
//		2.57065,143.701,24.071,0.,0.,
//		4e
		1,141.9,21.3,0,0,0,
		//4mu
		1,130.4,15.6,0,0,0,
		//2e2mu
		0.45,131.1,18.1,0.55,133.8,18.9,
		//4e RSE
		0.298623315141,238.804039513,38.5525989967,4.83114145422,-0.0097489713697,0,
		//2e2mu RSE
		0.000171310428786,209.221006175,26.5346636174,11.193766044,-0.00296426709129,0
	};
	double parzx[6]={0.};

	//if(cate_vbf!=2){
	if (chan=="4e") 	{
		for (int i=0;i<6;i++){parzx[i]=parzx_all[0][i];}
		
	} 
	if (chan=="4mu")  {
		for (int i=0;i<6;i++){parzx[i]=parzx_all[1][i];}
	}
	if (chan=="2e2mu") {
		for (int i=0;i<6;i++){parzx[i]=parzx_all[2][i];}
	} 
	ofstream yields("yields.txt",std::fstream::app);

	TFile *fwork;
	
		if(highmass==0)
			fwork= new TFile("workspace_2d_bkg_onshell_withCat/hzz4l_"+chan+Form("%dS_cat%d_13TeV.input_func.root",cate_vbf,cat),"recreate");
		if(highmass==1)
			fwork= new TFile("workspace_2d_bkg_offshell/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
		if(highmass==2)
			fwork= new TFile("workspace_2d_bkg/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
		fwork->cd();
    TTree *cuttree =new TTree("SelectedTree","SelectedTree");
	RooWorkspace w("w");
  double recolowarr[3]={105.,104,104.5};
  double recohigharr[3]={140.,1604.,3004.5};
  const int reconbinsarr[3]={70,750,2900};

	const double low_reco=recolowarr[highmass];
	const double high_reco=recohigharr[highmass];
	const int nbins_reco=reconbinsarr[highmass];
	
	const char* tmpl_input_dir = "/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/TemplateBuilder";
	TFile *ftmpl = new TFile(Form("%s/Normalized_spline_tmpl_finer.root",tmpl_input_dir),"read");

		//fwork->cd();
	RooRealVar* mreco= new RooRealVar("mreco","M_{ZZ} (GeV)",125,low_reco,high_reco);
	RooRealVar* dbkg_kin = new RooRealVar("dbkg_kin", "Dbkgkin", 0.5, 0.,1.);
 	 RooPlot* frame= mreco->frame(low_reco,high_reco) ;
	RooArgSet hist_argset;
	hist_argset.add(*mreco);
	hist_argset.add(*dbkg_kin);
//if(highmass==2)
//	mreco->setBins(70);
	dbkg_kin->setBins(10);
//qqzz pdf
	//TChain *tqqzz= new TChain("ZZTree/candTree");
	
	// simplify: for ggF, this is qqZZ bkg, for VBF, this should be ggZZ bkg
	// simplify: if no systematics wanted, comment out _up and _dn related stuff
	TString treename[5]={"","","_rse","_tle",""};
	TChain *tqqzz= new TChain("SelectedTree"+treename[cate_vbf]);

	TH1F *hqqzz= new TH1F ("hqqzz","",nbins_reco,low_reco,high_reco);
	TH1F *hqqzz_up= new TH1F ("hqqzz_up","",nbins_reco,low_reco,high_reco);
	TH1F *hqqzz_dn= new TH1F ("hqqzz_dn","",nbins_reco,low_reco,high_reco);

	//tqqzz->Add("./ZZ4lAnalysis.root");
	tqqzz->Add("qqzz_withDmass_quad9.root");

		int channum=1;
		if(chan=="2e2mu")
			channum=3;
		else if(chan=="4e")
			channum=1;
		else if(chan=="4mu")
			channum=2;

	

    RooRealVar* wt= new RooRealVar("weight","wt",125,0.000,1000.);
    RooRealVar* wt_up= new RooRealVar("weight_up","wt_up",125,0.000,1000.);
    RooRealVar* wt_dn= new RooRealVar("weight_dn","wt_dn",125,0.000,1000.);


    float ZZMass;
    float weight, weight_up, weight_dn;
    int channel, icat;
    tqqzz->SetBranchAddress("mreco",&ZZMass);
    tqqzz->SetBranchAddress("cat",&icat);
		 if(cate_vbf==1){
    tqqzz->SetBranchAddress("weight_vbf",&weight);
    tqqzz->SetBranchAddress("weight_vbf_up",&weight_up);
    tqqzz->SetBranchAddress("weight_vbf_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight_vbf*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_vbf_dn*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_vbf_up*(chan==%d&&cat==%d)",channum,cat));
		 }
		 else if(cate_vbf!=4){
    tqqzz->SetBranchAddress("weight",&weight);
    tqqzz->SetBranchAddress("weight_up",&weight_up);
    tqqzz->SetBranchAddress("weight_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_dn*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_up*(chan==%d&&cat==%d)",channum,cat));
		 }
		 else {
    tqqzz->SetBranchAddress("weight_inc",&weight);
    tqqzz->SetBranchAddress("weight_inc_up",&weight_up);
    tqqzz->SetBranchAddress("weight_inc_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight_inc*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_inc_dn*(chan==%d&&cat==%d)",channum,cat));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_inc_up*(chan==%d&&cat==%d)",channum,cat));
		 }
    tqqzz->SetBranchAddress("chan",&channel);
		

if(cate_vbf!=2){
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_up->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_dn->Integral()*lumi*1000./6.635892e+06<<endl;
}
else{
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz_up->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz_dn->Integral()*lumi*1000./6.635892e+06<<endl;
}

	
 
     cuttree->Branch("mreco",&ZZMass,"mreco/F");
     cuttree->Branch("weight",&weight,"weight/F");
     cuttree->Branch("weight_up",&weight_up,"weight_up/F");
     cuttree->Branch("weight_dn",&weight_dn,"weight_dn/F");

    for(int i =0;i<tqqzz->GetEntries();i++){
        tqqzz->GetEntry(i);
	if(channel==channum && cat==icat){
        	cuttree->Fill();
	}
    }


		double rho =1;
		if(highmass==0)
			rho=4;
//******************* qqzz and Template **************************\

	TH2F* qqzzth2ftmpl = (TH2F*) ftmpl->Get(Form("qqzz_bkg_%s",chan.Data()));

    cout<<"Background expectatioon: "<<endl;
    RooDataSet bkgdata("bkgdata"+chan+Form("_%d_cat%d",cate_vbf,cat),"",cuttree,RooArgSet(*mreco,*wt),"weight");
    RooKeysPdf qqzzkeyspdf("qqzzkeyspdf"+chan+Form("_%d_cat%d",cate_vbf,cat),"",*mreco,bkgdata,RooKeysPdf::MirrorBoth,rho);
    RooDataHist qqzzdatahist("qqzzdatahist"+chan,"",hist_argset, qqzzth2ftmpl);
    RooHistPdf qqzzhistpdf("qqzzhistpdf"+chan,"",hist_argset,qqzzdatahist);

    RooDataSet bkgdata_up("bkgdata_up"+chan+Form("_%d_cat%d",cate_vbf,cat),"",cuttree,RooArgSet(*mreco,*wt_up),"weight_up");
    RooKeysPdf qqzzkeyspdf_up("qqzzkeyspdf_up"+chan+Form("_%d_cat%d",cate_vbf,cat),"",*mreco,bkgdata_up,RooKeysPdf::MirrorLeft,rho);

    RooDataSet bkgdata_dn("bkgdata_dn"+chan+Form("_%d_cat%d",cate_vbf,cat),"",cuttree,RooArgSet(*mreco,*wt_dn),"weight_dn");
    RooKeysPdf qqzzkeyspdf_dn("qqzzkeyspdf_dn"+chan+Form("_%d_cat%d",cate_vbf,cat),"",*mreco,bkgdata_dn,RooKeysPdf::MirrorLeft);

    RooProdPdf qqzzpdf("bkg_qqzz","", qqzzkeyspdf, Conditional(qqzzhistpdf,*dbkg_kin));

    RooProdPdf qqzzpdf_up("bkg_qqzz_EWKscale_VVUp","", qqzzkeyspdf_up, Conditional(qqzzhistpdf,*dbkg_kin));

    RooProdPdf qqzzpdf_dn("bkg_qqzz_EWKscale_VVDown","",qqzzkeyspdf_dn, Conditional(qqzzhistpdf,*dbkg_kin));
	
	
  RooRealVar *ewkvar=new RooRealVar("EWKscale_VV","EWKscale_VV",-7,7);
  ProcessNormalization *int_qq;
	int_qq= new ProcessNormalization("bkg_qqzz_norm","bkg_qqzz_norm",1.);
	double hqqzz_dn_int = hqqzz_dn->Integral();
	double hqqzz_up_int = hqqzz_up->Integral();
	double hqqzz_norm_int = hqqzz->Integral();
  int_qq->addAsymmLogNormal(hqqzz_dn_int/hqqzz_norm_int, hqqzz_up_int/hqqzz_norm_int, *ewkvar);
	cout << hqqzz_dn_int/hqqzz_norm_int<<" "<< hqqzz_up_int/hqqzz_norm_int<<endl;
	
	//zjet pdf

	TF1 *fsum ;
	int parsize ;
	double exp;
	 fsum=new TF1("fsum"+chan,"landau( 0 )+ landau(3)",50,3000);
	 parsize=6;
	 fsum->SetParameters(parzx);
	 if(cate_vbf!=2){
	if(chan=="4e")
	 exp=9.8*lumi/12.9;
	 else if(chan=="2e2mu")
		 exp = 20.4*lumi/12.9;
		else
			exp=10.2*lumi/12.9;
	 }
	 else{
			if(chan=="4e")
				exp = 13.2*lumi/12.9; 
			else if(chan=="2e2mu")
				exp = 18.3*lumi/12.9;
	 }

	cout <<fsum->GetExpFormula("p")<<endl;
	
	if(cate_vbf==1){
	yields<< chan<< " "<< cate_vbf<<" ZX "<< 0.0762466*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	cout<< chan<< " "<< cate_vbf<<" ZX "<< 0.0762466*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	}
	else if(cate_vbf==0){
	yields<< chan<< " "<< cate_vbf<<" ZX "<< (1-0.0762466)*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	cout<< chan<< " "<< cate_vbf<<" ZX "<< (1-0.0762466)*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	}
	else{
	cout<< chan<< " "<< cate_vbf<<" ZX "<< exp*fsum->Integral(300,high_reco)/fsum->Integral(50,3000)<<endl;
	yields<< chan<< " "<< cate_vbf<<" ZX "<< exp*fsum->Integral(300,high_reco)/fsum->Integral(50,3000)<<endl;
	}
		
		RooArgList* parlist_zx=new RooArgList("parlist_zx");
		const int parsize_const = parsize;
		RooConstVar *pars_zx[parsize_const];

				parlist_zx->add(*mreco);
				
		for (int pa=0; pa<parsize; pa++){ 
				pars_zx[pa]=new RooConstVar(Form("par_zx_w%d_%s",pa,chan.Data()),"",parzx[pa]);	
				cout << parzx[pa]<<endl;
				parlist_zx->add(*pars_zx[pa]);
			}

	//RooGenericPdf 
	TString form="@1*TMath::Landau(@0,@2,@3)+@4*TMath::Landau(@0,@5,@6)";

//********************* ZX and Template ***************************

			TFile *f_zx = new TFile(Form("%s/Normalized_ZX_tmpl_finer.root",tmpl_input_dir),"read");

                        TH2F* th2f_zx = (TH2F*) f_zx->Get("zx_tmpl");
                        RooDataHist zxdatahist("zx_datahist"+chan+Form("_%d",cate_vbf), "", hist_argset, th2f_zx);
                        RooHistPdf zxhistpdf("zx_histpdf"+chan+Form("_%d",cate_vbf),"", hist_argset, zxdatahist);

		 	RooGenericPdf* zjetpdf_1d = new RooGenericPdf("bkg_zjet_1d","",form,*parlist_zx);
			RooProdPdf *zjetpdf = new RooProdPdf("bkg_zjet","",*zjetpdf_1d,Conditional(zxhistpdf, *dbkg_kin));
			RooProdPdf *zjetpdf_up = (RooProdPdf*) zjetpdf->Clone("bkg_zjet_EWKscale_VVUp");
			RooProdPdf *zjetpdf_dn = (RooProdPdf*) zjetpdf->Clone("bkg_zjet_EWKscale_VVDown");
		//	f_zx->Close();	
//********************* data ******************************************
		TChain *tdata = new TChain("SelectedTree");
		tdata->Add("data.root");
		TString cutstring = Form("chan==%d&&vbfcate==%d",channum,cate_vbf);
		if(cate_vbf==4)
			cutstring = Form("chan==%d&&(vbfcate==0||vbfcate==1)",channum);
		TTree* reducetree= tdata->CopyTree(cutstring);
		RooDataSet* data_obs = new RooDataSet("data_obs","data_obs",reducetree,hist_argset);
		w.import(*data_obs);
		cout<<data_obs->sumEntries()<<endl;


//********************* ggZZ and Template ***************************

  double ggzz_xsec = 2.827;
  
    if (chan=="2e2mu"){
      ggzz_xsec = 0.2184*1.773674746;
    }
    else if(chan=="4mu"){
  ggzz_xsec = 0.1092*1.430496941;
    }
    else{
      ggzz_xsec = 0.1092*1.58399592;
    }

  TString pdfn = "2e2mu";
  if (chan!="2e2mu") pdfn = "4e";
  TFile* fpdfbkg = new TFile("/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/signal_bkg_related/pdfs_onshell.root","read");
  RooWorkspace* wbkg =(RooWorkspace*)fpdfbkg->Get("w");
  TString ggzzloc = "";
  RooKeysPdf* pdfbkg = (RooKeysPdf*)wbkg->pdf("pdfbkg_"+pdfn);

  double effcate = 1./9.;
  RooConstVar* ggzznorm= new RooConstVar("ggzznorm"+chan+Form("%d", cat), "", lumi*ggzz_xsec);

//*****ggzz tmpl**********
        TH2F* ggzzth2ftmpl = (TH2F*) ftmpl->Get(Form("ggzz_bkg_%s",chan.Data()));
        RooDataHist *ggzzdatahist = new RooDataHist("ggzz_datahist_"+chan, "", hist_argset, ggzzth2ftmpl);
        RooHistPdf *ggzzhistpdf = new RooHistPdf("ggzz_histpdf_"+chan,"",hist_argset,*ggzzdatahist);


//  RooRealSumPdf pdf_ggzz("pdf_ggzz"+chan+Form("%d", cat), "pdf_ggzz"+chan+Form("%d", cat), RooArgList(*pdfbkg), RooArgList(*ggzznorm));
  RooDataSet* ggzz_datset = (RooDataSet*) pdfbkg->generate(RooArgSet(*mreco),2500);
  ggzz_datset->SetName("ggzz_dataset_"+chan);
  RooKeysPdf* pdf_ggzz_1d = new RooKeysPdf("bkg_ggzz_1d","",*mreco,*ggzz_datset);
  RooProdPdf *pdf_ggzz = new RooProdPdf("bkg_ggzz","",*pdf_ggzz_1d,Conditional(*ggzzhistpdf,*dbkg_kin));


  	w.import(*pdf_ggzz,RecycleConflictNodes());
	w.import(qqzzpdf,RecycleConflictNodes());
	w.import(*int_qq,RecycleConflictNodes());
	w.import(*zjetpdf,RecycleConflictNodes());



		fwork->cd();
		w.Write();
		fwork->WriteTObject(hqqzz);
		fwork->WriteTObject(hqqzz_up);
		fwork->WriteTObject(hqqzz_dn);
		hqqzz->Draw();
		fwork->Close();

}


void bkgWorkspace_withCat(TString chan, int vbfcate, int highmass,int cat){	 
       gSystem->AddIncludePath("-I$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/interface/");
    gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
    gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
    gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");
		dosomething(chan,vbfcate,highmass,cat);
}
