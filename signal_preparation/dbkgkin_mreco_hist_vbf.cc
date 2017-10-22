//dbkgkin_mreco_hist.cc
//#include "TrimTree_2D_Limit.h"
#include "dbkgkin_mreco_hist_vbf.h"
using namespace std;
using namespace RooFit;

TH2F* norm_th2f_fast(TH2F* pdf);

void dbkgkin_mreco_hist_no_weight_vbf(char* tag){
//	TString outputFile = "cond_hist_withWeights.root";
//	TChain tc("t1");
//	tc.Add("cond_hist.root");
	TFile f1(Form("%s/%s.root",workdir,outputFile));
	TTree* t1 = (TTree*) f1.Get("t1");

	RooRealVar mreco("mreco", "ZZMass", 125., 105., 140.);
	mreco.setBins(35);
	RooRealVar dbkgkin("dbkg_kin", "dbkg_kin", 0.,1.);
	dbkgkin.setBins(10);
	RooRealVar Dmass("DMass", "ZZMass/ZZMassErrCorr", 0., 1.);
	RooArgSet argset(mreco, dbkgkin);

	RooDataSet dataset("hist_dbkgkin_mreco", "hist_dbkgkin_mreco", t1, argset);

	hist2ds = Extract_all_hist_2d(t1,tag);
	cout<<hist2ds.size()<<endl;

	TFile fnew(Form("dbkgkin_mreco_WS_%s_%s_vbf.root",outputFile,tag), "recreate");
	RooWorkspace *ws = new RooWorkspace("w", "");
	ws->import(mreco, RooFit::RecycleConflictNodes());
	ws->import(dbkgkin, RooFit::RecycleConflictNodes());
	ws->import(Dmass, RooFit::RecycleConflictNodes());


	RooDataHist* datahist_temp;
	vector<RooDataHist*> datahists;
	RooHistPdf* histpdf_temp;
	vector<RooHistPdf*> histpdfs;
	for(int i=0; i<hist2ds.size(); i++){
	datahist_temp = new RooDataHist(Form("datahist_2d_r%d_%s_vbf",i,tag),Form("datahist_2d_r%d_%s_vbf",i,tag), argset, hist2ds[i]);
//	RooHistFunc *datahistfunc = new RooHistFunc("2dhistfunc","2dhistfunc", argset, *datahist);
	histpdf_temp = new RooHistPdf(Form("histpdf_2d_r%d_%s_vbf",i,tag), Form("histpdf_2d_r%d_%s_vbf",i,tag), argset, *datahist_temp);

	datahists.push_back(datahist_temp);
	histpdfs.push_back(histpdf_temp);

	ws->import(*datahists[i], RooFit::RecycleConflictNodes());
//	ws->import(*datahistfunc, RooFit::RecycleConflictNodes());
	ws->import(*histpdfs[i], RooFit::RecycleConflictNodes());
	//ws->Print();
	}

//	TFile fnew("dbkgkin_mreco_hist2d_only.root","recreate");
	ws->Print();
	TCanvas c1("","",1000, 800);
	gStyle->SetOptStat(0);
	c1.Divide(2,2);
	for(int i=0; i<hist2ds.size(); i++){
	c1.cd(i+1);
	hist2ds[i]->GetYaxis()->SetTitle("DBkgkin");
	hist2ds[i]->GetXaxis()->SetTitle("M_{4l} (GeV)");
	hist2ds[i]->Draw("colz");

	fnew.WriteTObject(hist2ds[i]);
//	fnew.Write();
	}

	fnew.WriteTObject(ws);
	fnew.Close();
/*//	mreco_hist->GetXaxis()->SetTitle("M_{4l} (GeV)");
//	mreco_hist->Draw();
	frame2->GetXaxis()->SetTitle("M_{4l} (GeV)");
	frame2->Draw();
	c1.cd(3);
	frame1->GetXaxis()->SetTitle("M_{4l} (GeV)");
	frame1->Draw();
	c1.cd(4);
	RooPlot *frame3 = dbkgkin.frame();
	datahistpdf->plotOn(frame3);//, projWData(
	frame3->Draw();
*/	c1.SaveAs(Form("~/www/hist_2d_test_%s_vbf.png",tag));
	return;
}


