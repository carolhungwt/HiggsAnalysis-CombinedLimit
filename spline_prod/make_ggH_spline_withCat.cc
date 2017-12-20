#include <iostream>
#include <utility>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
#include "TSystem.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "TGaxis.h"
#include "TString.h"
#include "TChain.h"
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussModel.h"
#include "RooRealIntegral.h"
#include "RooDecay.h"
#include "RooBMixDecay.h"
#include "RooCategory.h"
#include "RooBinning.h"
#include "RooRealSumPdf.h"
#include "RooPlot.h"
#include "RooNumIntConfig.h"
#include "RooWorkspace.h"
#include <ZZMatrixElement/MELA/interface/Mela.h>
#include <ZZMatrixElement/MELA/interface/ScalarPdfFactory_HVV.h>
#include "HiggsAnalysis/CombinedLimit/interface/RooNCSplineFactory_1D.h"
#include "HiggsAnalysis/CombinedLimit/interface/Width_conv.h"
#include "HiggsAnalysis/CombinedLimit/interface/Width_integral.h"
#include "HiggsAnalysis/CombinedLimit/interface/SplinePdf.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooNCSpline_2D_fast.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooNCSplineCore.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooNCSplineFactory_2D.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooNCSplineFactory_3D.h"
using namespace RooFit;
using namespace std;

//RooFormulaVar* extract_Formula(std::vector<RooRealVar*> projvars, RooWorkspace* wo, char *tag= "bkg");
RooPlot* quickplot(RooWorkspace* w, std::vector<RooAbsPdf*> pdfs, vector<RooRealVar*> projvars, unsigned int chosen, vector<float> varset, vector<unsigned int> ndim);
void plotOnC(TCanvas* c, RooPlot* plot, TString tag);
RooArgList extract_pdf(RooWorkspace* w);
void setVar(RooWorkspace* w, const int var, const float Val);
RooFormulaVar* writeFormula(RooWorkspace* w, RooRealVar* r, TString tag,int cat, int cate_vbf, int prod_cate);

void make_ggH_spline_withCat(TString tag, int cat,int quad){

	RooWorkspace* wo, *ws;
	TFile* fm, *f;
	vector<RooAbsReal*> vec_pdfs;
	RooAbsReal* temppdf;
	RooFormulaVar* tempcoeff;
	RooRealSumPdf* ggH;
	RooFormulaVar *ggH_norm;
	TString pdfname;
	RooRealVar *r= new RooRealVar("r","signal strength",1.,0.000,1000);

	TFile ftest(Form("spline_WS_withCat_quad%d/%s_spline_WS_cat%d.root",quad,tag.Data(),cat),"recreate");
	RooWorkspace* newWS = new RooWorkspace("w", "");
	newWS->addClassDeclImportDir("../interface/");
	newWS->importClassCode(RooNCSplineCore::Class(),true);
	newWS->importClassCode(RooNCSpline_2D_fast::Class(), true);
	newWS->importClassCode(RooNCSpline_3D_fast::Class(), true);

	
 	for(int cate_vbf=0; cate_vbf<3; cate_vbf++){
 		for(int prod_cate=0; prod_cate<3; prod_cate++){

	char* inputDir = "/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/signal_clean";
 	
	cout<< inputDir<<endl;
        char* tag2 = (char*) tag.Data();
        TString tag1="";
        if(cate_vbf==0)         tag1="0jet_";
        else if(cate_vbf==1)    tag1="1jet_";
        else if(cate_vbf==2)    tag1="2jet_";
        TString prodtag="";
        if(prod_cate==0)        prodtag="";
        else if(prod_cate==1)   prodtag="vbf_";
        else if(prod_cate==2)   prodtag="vh_";
    	fm = new TFile(Form("%s/workspace125_onshell/hzz%s_13TeV.input_func_%s%scat%d.root",inputDir,tag2,prodtag.Data(),tag1.Data(),cat));
	TString workdir="./rpdfWS_withCat";
	if(prod_cate==0)        prodtag="ggH_";
	f = new TFile(Form("%s/%s_rpdfWS_cat%d_%s%s.root",workdir.Data(), tag.Data(), cat,prodtag.Data(),tag1.Data()));
	//4e_rpdfWS_cat0.root
	wo = (RooWorkspace*) f->Get("w");

	ws = (RooWorkspace*) fm->Get("w");

    if(cate_vbf==0)         tag1="_untagged";
    else if(cate_vbf==1)    tag1="_vbf2j";
    else if(cate_vbf==2)    tag1="_vhhad";
	if(prod_cate==0)		pdfname="ggH";
	else if (prod_cate==1)		pdfname="qqH";
	else if (prod_cate==2)		pdfname="VH";
	pdfname += tag1;



	temppdf = (RooAbsReal*) wo->obj("r1");
	if(prod_cate==0)	temppdf->SetNameTitle("r1_ggH","r1_ggH");
	else 			temppdf->SetNameTitle("r1_qqH","r1_qqH");
	tempcoeff = writeFormula(ws,r, tag,cat,cate_vbf,prod_cate);



	
	ggH = new RooRealSumPdf(pdfname.Data(), pdfname.Data(), RooArgList(*temppdf), RooArgList(*tempcoeff)); 
	ggH_norm = new RooFormulaVar(Form("%s_norm",pdfname.Data()),Form("%s_norm",pdfname.Data()),"@0",RooArgList(*tempcoeff));	

	ftest.cd();
	newWS->import(*ggH,RooFit::RecycleConflictNodes());
	newWS->import(*ggH_norm,RooFit::RecycleConflictNodes());



	}
}
	ftest.WriteTObject(newWS);
	ftest.Close();
	newWS->Print();
	return;


}

