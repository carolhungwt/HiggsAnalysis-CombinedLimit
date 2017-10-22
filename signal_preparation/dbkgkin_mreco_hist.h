//dbkgkin_mreco_hist.h
#include <fstream>
#include <time.h>

using namespace std;
using namespace RooFit;

double xmin = 105.;
double xmax = 140.;
int xbin = 35;
int ybin = 10;
float zzmass, m4l,dbkg, weightH, weightHBI, weight2;
int chan;
float overallEventWeight;
ofstream timeRd("time_record.txt",ofstream::out);
char timestr[200];
clock_t starttime;
clock_t endtime;
//TString outputFile = "cond_hist_ggHmH150_2e2mu_withcut.root";
//TString outputFile = "cond_hist_vbf_2e2mu_115_250_withCut.root";
//TString outputFile = "cond_hist_ggH_2e2mu_115_250_w2.root";
//TString outputFile = "cond_hist_ggH0PM_M125.root";
char* outputFile = "cond_hist_ggH_2e2mu_125_mreco";
char* workdir = "/afs/cern.ch/user/w/wahung/work/CMSSW_8_0_26_patch1/src/HiggsAnalysis/CombinedLimit/Mass-Width/testBKg";
TString pngtag = "ggH_2e2mu_M125_norm";
vector<TH2F*> Extract_all_hist_2d(TTree *t1, char *tag);
//TH2F* extract_int(TH2F* bkgPdf, TH2F* sigPdf, TH2F* bsiPdf, const float xmin, const float xmax);
vector<int> getDim(TH2F* pdf);
TH2F* norm_th2f(TH2F* pdf);