vector<TH2F*> Extract_all_hist_2d(TTree *t1, char *tag){

	t1->SetBranchAddress("mreco", &zzmass);
	t1->SetBranchAddress("DBkgKin", &dbkg);
	t1->SetBranchAddress("weightH", &weightH);
	t1->SetBranchAddress("weightHBI", &weightHBI);
	t1->SetBranchAddress("weight2", &weight2);
	t1->SetBranchAddress("Channel", &chan);
	TH2F *hist_temp_0 = new TH2F("2d_hist_r0" ,"2d_hist_r0", xbin, xmin, xmax, ybin, 0., 1.);	// Bkg
	TH2F *hist_temp_1 = new TH2F("2d_hist_r1" ,"2d_hist_r1", xbin,xmin, xmax, ybin, 0.,1.);		// BSI
	TH2F *hist_temp_2 = new TH2F("2d_hist_r2" ,"2d_hist_r2", xbin,xmin, xmax, ybin, 0.,1.);		// 4S+B+2I
//	t1->SetBranchAddress("mreco", &m4l);

	string strtag(tag);
	int chosen;
	if(strtag=="4mu")		chosen = 1;
	else if(strtag=="4e")		chosen = 2;
	else if(strtag=="2e2mu")	chosen = 3;
	else {
		cout<<"Check tag name!!!"<<endl;		
	//	return;
	}
	int count = 0 ;
	for(int i=0; i<t1->GetEntries(); i++){
		t1->GetEntry(i);
		if(chan != chosen)				continue;
		if(zzmass>140.||zzmass<105.||weight2>0.001)	continue;
		hist_temp_0->Fill(zzmass, dbkg, weight2);
		hist_temp_1->Fill(zzmass, dbkg, weightHBI);
//		hist_temp_2->Fill(zzmass, dbkg, weightH);
		hist_temp_2->Fill(zzmass, dbkg, 2*weightHBI+2*weightH-weight2);
		count++;
	}
	cout<< "Now processing channel "<<strtag<<"("<<chosen<<") "<<" with a total number of entry: "<<count<<endl;
//	TH2F *hist_r0 = hist_temp_0; 

	
	TH2F *hist_r0 = norm_th2f(hist_temp_0);
	TH2F *hist_r1 = norm_th2f(hist_temp_1);
	TH2F* hist_r2 = norm_th2f(hist_temp_2);
//	TH2F *hist_int = extract_int(hist_bkg, hist_sig, hist_bsi, xmin, xmax);
	hist_r0->SetNameTitle("hist_2d_mreco_dbkgkin_r0","hist_2d_mreco_dbkgkin_r0");
//	hist_r0_fast->SetNameTitle("hist_2d_mreco_dbkgkin_sig_fast","hist_2d_mreco_dbkgkin_sig_fast");
	hist_r1->SetNameTitle("hist_2d_mreco_dbkgkin_r1", "hist_2d_mreco_dbkgkin_r1");
	hist_r2->SetNameTitle("hist_2d_mreco_dbkgkin_r2", "hist_2d_mreco_dbkgkin_r2");
//	hist_r2->SetNameTitle("hist_2d_mreco_dbkgkin_sig", "hist_2d_mreco_dbkgkin_sig");
	vector<TH2F*> hist2ds;
	hist2ds.push_back(hist_r0);
//	hist2ds.push_back(hist_r0_fast);
	hist2ds.push_back(hist_r1);
	hist2ds.push_back(hist_r2);
//	hist2ds.push_back(hist_int);

	return hist2ds;
}
/*
TH2F* extract_int(TH2F* bkgPdf, TH2F* sigPdf, TH2F* bsiPdf, const float xmin, const float xmax){
	vector<int> binDim = getDim(bkgPdf);
	for(int i=0;i<6; i++){
		binDim[i]=binDim[i+6];
	}
	cout<<endl;
	TH2F* intPdf = new TH2F("hist_2d_mreco_dbkgkin_int", "hist_2d_mreco_dbkgkin_int", binDim[0], xmin, xmax, binDim[3], 0., 1.);
	vector<vector<float>> intCon(binDim[0], vector<float>(binDim[3],0.));
	float sigCon, bkgCon, bsiCon, temp;
	int globalBin;
	for(int nx=binDim[1]; nx<binDim[2]+1; nx++){
		for(int ny=binDim[4]; ny<binDim[5]+1; ny++){
			globalBin = bkgPdf->GetBin(nx,ny);
			sigCon = sigPdf->GetBinContent(globalBin);
			bkgCon = bkgPdf->GetBinContent(globalBin);
			bsiCon = bsiPdf->GetBinContent(globalBin);
			temp = bsiCon-sigCon-bkgCon;
			if(temp<0)		temp=0;
			intCon[nx][ny] = temp;
			intPdf->SetBinContent(nx, ny, temp);
		}
	}
	return intPdf;

}
*/
vector<int> getDim(TH2F* pdf){
	vector<int> binDim(6);	//[nxbin, nxfirst, nxlast, nybin, nyfirst, nylast]
	binDim.push_back(pdf->GetXaxis()->GetNbins());
	binDim.push_back(pdf->GetXaxis()->GetFirst());
	binDim.push_back(pdf->GetXaxis()->GetLast());
	binDim.push_back(pdf->GetYaxis()->GetNbins());
	binDim.push_back(pdf->GetYaxis()->GetFirst());
	binDim.push_back(pdf->GetYaxis()->GetLast());
	return binDim;
}

