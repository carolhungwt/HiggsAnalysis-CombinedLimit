
TH2F* norm_th2f(TH2F* pdf){
	int nxbin, nybin;
	nxbin = pdf->GetXaxis()->GetNbins();
	nybin = pdf->GetYaxis()->GetNbins();
	int nxfirst, nxlast, nyfirst, nylast;
	nxfirst = pdf->GetXaxis()->GetFirst();
	nxlast = pdf->GetXaxis()->GetLast();
	nyfirst = pdf->GetYaxis()->GetFirst();
	nylast = pdf->GetYaxis()->GetLast();	

	TH2F* newpdf = (TH2F*) pdf->Clone();

	float newCon, oldCon, temp; 
	int gBin;
	int tracker =0;
//	vector<float> weights(nxbin, 0.);
	float weights;
	std::vector<std::vector<float>> binContent(nxbin+1, vector<float>(nybin+1,0.));
	//first loop over x then loop over y
	for(int nx=0; nx<nxlast+1; nx++){
		weights=0.;
		for(int ny=nyfirst; ny<nylast+1; ny++){
			gBin = pdf->GetBin(nx, ny, 0);
			oldCon=pdf->GetBinContent(gBin);
//			cout<<"Bin: ("<<nx<<" ,"<<ny<<") And gBin: "<<gBin<<"   Bin Content: "<<oldCon<<endl;
//			binContent[nx][ny]=oldCon;
			weights+=oldCon;
		}

	//	cout<<"Weight:   "<<weights<<endl<<endl;
		float checknorm=0.;
		for(int ny=nyfirst; ny<nylast+1; ny++){
			gBin = pdf->GetBin(nx, ny);
			oldCon=pdf->GetBinContent(gBin);
			temp = oldCon/weights;	
			if(temp==0)	temp=0.000001;
			checknorm+=temp;
			newpdf->SetBinContent(gBin, temp);
//			cout<<"Bin: ("<<nx<<" ,"<<ny<<") And gBin: "<<gBin<<"   Bin Content: "<<newpdf->GetBinContent(gBin)<<endl;
		}
	//	cout<<checknorm<<endl<<endl;
	}
return newpdf;
}

void Normalize_TH2_all(TString rootfile){
	TFile *f1 = new TFile(Form("/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/TemplateBuilder/%s",rootfile.Data()));
	TFile* fnew = new TFile(Form("Normalized_%s",rootfile.Data()),"recreate");
	gStyle->SetOptStat(0);
	TCanvas* c1 = new TCanvas("c1","c1",1000, 800);
	vector<TString> tags={"4e","2e2mu","4mu"};
	vector<TString> subprocesses = {"bkg","sig"};
	vector<TString> processes = {"ggzz","qqzz","vbf"};
	for(int tag=0; tag<3; tag++){
	for(int subprocess=0; subprocess<2; subprocess++){
		for(int process=0; process<3; process++){
		if(subprocess==1&&process==1)	continue;
		const char* tempname = Form("%s_%s_%s",processes[process].Data(),subprocesses[subprocess].Data(),tags[tag].Data());
		TH2F* th2_temp = (TH2F*) f1->Get(tempname);
		th2_temp = norm_th2f(th2_temp);
//		if(process==0 && tag==1) th2_temp->SetName(Form("%s_%s_%s",processes[process].Data(),subprocesses[subprocess].Data(),tags[tag].Data()));
		th2_temp->Draw("colz");
		fnew->cd();
		fnew->WriteTObject(th2_temp);
	c1->SaveAs(Form("~/www/Mass_Width_Analysis/tmpl_check/20171128_template_builder_%s_normalized_rew.png",tempname));
	c1->SaveAs(Form("~/www/Mass_Width_Analysis/tmpl_check/20171128_template_builder_%s_norrmalized_rew.pdf",tempname));
	}
	}
	}

f1->Close();
fnew->Close();
} 
