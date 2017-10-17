//Normalize_TH2.cc
#include <string>
#include "Normalize_TH2.h"
using namespace std;
using namespace RooFit;

TH2F* norm_th2f_fast(TH2F* pdf);
TH2F* dummy_th2f(TH2F* pdf);

void Normalize_TH2_qqzz(char* tag){
	TFile *f = new TFile(Form("%s_template_unnorm.root", tag), "read");
	th2f_norm = (TH2F*) f->Get(Form("%s_template",tag));
	th2f_norm_up = (TH2F*) f->Get(Form("%s_template_up",tag));
	th2f_norm_dn = (TH2F*) f->Get(Form("%s_template_dn",tag));
	th2f_norm = norm_th2f_fast(th2f_norm);
//	th2f_norm = dummy_th2f(th2f_norm);
	th2f_norm->SetNameTitle(Form("%s_template",tag),"");
	th2f_norm_up = norm_th2f_fast(th2f_norm_up);
//	th2f_norm_up = dummy_th2f(th2f_norm_up);
	th2f_norm_up->SetNameTitle(Form("%s_template_up",tag),"");
	th2f_norm_dn = norm_th2f_fast(th2f_norm_dn);
//	th2f_norm_dn = dummy_th2f(th2f_norm_dn);
	th2f_norm_dn->SetNameTitle(Form("%s_template_dn",tag),"");

	TFile* fnew = new TFile(Form("%s_template.root", tag),"recreate");
	fnew->cd();
	fnew->WriteTObject(th2f_norm);
	fnew->WriteTObject(th2f_norm_up);
	fnew->WriteTObject(th2f_norm_dn);
	fnew->Close();
	return;

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
	int globalBin;
	double oldCon, binCon, newbinCon;
	double intx=0.; double intx_new=0.;
        for(int nx=0; nx<nxlast+2; nx++){
		intx=0.;  intx_new=0.;
		for(int ny=nyfirst; ny<nylast+2; ny++){
			globalBin = pdf->GetBin(nx, ny, 0);
			oldCon=pdf->GetBinContent(globalBin);
	//		cout<<"xbin: "<<nx<<"   ybin: "<<ny<<"   global bin: "<<globalBin<<"   Current bin content: "<<oldCon<<endl;
			intx+=oldCon;
		}

                for(int ny=nyfirst; ny<nylast+2; ny++){
                        globalBin = pdf->GetBin(nx, ny);
                        binCon = pdf->GetBinContent(globalBin);
                        newbinCon = binCon/intx;
			if (newbinCon<0.0001)	newbinCon=0.0001;
                        th2f_fast_temp->SetBinContent(globalBin, newbinCon);
			intx_new+=newbinCon;
        //		cout<< "xbin: "<<nx<<"   ybin: "<<ny<<"   global bin: "<<globalBin<<"   Current bin content: "<<newbinCon<<endl;
	        } 
//		cout<<"column nx: "<<nx<<"   old integral: "<<intx<<endl;
//		cout<<"column nx: "<<nx<<"   new integral: "<<intx_new<<endl;

        }    

/*        TCanvas c;
        gStyle->SetOptStat(0);
        c.cd();
        th2f_fast_temp->Draw("colz");
        c.Update();
        string str(pdf->GetName());
        c.SaveAs(Form("~/www/%s_fast.png",pdf->GetName()));
*/
        return th2f_fast_temp;

}

TH2F* dummy_th2f(TH2F* pdf){
	int nxbin, nybin;
	nxbin = pdf->GetXaxis()->GetNbins();
	nybin = pdf->GetYaxis()->GetNbins();
	int nxfirst, nxlast, nyfirst, nylast;
	nxfirst = pdf->GetXaxis()->GetFirst();
	nxlast = pdf->GetXaxis()->GetLast();
	nyfirst = pdf->GetYaxis()->GetFirst();
	nylast = pdf->GetYaxis()->GetLast();	

	TH2F* newpdf = (TH2F*) pdf->Clone();
	int gBin;
	std::vector<std::vector<float>> binContent(nxbin+1, vector<float>(nybin+1,0.));
	//first loop over x then loop over y
	for(int nx=0; nx<nxlast+1; nx++){
		for(int ny=nyfirst; ny<nylast+1; ny++){
			gBin = pdf->GetBin(nx, ny, 0);
			newpdf->SetBinContent(gBin, 0.1);
		}
	}
	return newpdf;	
	
}