RooFormulaVar* writeFormula(RooWorkspace* w, RooRealVar* r, TString tag,int cat, int cate_vbf, int prod_cate){
	// The following is how the coefficient is derived
	//
	//f0 = bkg_integral2e2mu0Nominal;
	//f1 = bkg_integral2e2mu0Nominal+sig_integral2e2mu0Nominal+int_integral2e2mu0Nominal;
	//f2 = bkg_integral2e2mu0Nominal+4*sig_integral2e2mu0Nominal+2*int_integral2e2mu0Nominal;

	//f0*r0 = b;
	//f1*r1 = b+s+I;
	//f2*r2 = b+4s+2I;

	//s = ( r2*f2 - 2*r1*f1 +   r0*f0)/2.;
	//I = (-r2*f2 + 4*r1*f1 - 3*r0*f0)/2.;

	//Pdf = s *r + b+ I*sqrt(r);

	//Pdf = r0 * f0 * (r/2. - 3./2.*sqrt(r)+1) + r1*f1 *(-r+2*sqrt(r)) + r2*f2*(r/2.-sqrt(r)/2.); 
	


	cout<<prod_cate<<endl;
	RooAbsReal* bkg_integral2e2mu0Nominal;

	RooAbsReal* sig_integral2e2mu0Nominal;
	RooAbsReal* int_integral2e2mu0Nominal;
	RooFormulaVar* rvbf;
	if(prod_cate==0){

		sig_integral2e2mu0Nominal= (RooAbsReal*) w->obj(Form("sig_integral%s%d_%djetNominal",tag.Data(),cat,cate_vbf)); 

		r = (RooRealVar*) w->var("r");
	}
	else if (prod_cate==1){
		cout <<Form("vbfsig_integral%s%d_%djet",tag.Data(),cat,cate_vbf)<<endl;
                sig_integral2e2mu0Nominal= (RooAbsReal*) w->obj(Form("vbfsig_integral%s%d_%djet",tag.Data(),cat,cate_vbf)); 
		rvbf = (RooFormulaVar*) w->obj("rvbf");
	}
	else if (prod_cate==2){
                sig_integral2e2mu0Nominal= (RooAbsReal*) w->obj(Form("vhsig_integral%s%d_%djet",tag.Data(),cat,cate_vbf)); 

		rvbf = (RooFormulaVar*) w->obj("rvbf");
	}

	RooFormulaVar* f1= new RooFormulaVar("f1","f1", "@0", RooArgList(*sig_integral2e2mu0Nominal));

	f1->SetNameTitle(Form("f1%s_cat%d_vbf%d_prod%d",tag.Data(),cat,cate_vbf,prod_cate),"f1"+tag);

	RooFormulaVar* f1_final;


	if(prod_cate==0){
	f1_final= new RooFormulaVar("f1_final","f1_final", "@1*@0", RooArgList(*f1,*r));
	f1_final->SetNameTitle(Form("f1_final%s_cat%d_vbf%d_prod%d",tag.Data(),cat,cate_vbf,prod_cate),Form("f1_final%s_cat%d",tag.Data(),cat));
}	else{
        f1_final= new RooFormulaVar("f1_final","f1_final", "@0*@1", RooArgList(*f1,*rvbf));
        f1_final->SetNameTitle(Form("f1_final%s_cat%d_vbf%d_prod%d",tag.Data(),cat,cate_vbf,prod_cate),Form("f1_final%s_cat%d",tag.Data(),cat));
}
	return f1_final;

}


RooArgList extract_pdf(RooWorkspace* w){
	RooArgList pdf_list;
	for(int rval=0; rval<3; rval++){
		TString pdfname = Form("r%d",rval);
		RooAbsPdf* pdf = w->pdf(pdfname);
		pdf_list.add(*pdf);
	}
	return pdf_list;
}

void plotOnC(TCanvas* c, RooPlot* plot, TString tag){
	c->SetGrid(0,1);
	c->cd();
	plot->Draw();
	c->SaveAs("~/www/test_spline_signal_"+tag+".png");
}

RooPlot* quickplot(RooWorkspace* w, std::vector<RooAbsPdf*> pdfs, vector<RooRealVar*> projvars, unsigned int chosen, vector<float> varset, vector<unsigned int> ndim){
	RooPlot* plot = projvars[chosen]->frame(ndim[chosen]-1);
	plot->GetXaxis()->CenterTitle();
    plot->GetYaxis()->SetTitleOffset(1.2);
    plot->GetYaxis()->CenterTitle();
    plot->GetXaxis()->SetTitle("M_{4l} (GeV)");//projvars.at(1)->GetTitle());
    plot->GetYaxis()->SetTitle("H4l amplitude");
    //plot->GetYaxis()->SetRangeUser(0,0.3);
    plot->GetXaxis()->SetNdivisions(-505);
    for(unsigned int i=0; i<projvars.size(); i++){
    	if (i==chosen)	continue;
    	setVar(w,i,varset[i]);
    }
    for(unsigned int k=0; k<pdfs.size(); k++){
	    pdfs.at(k)->plotOn(plot,LineColor(k), LineWidth(2), LineStyle(2));
	}
	return plot;
}



void setVar(RooWorkspace* w, const int var, const float Val){
		TString curvar = "";
		if(var==0)	curvar="mean_pole";
		if(var==1)	curvar="mreco";
		if(var==2)	curvar="sigma_pole";
		w->var(curvar)->setConstant(false);
		w->var(curvar)->setVal(Val);
		w->var(curvar)->setConstant(true);
}
