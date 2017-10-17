//template_spline.cc

#include "template_spline_withCat.h"


void template_spline_withCat(){
	gSystem->AddIncludePath("-I$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/interface/");
	gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
	gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
	gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");
	
		
	for(int i=0; i<sizeof(ch_list)/sizeof(ch_list[0]); i++){
		for(int cat=0; cat<3; cat++){
//		hist_template = extract_templates(hist_input);	
		TFile *f_spline = new TFile(Form(spline_input+"%s_spline_WS_cat%d.root", (ch_list[i]).c_str(),cat));
		RooWorkspace *ws = (RooWorkspace*) f_spline->Get("w");
//		spline_template[i] = extract_splines(ws, ch_list[i]);

//************* Collection of coeff list **********************************

	RooRealVar* r = (RooRealVar*) ws->var("r");
	r->setRange(0.,200.);

//		coeff_list[i] = extract_coeff(ws, ch_list[i]);
         RooArgList form_list;
         RooFormulaVar* temp_form;
	 cout<<temp_form<<endl;
         temp_form = (RooFormulaVar*) ws->obj(Form("f0_final%s_cat%d", (ch_list[i]).c_str(),cat));
	 cout<<temp_form<<endl;
         form_list.add(*temp_form);
         temp_form = (RooFormulaVar*) ws->obj(Form("f1_final%s_cat%d", (ch_list[i]).c_str(),cat));
         form_list.add(*temp_form);
         temp_form = (RooFormulaVar*) ws->obj(Form("f2_final%s_cat%d", (ch_list[i]).c_str(),cat));
         form_list.add(*temp_form);
	form_list.Print();
	coeff_list.push_back(form_list);

//************** Collect TH2Fs as RooArgList Input ****************************
//		pdf_list[i] = prod_pdf(ws, hist_input, ch_list[i], i);	
        TFile f_hist(Form("%s/%s_templates_withCut_checkDMass_cat%d_Normalized.root",hist_input.Data(),(ch_list[i]).c_str(),cat));
//        vector<TH2F*> th2fs(3);
	for(int j=0; j<3; j++){
	        th2fs[i][j] = (TH2F*) f_hist.Get(Form("r%d_template",j));
	}
//******************************************************************************


	TCanvas *c = new TCanvas("","");
        RooRealVar mreco("mreco", "mreco", 105., 140.);
	mreco.setBins(35);
        RooRealVar dbkg("dbkg_kin", "DBkgKin", 0.,1.);
	dbkg.setBins(10);

//****************** Collect splines as RooArgList Input ************************************
        spline_template[i] = extract_splines(ws, ch_list[i],cat);
        int sp_size = spline_template[i].size();

//******************************************************************************************
//

        vector<RooDataHist*> datahists;
        RooDataHist* datahist_temp;
        vector<RooHistPdf*> histpdfs;
        RooHistPdf* histpdf_temp;
        RooArgSet argset(mreco, dbkg);  
        cout<< th2fs[i].size()<<endl;
        cout<<th2fs[i][0]->Integral()<<endl;

        for(int j=0; j<sp_size; j++){
                cout<<"j: "<<j<<endl;
		
                datahist_temp = new RooDataHist(Form("datahist_r%d_%s",j,(ch_list[i]).c_str()),"",argset, th2fs[i][j]);    
                datahists.push_back(datahist_temp);
                histpdf_temp = new RooHistPdf("histpdf", "", argset, *datahists[j]);
		histpdf_temp->SetNameTitle(Form("%s_r%d_cat%d_template",(ch_list[i]).c_str(),j,cat),Form("%s_r%d_cat%d_template",(ch_list[i]).c_str(),j,cat));
		
                histpdfs.push_back(histpdf_temp);

        }   

//        RooArgList prods;
        RooProdPdf* prod_temp;
        if(sp_size==th2fs[i].size()){
cout<<"*********************************"<<endl;
cout<<"cat: "<<cat<<endl;
cout<<"*********************************"<<endl;

                for(int j=0; j<sp_size; j++){
                        prod_temp = new RooProdPdf(Form("prodPdf_%s_r%d_cat%d", (ch_list[i]).c_str(), j,cat),Form("prodPdf_%s_r%d_cat%d", (ch_list[i]).c_str(), j,cat), *(spline_template[i][j]), Conditional(*(histpdfs[j]),dbkg));
			prod_temp->SetNameTitle(Form("prodPdf_%s_r%d_cat%d", (ch_list[i]).c_str(), j,cat),Form("prodPdf_%s_r%d_cat%d", (ch_list[i]).c_str(), j,cat));
                        prods[i][cat].add(*prod_temp);
                }
        }
        else cout<< "Error between the dimension of the two pdf vectors         sp_size: "<<sp_size<<"   th2fs[i].size: "<<th2fs[i].size()<<endl;

	prods[i][0].Print();
	prods[i][1].Print();
	coeff_list[i].Print();	
//	pdf_list[i][cat]=prods[i][cat];
//	pdf_list[i][cat].Print();

//	pdf_list[i]=*prods;
//**************** Ending pdf_list collection **********************

		pdf_list[i][cat].Print();
		coeff_list[i].Print();
		HZZ4L_onshell_2D* ggH = new HZZ4L_onshell_2D("ggH", "ggH", mreco,dbkg,prods[i][cat], coeff_list[i]);	
	        RooFormulaVar *ggH_norm = new RooFormulaVar("ggH_norm","ggH_norm","@0+@1+@2",coeff_list[i]);
		
//	return;
		TFile *f= new TFile(Form("2d_spline_WS_withCat/%s_2d_spline_WS_cat%d.root",(ch_list[i]).c_str(),cat), "recreate");
		f->cd();
		RooWorkspace* newWS = new RooWorkspace("w", "");
        	newWS->addClassDeclImportDir("../interface/");
        	newWS->importClassCode(RooNCSplinePdfCore::Class(),true);
    		newWS->importClassCode(RooNCSplinePdf_3D_fast::Class(), true);
        	newWS->import(*ggH,RooFit::RecycleConflictNodes());
        	newWS->import(*ggH_norm,RooFit::RecycleConflictNodes());
       		f->WriteTObject(newWS);
        	f->Close();

        	newWS->Print();
	}


	return;
	}
}



