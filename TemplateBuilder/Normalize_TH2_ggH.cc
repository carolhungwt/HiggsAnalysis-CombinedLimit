using namespace std;
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
			if(ny==nylast)		cout<<"column norm:\t:"<<checknorm<<endl;
//			cout<<"Bin: ("<<nx<<" ,"<<ny<<") And gBin: "<<gBin<<"   Bin Content: "<<newpdf->GetBinContent(gBin)<<endl;
		}
	//	cout<<checknorm<<endl<<endl;
	}
return newpdf;
}

void Normalize_TH2_ggH(){
	TFile *f1 = new TFile("/afs/cern.ch/user/w/wahung/work/public/CombineLimitDbkgkin/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/TemplateBuilder/ZX_tmpl_finer.root");
	TFile *fnew = new TFile("Normalized_ZX_tmpl_finer.root","recreate");
	TCanvas* c1 = new TCanvas("c1","c1",1000, 800);

		TH2F* th2_temp = (TH2F*) f1->Get("zx_tmpl");
		th2_temp = norm_th2f(th2_temp);
		th2_temp->Draw("colz");
		fnew->WriteTObject(th2_temp);	

	c1->SaveAs("~/www/Mass_Width_Analysis/tmpl_check/20171121_template_builder_tmpls_zx_normalized.png");
	c1->SaveAs("~/www/Mass_Width_Analysis/tmpl_check/20171121_template_builder_tmpls_zx_norrmalized.pdf");



} 