TH2F* norm_th2f_fast(TH2F* pdf){
	int nxbin, nybin;
	nxbin = pdf->GetXaxis()->GetNbins();
	nybin = pdf->GetYaxis()->GetNbins();
	int nxfirst, nxlast, nyfirst, nylast;
	nxfirst = pdf->GetXaxis()->GetFirst();
	nxlast = pdf->GetXaxis()->GetLast();
	nyfirst = pdf->GetYaxis()->GetFirst();
	nylast = pdf->GetYaxis()->GetLast();	

	TH2F* th2f_fast_temp = new TH2F("hist_temp_fast", "hist_temp_fast", xbin, xmin, xmax, ybin, 0., 1.);

	for(int nx=nxfirst; nx<nxlast+1; nx++){
		double intx = pdf->Integral(nx, nx, nyfirst, nylast);
		for(int ny=nyfirst; ny<nylast+1; ny++){
			int globalBin = pdf->GetBin(nx, ny);
			double binCon = pdf->GetBinContent(globalBin);
			double newbinCon = binCon/intx;
			th2f_fast_temp->SetBinContent(globalBin, newbinCon);
		}
	}	

	TCanvas c;
	gStyle->SetOptStat(0);
	c.cd();
//	th2f_fast_temp->Draw("colz");
	c.Update();
	string str(pdf->GetName());
//	c.SaveAs(Form("~/www/%s_fast.png",pdf->GetName()));

	return th2f_fast_temp;

}

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

/*	for(int i=0; i<nxbin;i++){
		cout<<weights[i]<<" ";
	}
		cout<<endl;
*/

/*	try{
		if(tracker!=(nxbin*nybin))
		throw 111;
	}
	catch(int n){	cout<<"=========================="<<endl;
			cout<<"Tracker: "<< tracker<<"   nx*ny: "<< nxbin*nybin<<endl;
			cout<<"Error "<<n<<"!!!"<<endl;
			cout<<"=========================="<<endl;}
*//*	TCanvas c;
	gStyle->SetOptStat(0);
	c.cd();
	newpdf->Draw("colz");
	c.Update();
	string str(pdf->GetName());
	c.SaveAs(Form("~/www/%s.png",pdf->GetName()));
*/
//	pdf = newpdf;
//	return 1;
	return newpdf;

}