RooArgList extract_coeff(RooWorkspace* w, string tag){
	 RooArgList form_list;
	 RooFormulaVar* temp_form;
	 temp_form = (RooFormulaVar*) w->obj(Form("f0_final%s", tag.c_str()));
	 form_list.add(*temp_form);
	 temp_form = (RooFormulaVar*) w->obj(Form("f1_final%s", tag.c_str()));
	 form_list.add(*temp_form);
	 temp_form = (RooFormulaVar*) w->obj(Form("f2_final%s", tag.c_str()));
	 form_list.add(*temp_form);

	 return form_list;

}


vector<RooNCSplinePdf_3D_fast*> extract_splines(RooWorkspace* w, string channel,int cat){
	vector<RooNCSplinePdf_3D_fast*> splines;		// splines here are referring to rs
	RooNCSplinePdf_3D_fast* sp_temp;
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf(Form("r0_%s_cat%d",channel.c_str(), cat));
	splines.push_back(sp_temp);
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf(Form("r1_%s_cat%d",channel.c_str(), cat));
	splines.push_back(sp_temp);
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf(Form("r2_%s_cat%d",channel.c_str(), cat));
	splines.push_back(sp_temp);
	for(int i=0; i<splines.size(); i++){
		cout<<splines[i]<<endl;
		splines[i]->SetNameTitle(Form("spline_%s_r%d_cat%d", channel.c_str(), i,cat),Form("spline_%s_r%d_cat%d", channel.c_str(), i,cat));
	}
	cout<<"spline vector size: "<< splines.size()<<endl;
	return splines; 
}

vector<TH2F*> extract_templates(TString input_file){
	TFile f(input_file);
	//TTree* t1 = f.Get("t1");
	vector<TH2F*> th2fs;
	TH2F* temp_th2f;
	temp_th2f = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r0");
	th2fs.push_back(temp_th2f);
	temp_th2f = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r1");
	th2fs.push_back(temp_th2f);
	temp_th2f = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r2");
	th2fs.push_back(temp_th2f);

	return th2fs;
}

RooArgList prod_pdf(RooWorkspace* w, TString input_file, string ch, int i, int cat){
	
	RooArgList blank("blank_arg_list");
//	vector<TH2F*> th2fs = (vector<TH2F*>) extract_templates(input_file);
	TFile f(input_file);
        vector<TH2F*> th2fs(3);
        th2fs[0] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r0");
        th2fs[1] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r1");
        th2fs[2] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r2");

	RooRealVar mreco("mreco", "mreco", 105., 140.);
	RooRealVar dbkg("DBkgKin", "DBkgKin", 0.,1.);
	spline_template[i] = extract_splines(w, ch, cat);
	int sp_size = spline_template[i].size();
	vector<RooDataHist*> datahists;
	RooDataHist* datahist_temp;
	vector<RooHistPdf*> histpdfs;
	RooHistPdf* histpdf_temp;
	RooArgSet argset(mreco, dbkg);	
	cout<< th2fs.size()<<endl;
	cout<<th2fs[0]->Integral()<<endl;

	for(int j=0; j<sp_size; j++){
		cout<<"j: "<<j<<endl;
/*		TCanvas *c = new TCanvas("","");
		th2fs[j]->Draw("colz");
		c->SaveAs("~/www/check_th2fs.png");
*/		datahist_temp = new RooDataHist("datahist","",argset, th2fs[j]);	
		datahists.push_back(datahist_temp);
		histpdf_temp = new RooHistPdf("histpdf", "", argset, *datahists[j]);
		histpdfs.push_back(histpdf_temp);
	}

	RooArgList prods;
	RooProdPdf* prod_temp;
	if(sp_size==th2fs.size()){
		for(int j=0; j<sp_size; j++){
			prod_temp = new RooProdPdf(Form("prodPdf_%s_r%d", ch.c_str(), j),Form("prodPdf_%s_r%d", ch.c_str(), j), *(spline_template[i][j]), Conditional(*(histpdfs[j]),dbkg));
			prods.add(*prod_temp);
		}
	}	
	else cout<< "Error between the dimension of the two pdf vectors		sp_size: "<<sp_size<<"   th2fs.sizei: "<<th2fs.size()<<endl;
	
	prods.Print();
	return prods;
	
}
