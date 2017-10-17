//template_spline.cc

#include "template_spline.h"


void template_spline(){
	gSystem->AddIncludePath("-I$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/interface/");
	gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
	gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
	gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");
	
	for(int i=0; i<sizeof(ch_list)/sizeof(ch_list[0]); i++){
//	for(int i=2; i<3; i++){
//		hist_template = extract_templates(hist_input);	
		TFile *f_spline = new TFile(Form(spline_input+"%s_spline_WS.root", (ch_list[i]).c_str()));
		RooWorkspace *ws = (RooWorkspace*) f_spline->Get("w");
//		spline_template[i] = extract_splines(ws, ch_list[i]);

//************* Collection of coeff list **********************************

	RooRealVar* r = (RooRealVar*) ws->var("r");
	r->setRange(0.,200.);

//		coeff_list[i] = extract_coeff(ws, ch_list[i]);
         RooArgList form_list;
         RooFormulaVar* temp_form;
	 cout<<temp_form<<endl;
         temp_form = (RooFormulaVar*) ws->obj(Form("f0_final%s", (ch_list[i]).c_str()));
	 cout<<temp_form<<endl;
         form_list.add(*temp_form);
         temp_form = (RooFormulaVar*) ws->obj(Form("f1_final%s", (ch_list[i]).c_str()));
         form_list.add(*temp_form);
         temp_form = (RooFormulaVar*) ws->obj(Form("f2_final%s", (ch_list[i]).c_str()));
         form_list.add(*temp_form);
	form_list.Print();
	coeff_list.push_back(form_list);

//************** Ending Collection of coeff list **************************

//return;

//************** Collect pdf_list as RooArgList ****************************
//		pdf_list[i] = prod_pdf(ws, hist_input, ch_list[i], i);	
        TFile f_hist(hist_input+ch_list[i]+".root");
//      
        th2fs[i][0] = (TH2F*) f_hist.Get("hist_2d_mreco_dbkgkin_r0");
        th2fs[i][1] = (TH2F*) f_hist.Get("hist_2d_mreco_dbkgkin_r1");
        th2fs[i][2] = (TH2F*) f_hist.Get("hist_2d_mreco_dbkgkin_r2");
TCanvas *c = new TCanvas("","");
        RooRealVar mreco("mreco", "mreco", 105., 140.);
	mreco.setBins(35);
        RooRealVar dbkg("dbkg_kin", "DBkgKin", 0.,1.);
	dbkg.setBins(10);
        spline_template[i] = extract_splines(ws, ch_list[i]);
        int sp_size = spline_template[i].size();
        vector<RooDataHist*> datahists;
        RooDataHist* datahist_temp;
        vector<RooHistPdf*> histpdfs;
        RooHistPdf* histpdf_temp;
        RooArgSet argset(mreco, dbkg);  
        cout<< th2fs[i].size()<<endl;
        cout<<th2fs[i][0]->Integral()<<endl;

        for(int j=0; j<sp_size; j++){
                cout<<"j: "<<j<<endl;
		
		th2fs[i][j]->Draw("colz");
		c->SaveAs(Form("~/www/170627_check_th2f_r%d_before_datahist.png",j));


                datahist_temp = new RooDataHist(Form("datahist_r%d_%s",j,(ch_list[i]).c_str()),"",argset, th2fs[i][j]);    
                datahists.push_back(datahist_temp);
                histpdf_temp = new RooHistPdf("histpdf", "", argset, *datahists[j]);
		histpdf_temp->SetNameTitle(Form("r%d_template_%s",j,(ch_list[i]).c_str()),Form("r%d_template_%s",j,(ch_list[i]).c_str()));

                histpdfs.push_back(histpdf_temp);
	
		
		datahist_temp = (RooDataHist*) histpdf_temp->generateBinned(argset,100000);
//                TH2* th2temp = (TH2*) datahist_temp->createHistogram("mreco,dbkg_kin",33,10);
		datahist_temp->Draw("colz");
                c->SaveAs(Form("~/www/170702_check_th2f_r%d_%s_after_datahist.png",j,(ch_list[i]).c_str()));


        }   

//        RooArgList prods;
        RooProdPdf* prod_temp;
        if(sp_size==th2fs[i].size()){
                for(int j=0; j<sp_size; j++){
                        prod_temp = new RooProdPdf(Form("prodPdf_%s_r%d", (ch_list[i]).c_str(), j),Form("prodPdf_%s_r%d", (ch_list[i]).c_str(), j), *(spline_template[i][j]), Conditional(*(histpdfs[j]),dbkg));
                        prods[i].add(*prod_temp);
                }
        }
        else cout<< "Error between the dimension of the two pdf vectors         sp_size: "<<sp_size<<"   th2fs[i].size: "<<th2fs[i].size()<<endl;

//	prods.Print();
	pdf_list.push_back(prods[i]);
//	pdf_list[i]=*prods;
//**************** Ending pdf_list collection **********************

		pdf_list[i].Print();
		coeff_list[i].Print();
//		RooRealSumPdf* ggH = new RooRealSumPdf("ggH", "ggH", pdf_list[i], coeff_list[i]);	
		HZZ4L_onshell_2D* ggH = new HZZ4L_onshell_2D("ggH", "ggH", mreco, dbkg,pdf_list[i], coeff_list[i]);	
	        RooFormulaVar *ggH_norm = new RooFormulaVar("ggH_norm","ggH_norm","@0+@1+@2",coeff_list[i]);
		
//	return;
		TFile *f= new TFile(Form("%s_2d_spline_WS.root",(ch_list[i]).c_str()), "recreate");
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


vector<RooNCSplinePdf_3D_fast*> extract_splines(RooWorkspace* w, string channel){
	vector<RooNCSplinePdf_3D_fast*> splines;		// splines here are referring to rs
	RooNCSplinePdf_3D_fast* sp_temp;
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf("r0");
	splines.push_back(sp_temp);
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf("r1");
	splines.push_back(sp_temp);
	sp_temp = (RooNCSplinePdf_3D_fast*) w->pdf("r2");
	splines.push_back(sp_temp);
	for(int i=0; i<splines.size(); i++){
		cout<<splines[i]<<endl;
		splines[i]->SetNameTitle(Form("spline_%s_r%d", channel.c_str(), i),Form("spline_%s_r%d", channel.c_str(), i));
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

RooArgList prod_pdf(RooWorkspace* w, TString input_file, string ch, int i){
	RooArgList blank("blank_arg_list");
//	vector<TH2F*> th2fs = (vector<TH2F*>) extract_templates(input_file);
	TFile f(input_file);
        vector<TH2F*> th2fs(3);
        th2fs[0] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r0");
        th2fs[1] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r1");
        th2fs[2] = (TH2F*) f.Get("hist_2d_mreco_dbkgkin_r2");

	RooRealVar mreco("mreco", "mreco", 105., 140.);
	RooRealVar dbkg("DBkgKin", "DBkgKin", 0.,1.);
	spline_template[i] = extract_splines(w, ch);
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

