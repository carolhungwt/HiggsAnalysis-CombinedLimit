#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
/*#include "TSystem.h"
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
#include "RooPlot.h"
#include "RooNumIntConfig.h"
#include "RooWorkspace.h"
#include <ZZMatrixElement/MELA/interface/Mela.h>
#include <ZZMatrixElement/MELA/interface/ScalarPdfFactory_HVV.h>
#include "NCSplinePdfFactory_1D.h"
#include "NCSplinePdfFactory_2D.h"
#include "NCSplinePdfFactory_3D.h"
*/
using namespace RooFit;
using namespace std;

string ch_list[] = {"2e2mu"};//"4mu","4e","2e2mu"};
char *input_address[200];
RooRealSumPdf sumpdf_temp;

vector<TH2F*> hist_template(3);
vector< vector<RooNCSplinePdf_3D_fast*> > spline_template(3, vector<RooNCSplinePdf_3D_fast*>(3));
//vector< vector<TH2F*>> th2fs(3,vector<RooProdPdf*>(3));
vector<vector<TH2F*>> th2fs(3, vector<TH2F*>(3));
vector<vector<RooArgList>> prods(3, vector<RooArgList>(3));
vector<RooRealSumPdf> sumpdfs;

vector<RooArgList> coeff_list;
vector<vector<RooArgList>> pdf_list(3, vector<RooArgList>(3));

// Input and output filenames
TString hist_input = "/afs/cern.ch/user/w/wahung/work/CMSSW_8_0_26_patch1/src/HiggsAnalysis/CombinedLimit/Mass-Width/testBKg/template_th2fs/dbkgkin_mreco_WS_cond_hist_ggH_2e2mu_125_mreco_";
TString spline_input = "/afs/cern.ch/user/w/wahung/work/CMSSW_8_0_26_patch1/src/HiggsMassConstraint_new/HiggsMassConstraint/test/spline_WS_withCat_quad3/";
//TString spline_input = "/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/Mass-Width/prepareInputs/";
// User-Defined functions

vector<TH2F*> extract_templates(TString input_file);
vector<RooNCSplinePdf_3D_fast*> extract_splines(RooWorkspace* w, string channel, int cat);
//RooArgList prod_pdf(vector<RooNCSplinePdf_3D_fast*> splines, vector<TH2F*> th2fs, TString ch);
RooArgList prod_pdf(RooWorkspace* w, TString input_file, string ch, int i, int cat);
RooArgList writeFormula(RooWorkspace* w, TString tag);
RooArgList extract_coeff(RooWorkspace* w, string tag);
