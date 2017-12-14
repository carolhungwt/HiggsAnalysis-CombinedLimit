/** Things to remember later
*  mzz range defined by ggZZ workspace, create 2 versions for onshell and offshell
*  give different ggzz RooKeysPdf name for 2e2mu and 4e
*
****/


using namespace std;
using namespace TMath;
using namespace RooFit;


void dosomething(TString chan ="2e2mu",int cate_vbf =0, bool onshell=true, int quad=3,int iCat=0){
 // for (int iCat=0; iCat<quad; iCat++){

  TString filename;
  if (onshell) filename = "workspace125_onshell/hzz"+chan+Form("_13TeV.input_func_%djet_cat%d.root",cate_vbf, iCat);
  else filename = "workspace125/hzz"+chan+Form("_13TeV.input_func_%djet_cat%d.root",cate_vbf, iCat);
  TFile* fwor=new TFile(filename, "recreate");
  fwor->cd();

  //	double lumi = 9.235;
  double lumi = 35.9;
  double ggzz_xsec = 2.827;
  double x_xsec = 1.1584*1.19*1.13;

  if (onshell){
    if (chan=="2e2mu"){
      ggzz_xsec = 0.2184*1.773674746;
      x_xsec = 0.99794*1.19*1.13*1.097535592*0.823938413;
    }
    else if(chan=="4mu"){
  ggzz_xsec = 0.1092*1.430496941;
  x_xsec = 0.540*1.19*1.13*1.098125903*0.798608623;
    }
    else{
      ggzz_xsec = 0.1092*1.58399592;
      x_xsec = 0.540*1.19*1.13*1.086610388*0.800546337;
    }
  }

  gStyle->SetOptStat(0);
double dcbPara_4e_2nd[6][9][2]={
{{ 3.604205E-02, 0},{ -8.546492E-01, 1.769646E-02},{ 3.402014E+00, -1.563241E-02},{ 3.287163E-01, 8.497529E-03},{ 2.011963E+00, -4.896627E-03},{ 4.870703E-01, 7.192263E-03},{ 1.4332,0/*2.307730E+00, -5.899981E-03*/},{ 1.679881E+00, -1.515169E-03},{ 7.663841E-01, 4.301968E-03}},
{{ 3.677142E-02, 0},{ 4.447919E+00, -1.673887E-02},{ 6.666261E-01, 1.232408E-02},{ 1.554128E+00, 7.132843E-03},{ 1.135115E+00, 1.036323E-02},{ 1.879356E+00, 4.784131E-03},{ 2.4735,0/*-6.367735E-01, 2.460872E-02*/},{ 4.428821E+00, -1.569707E-02},{ 2.592312E+00, -6.822154E-03}},
{{ -1.287582E-01, 7.730467E-04},{ 1.775613E+00, -1.501577E-02},{ 2.338905E-02, -1.537548E-03},{ 5.017587E-01, -5.577151E-03},{ -3.609846E-01, 1.274098E-03},{ 3.953380E-01, -4.716141E-03},{ -0.241434,0/*-3.287515E-01, 7.210817E-04*/},{ 3.377084E-01, -4.628214E-03},{ 4.988794E-01, -6.119909E-03}},
{{ 4.649758E-01, 1.549270E-02},{/* 2.23516,0 */1.736444E+01, -1.050342E-01},{ 1.249589E+00, 4.347397E-03},{ 3.135828E+00, -1.093686E-02},{ 1.398886E+00, 4.119205E-03},{ 5.014943E+00, -2.417578E-02},{ 1.8592,0/*1.143951E+00, 4.544188E-03*/},{ 3.956821E+00, -1.597148E-02},{ 4.639095E+00, -1.609898E-02}},
{{ -3.653200E-01, 4.198242E-02},{ 8.592613E+00, -5.248302E-02},{ 5.259923E+00, -2.689978E-02},{ 1.717497E+00, -2.304151E-03},{ 2.200323E+00, -7.454066E-03},{ 1.088825E+00, 2.420771E-03},{1.5036,0/* 1.016670E+01, -6.752724E-02*/},{ -3.932252E+00, 4.507903E-02},{ -7.682934E+00, 9.028073E-02}},
{{ 1.939240E-02, -1.523893E-05},{ -5.596520E-01, 1.190440E-02},{ 3.671491E-01, 5.292243E-03},{ -2.437097E-01, 1.094002E-02},{ -5.595199E-02, 1.017909E-02},{ -2.270109E-01, 1.218843E-02},{ 1.4089,0/*8.818474E-02, 1.095227E-02*/},{ 3.902680E-01, 9.641516E-03},{ 1.840794E-01, 1.394331E-02}},
};

double dcbPara_4mu_2nd[6][9][2]={
{{ 8.526115E-01, 2.227980E-03},{ 3.255692E+00, -1.725038E-02},{ -2.014864E-01, 1.060626E-02},{ 4.863959E-01, 4.915399E-03},{ 5.105956E+00, -3.152222E-02},{ -9.493855E-01, 1.663639E-02},{ -1.172206E+00, 1.933802E-02},{ 5.558455E-01, 4.405509E-03},{ 2.490628E+00, -1.306446E-02}},
{{ 4.899310E+00, -1.778398E-02},{ 1.338495, 0.0111748},{ 3.526374E+00, -8.595054E-03},{ 3.474449E+00, -7.248744E-03},{ 1.9668044, 0},{ 8.489893E-01, 1.358677E-02},{ -1.806192E+01, 1.698762E-01},{ 7.324985E+00, -3.354207E-02},{ 1.166543E+01, -7.308319E-02}},
{{ 7.767598E-02, -4.927211E-03},{ -2.874766E+00, 1.722325E-02},{ 2.661901E-01, -9.557390E-03},{ 7.725300E-01, -1.361708E-02},{ -1.454694E+00, 3.177778E-03},{ 2.310581E+00, -2.831616E-02},{ 9.450116E-01, -1.973634E-02},{ 7.153581E-01, -1.854016E-02},{ 2.431426E+00, -3.814720E-02}},
{{ 5.855148E+00, -2.257045E-02},{ 1.068162E+01, -5.501896E-02},{ 6.800230E+00, -2.488386E-02},{ 7.508105E+00, -2.053556E-02},{ 5.000000E+00, 0.000000E+00},{ 2.040790E+01, -1.205580E-01},{ 9.482288E+00, -3.727021E-02},{ 5.000779E+00, -6.414883E-06},{ -2.385651E+01, 2.104132E-01}},
{{/*0.48545775,0*/ -3.359361E+00, 3.475855E-02},{ 7.584396E-01, -2.554411E-03},{ -3.769422E+00, 4.224120E-02},{ 1.832647E+00, -5.768901E-03},{ -6.340022E+00, 1.075889E-01},{ -7.775982E-02, 6.497611E-03},{ 3.938394E+00, -1.307626E-02},{ 3.4079075,0 /*-2.399248E+01, 2.192031E-01*/},{ -9.023513E+01, 8.344963E-01}},
{{ 6.006382E-01, 5.446114E-03},{ 5.599025E-01, 8.733555E-03},{ -1.020963E+00, 2.400102E-02},{ 1.867931E-01, 1.623739E-02},{ 4.424567E+00, -1.586667E-02},{ 6.156504E-02, 2.214310E-02},{ -1.126659E+00, 3.345068E-02},{ -3.008629E-01, 3.018294E-02},{ 14.0769075, 0/*-1.616039E+00, 5.023324E-02*/}},
};

double dcbPara_2e2mu_2nd[6][9][2]={
{{ 5.308849E-01, 5.639260E-03},{ 1.617131E+00, -2.618333E-03},{ 5.849280E-01, 4.687802E-03},{ 5.683647E-01, 4.849986E-03},{ 1.259839E+00, 3.018607E-05},{ 1.041757E+00, 1.543258E-03},{ 1.253,0/*9.967042E-01, 1.875326E-03*/},{ 1.802505E+00, -4.258665E-03},{ 1.446138E+00, -3.797519E-03}},
{{ 4.768567E+00, -1.805979E-02},{ 3.966990E+00, -1.197597E-02},{ 3.983401E+00, -1.262042E-02},{ 1.892581E+00, 4.544490E-03},{ 1.064102E+00, 1.144452E-02},{ 3.547569E+00, -7.561439E-03},{ 2.6708,0/*-6.357442E+00, 7.060518E-02*/},{ -1.966102E-02, 2.115886E-02},{ 4.008361E+00, -1.488718E-02}},
{{ 4.963583E-01, -6.698603E-03},{ -1.198965E-01, -2.450679E-03},{ 5.567234E-01, -8.086327E-03},{ 6.328373E-02, -4.925896E-03},{ 3.818628E-01, -7.992381E-03},{ 8.063672E-01, -1.181705E-02},{-0.751209,0/* -3.183777E-01, -3.627623E-03*/},{ -3.632602E-01, -4.906693E-03},{ -1.812749E-01, -8.508741E-03}},
{{ 4.020662E+00, -1.446097E-02},{ 1.229353E+00, 7.632894E-03},{ 1.833292E+00, 3.995944E-03},{ 5.123854E+00, -1.906521E-02},{ 2.843544E+00, -2.383150E-03},{ 2.616983E+00, 2.148972E-03},{3.486,0/* 1.118821E+01, -5.973263E-02*/},{ 2.349563E+00, 5.358558E-03},{ 1.460400E+01, -8.289457E-02}},
{{/*0.95262625,0*/ -5.069595E+00, 5.217777E-02},{ -2.811633E+00, 3.251661E-02},{ -3.031967E+00, 3.691989E-02},{ 8.307041E-01, 5.111874E-03},{ 1.529242E+00, -3.703504E-03},{ -4.604330E-01, 1.507964E-02},{ 1.5409,0/*1.756774E+01, -1.251889E-01*/},{ 5.776022E+00, -3.707139E-02},{ -4.449155E+00, 4.910228E-02}},
{{ 1.707566E-02, 8.436180E-03},{ 4.358140E-01, 6.722924E-03},{ -1.701195E-01, 1.275579E-02},{ 1.014979E-01, 1.237920E-02},{ 1.185306E-01, 1.411009E-02},{ -7.356599E-02, 1.751896E-02},{2.3410,0/* -3.012325E-01, 2.112347E-02*/},{ -8.143149E-02, 2.245820E-02},{ 1.147075E+00, 1.890421E-02}},
};




  double eff[3][11]={
    //		ggH->0p_4e_param_0:
    { -4.4781, 4.56478, -410.689, 302.175, 3.54126, 0.00226419, -9.08486e-07, 0.0861973, 160, 45.6037, 1.07622e-10 },
    //	ggH->0p_4mu_param_0:
    { -4.44313, 4.59816, -2313.39, 1129.31, 3.13553, 0.00269624, -1.20507e-06, -9.71235, -105.225, 79.0966, 1.66983e-10 },
    //	ggH->0p_2e2mu_param_0:
    { -4.45465, 4.5858, -313.199, 295.457, 3.09284, 0.00192647, -7.78952e-07, 0.36715, 105.723, 81.4614, 9.69396e-11 }
  };


 double eff_adj[3][9][2]={
//Channel 4e
{{0.142951, -0.000237},{0.140153, -0.000233},{0.066292, 0.000366},{0.034540, 0.000634},{0.086560, 0.000192},{0.109414, -0.000006},{0.096713, 0.000119},{0.152212, -0.000342},{0.171165, -0.000493}},
//Channel 4mu
{{0.031283, 0.000614},{0.086497, 0.000176},{0.102232, 0.000075},{0.047921, 0.000503},{0.035003, 0.000641},{0.055539, 0.000465},{0.136926, -0.000230},{0.256384, -0.001143},{0.248215, -0.001100}},
////Channel 2e2mu
{{0.033567, 0.000617},{0.066665, 0.000353},{0.085146, 0.000204},{0.074805, 0.000285},{0.020880, 0.000707},{0.059935, 0.000412},{0.107745, 0.000028},{0.226598, -0.000925},{0.324660, -0.001680}},
};

  double dcbPara_2nd[6][3];
  double* effsig;
  double* effadj;
  int nparampar=2;

  if (chan=="4e"){
    for (int p=0; p<6; p++){ for (int k=0; k<nparampar; k++) dcbPara_2nd[p][k]=dcbPara_4e_2nd[p][iCat][k]; }
    effsig=eff[0];
    effadj = eff_adj[0][iCat];
  }
  if (chan=="4mu"){
    for (int p=0; p<6; p++){ for (int k=0; k<nparampar; k++) dcbPara_2nd[p][k]=dcbPara_4mu_2nd[p][iCat][k]; }
    effsig=eff[1];
    effadj = eff_adj[1][iCat];
  }
  if (chan=="2e2mu"){
    for (int p=0; p<6; p++){ for (int k=0; k<nparampar; k++) dcbPara_2nd[p][k]=dcbPara_2e2mu_2nd[p][iCat][k]; }
    effsig=eff[2];
    effadj = eff_adj[2][iCat];
  }

if(iCat==0){
for(int kk=0; kk<6; kk++)	std::cout<<dcbPara_2nd[kk][0]<<endl;}


  double lowarr[2]={ 100.5, 100.5 };
  double higharr[2]={ 1500.5, 150.5 };
  const int nbinsarr[2]={ 1500, 500 };

  double recolowarr[2]={ 104, 105 };
  double recohigharr[2]={ 1604., 140. };
  //const int reconbinsarr[2]={ 750, 100 };

  const double low= lowarr[onshell];
  const double high=higharr[onshell];
  const int nbins= nbinsarr[onshell];

  const double low_reco=recolowarr[onshell];
  const double high_reco=recohigharr[onshell];
  //const int nbins_reco=reconbinsarr[onshell];

  cout << low << "\t" << high << endl;
  cout << low_reco << "\t" << high_reco << endl;

  TString ap = (onshell ? "_onshell" : "");
  TFile* fpdfbkg = new TFile("pdfs"+ap+".root");
  RooWorkspace* wbkg =(RooWorkspace*)fpdfbkg->Get("w");

  //RooRealVar* mzz = new RooRealVar("ZZMass","M_{ZZ} (GeV)",125,low,high);
  RooRealVar* mzz = wbkg->var("ZZMass");
  RooRealVar* mreco= new RooRealVar("mreco", "M_{ZZ} (GeV)", 125, low_reco, high_reco);
  //RooRealVar* mdiff= new RooRealVar("mdiff", "M_{ZZ} (GeV)", 125, low_reco, high_reco);

  RooRealVar* r= new RooRealVar("r", "signal strength", 1., 0.0001, 1000);

  //mreco->setBins(nbins_reco);
  RooRealVar* mean = new RooRealVar("mean_pole", "mean_pole", 125, 100, 180);
  RooRealVar* sigma= new RooRealVar("sigma_pole", "sigma_pole", 0.00407, 0., 10.);

  //RooConstVar* mean_125= new RooConstVar("mean_125", "mean_125", 125);
  //RooConstVar* sigma_125= new RooConstVar("sigma_125", "sigma_125", 0.00407);

  TCanvas* cframe[4];
  for (unsigned int c=0; c<4; c++) cframe[c] = new TCanvas(Form("c%i", c), "", 750, 700);
  RooPlot* frame=mreco->frame(low_reco, high_reco);
  RooPlot* frame_mzz=mzz->frame();
  frame_mzz->SetTitle("ZZMass");
  RooPlot* frame_width=sigma->frame();
  frame_width->SetTitle("width");
  RooPlot* frame_mean=mean->frame();
  frame_mean->SetTitle("mean");
  cframe[3]->cd(); frame_mean->Draw();
  fwor->cd();

  TFile* flo=new TFile("ggh_input_spline.root", "read");
  //TFile* flo=new TFile("xsec_ggzz4l_13TeV_4e.root","read");
  //TFile* flo=new TFile("width_new.root","read");
  TString chn = "";
  if (chan!="2e2mu") chn="_4e";
  //TGraph* lo=(TGraph*) flo->Get("gr_"+chn);
  //TGraph* lo=(TGraph*) flo->Get("br_"+chn);
  TSpline3* lo=(TSpline3*)flo->Get("sp_xsec_statnom"+chn);
  fwor->cd();

  SplinePdf par2_int("par2_int"+chan+Form("%d", iCat), "", *mzz, *mean, *sigma, *lo);
  RooRealVar m_gauss("m_gauss", "", 125);
  RooRealVar w_gauss("w_gauss", "", 0.004);
  RooGaussian gauss("gauss", "", *mzz, m_gauss, w_gauss);

  TString pdfn = "2e2mu";
  if (chan!="2e2mu") pdfn = "4e";
  RooKeysPdf* pdfbkg = (RooKeysPdf*)wbkg->pdf("pdfbkg_"+pdfn);

  RooConstVar* ggzznorm= new RooConstVar("ggzznorm"+chan+Form("%d", iCat), "", lumi*ggzz_xsec);
  RooExtendPdf pdf_ggzz("pdf_ggzz"+chan+Form("%d", iCat), "pdf_ggzz"+chan+Form("%d", iCat), *pdfbkg, *ggzznorm);

  RooConstVar* xnorm= new RooConstVar("xnorm"+chan+Form("%d", iCat), "", lumi*x_xsec);
  RooExtendPdf pdf_x("pdf_x"+chan+Form("%d", iCat), "pdf_x"+chan+Form("%d", iCat), par2_int, *xnorm);


  TFile* fphase_noweight=new TFile("fphase_ggH.root");
  TGraph* phase_sin = (TGraph*)fphase_noweight->Get("sinspline");
  TGraph* phase_cos = (TGraph*)fphase_noweight->Get("cosspline");

  /*
  TFile* fkfactor = new TFile("/afs/cern.ch/work/x/xiaomeng/test/myWorkingArea/highmass/Fit/whatthefuck/Kfactor_Collected_ggHZZ_2l2l_NNLO_NNPDF_NarrowWidth_13TeV.root");
  //TSpline3* ggZZ_kf [9];//= (TSpline3*)fkfactor->Get("sp_kfactor_Nominal");
  TSpline3* ggZZ_kf = (TSpline3*)fkfactor->Get("sp_kfactor_Nominal");
  */
  TString strSystTitle[5] ={
    "Nominal",
    "qcd_dn",
    "qcd_up",
    "pdf_dn",
    "pdf_up"
  };
  TFile* fkfactor = new TFile("kfactor.root");
  TFile* fbkge = new TFile("/afs/cern.ch/work/x/xiaomeng/public/bkg_reg_eff.root");
  TGraph* eff_bkg =  (TGraph*)fbkge->Get("ggZZ_reg_"+chan);


  TGraph* phase_sin_fix= new TGraph(nbins*2);
  TGraph* phase_cos_fix= new TGraph(nbins*2);

  RooWorkspace w("w");
  TGraph* ggZZ_kf[5];

  mean->setRange(100, 1500);
  sigma->setRange(0.00005, 100.);
  mean->setVal(125);
  sigma->setVal(0.004165);

TString formu_2nd="@1+@0*@2";

  RooArgList formuList_a1;
  RooArgList formuList_a2;
  RooArgList formuList_mean;
  RooArgList formuList_n1;
  RooArgList formuList_n2;
  RooArgList formuList_sigma;
  formuList_a1.add(*mzz);
  formuList_a2.add(*mzz);
  formuList_mean.add(*mzz);
  formuList_n1.add(*mzz);
  formuList_n2.add(*mzz);
  formuList_sigma.add(*mzz);

  RooConstVar* a1_p0_0_2nd[11];
  RooConstVar* a2_p0_0_2nd[11];
  RooConstVar* mean_p0_0_2nd[11];
  RooConstVar* n1_p0_0_2nd[11];
  RooConstVar* n2_p0_0_2nd[11];
  RooConstVar* sigma_p0_0_2nd[11];
  for (int i=0; i<nparampar; i++){
    a1_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_a1_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), Form("%s_%d_a1_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat,cate_vbf), dcbPara_2nd[0][i]);
    a2_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_a2_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat,cate_vbf), Form("%s_%d_a2_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), dcbPara_2nd[1][i]);
    mean_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_mean_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), Form("%s_%d_mean_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), dcbPara_2nd[2][i]);
    n1_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_n1_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), Form("%s_%d_n1_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), dcbPara_2nd[3][i]);
    n2_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_n2_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), Form("%s_%d_n2_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), dcbPara_2nd[4][i]);
    sigma_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_sigma_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), Form("%s_%d_sigma_p0_0_2nd_iCat%d_%djet", chan.Data(), i, iCat, cate_vbf), dcbPara_2nd[5][i]);
std::cout<<"***********************************"<<endl;
if(i==0) std::cout<<(a1_p0_0_2nd[i])->getValV()<<endl;
std::cout<<"***********************************"<<endl;
    formuList_a1.add(*a1_p0_0_2nd[i]);
    formuList_a2.add(*a2_p0_0_2nd[i]);
    formuList_mean.add(*mean_p0_0_2nd[i]);
    formuList_n1.add(*n1_p0_0_2nd[i]);
    formuList_n2.add(*n2_p0_0_2nd[i]);
    formuList_sigma.add(*sigma_p0_0_2nd[i]);
  }

  RooFormulaVar* a1_p0_2nd= new RooFormulaVar("a1_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "a1_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_a1);
  RooFormulaVar* a2_p0_2nd= new RooFormulaVar("a2_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "a2_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_a2);
  RooFormulaVar* mean_p0_2nd= new RooFormulaVar("mean_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "mean_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_mean);
  RooFormulaVar* n1_p0_2nd= new RooFormulaVar("n1_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "n1_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_n1);
  RooFormulaVar* n2_p0_2nd= new RooFormulaVar("n2_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "n2_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_n2);
  RooFormulaVar* sigma_p0_2nd= new RooFormulaVar("sigma_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), "sigma_p0_2nd"+chan+Form("_iCat%d_%djet",iCat, cate_vbf), formu_2nd, formuList_sigma);


  RooFormulaVar* sigma_p0_up = new RooFormulaVar("sigma_p0_up", "", "@0+0.2*@0", *sigma_p0_2nd);
  RooFormulaVar* sigma_p0_dn = new RooFormulaVar("sigma_p0_dn", "", "@0-0.2*@0", *sigma_p0_2nd);
  RooDoubleCB dcrReso(Form("dcrReso_%s_iCat%d_%djet",chan.Data(), iCat, cate_vbf), "Double Crystal ball ", *mreco, *mzz, *mean_p0_2nd, *sigma_p0_2nd, *a1_p0_2nd, *n1_p0_2nd, *a2_p0_2nd, *n2_p0_2nd);
  RooDoubleCB dcrReso_up(Form("dcrReso_%s_iCat%d_%djet_up",chan.Data(), iCat, cate_vbf), "dcb up", *mreco, *mzz, *mean_p0_2nd, *sigma_p0_up, *a1_p0_2nd, *n1_p0_2nd, *a2_p0_2nd, *n2_p0_2nd);
  RooDoubleCB dcrReso_dn(Form("dcrReso_%s_iCat%d_%djet_dn",chan.Data(),iCat, cate_vbf), "dcb dn", *mreco, *mzz, *mean_p0_2nd, *sigma_p0_dn, *a1_p0_2nd, *n1_p0_2nd, *a2_p0_2nd, *n2_p0_2nd);


  //Width_conv* convpdf_spline[5];
  //for(int f = 0; f<1;f++){
  int f=0;
  TString sysname = strSystTitle[f];
  ggZZ_kf[f] =(TGraph*)fkfactor->Get("sp_kfactor_"+sysname);
  TGraph* effxkf_sig= new TGraph(nbins*2);
  TGraph* effxkf_bkg= new TGraph(nbins*2);

  for (int i =0; i<nbins*2; i++){
    double cva = low+ i*(high-low)/double(nbins)/2.;
    double effval_sig = (effsig[0]+effsig[1]*TMath::Erf( (cva-effsig[2])/effsig[3] ))*(effsig[4]+effsig[5]*cva+effsig[6]*cva*cva+effsig[10]*cva*cva*cva)+effsig[7]*TMath::Gaus(cva,effsig[8],effsig[9]);
    double effcate_sig = 0.9527;
    double ggh_vbf2j_sig = 0.0439;
    double ggh_vhlep_sig = 1-ggh_vbf2j_sig-effcate_sig;
    double effcate = effadj[0]+effadj[1]*(cva);

      if(cate_vbf==0)         effcate_sig=0.9527;
      else if(cate_vbf==1)    effcate_sig = ggh_vbf2j_sig;
      else if(cate_vbf==2)     effcate_sig = ggh_vhlep_sig;

    effcate = effcate*effcate_sig;
    double va_bkg= eff_bkg->Eval(cva)*ggZZ_kf[f]->Eval(cva)*effcate;
    double va_sig= effval_sig*ggZZ_kf[f]->Eval(cva)*effcate;
    //if (chan=="2e2mu"){
    //  va_bkg*=0.95;
    //  va_sig*=0.95;
    //}
    effxkf_sig->SetPoint(effxkf_sig->GetN(), cva, va_sig);
    effxkf_bkg->SetPoint(effxkf_bkg->GetN(), cva, va_bkg);
    phase_sin_fix->SetPoint(phase_sin_fix->GetN(), cva, phase_sin->Eval(cva)/2.*1.76);
    phase_cos_fix->SetPoint(phase_cos_fix->GetN(), cva, phase_cos->Eval(cva)/2.*1.76);
  }
  effxkf_sig->SetName("gghsigeffxkf"+chan+Form("%d", iCat)+strSystTitle[f]);
  effxkf_bkg->SetName("gghbkgeffxkf"+chan+Form("%d", iCat)+strSystTitle[f]);
  phase_sin_fix->SetName("gghsin"+chan+Form("%d", iCat));
  phase_cos_fix->SetName("gghcos"+chan+Form("%d", iCat));
  //phase_cos_fix->Draw("al");
  //phase_sin_fix->Draw("lsame");
  //
  ///return;
  RooAbsPdf* convpdf_spline;
  RooAbsReal* final_integral;
  if (onshell){
    convpdf_spline = new Width_conv("ggH"+strSystTitle[f]+Form("_%s_iCat%d_%djet",chan.Data(),iCat, cate_vbf), "ggH"+strSystTitle[f], *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
    convpdf_spline->SetNameTitle("ggH", "ggH");
    Width_conv convpdf_spline_up("ggH_Res"+chan+"Up", "ggH"+chan+"Up", *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso_up), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
    Width_conv convpdf_spline_dn("ggH_Res"+chan+"Down", "ggH"+chan+"Down", *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso_dn), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
    w.import(*convpdf_spline, RecycleConflictNodes());
    w.import(convpdf_spline_up, RecycleConflictNodes());
    w.import(convpdf_spline_dn, RecycleConflictNodes());
    final_integral = convpdf_spline->createIntegral(*mreco);
  }
  else{
    convpdf_spline=new Width_conv_offshell("bggH"+strSystTitle[f], "bggH"+strSystTitle[f], *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
        final_integral = convpdf_spline->createIntegral(*mreco);
  }
  
  mean->setVal(125);

  //pdf_ggzz.plotOn(frame_mzz);
  pdf_x.plotOn(frame_mzz);

  sigma->setVal(5.);
  convpdf_spline->plotOn(frame);
  sigma->setVal(1.);
  convpdf_spline->plotOn(frame,LineColor(2));
  sigma->setVal(0.004);
  convpdf_spline->plotOn(frame,LineColor(f+1));
  cframe[0]->cd(); frame->Draw(); fwor->cd();
  cframe[1]->cd(); frame_mzz->Draw(); fwor->cd();

  r->setVal(0);
  double bexp = final_integral->getVal();

  RooConstVar* bkg_integral= new RooConstVar("bkg_integral"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "", bexp);

  mean->setVal(125);
  //ROOT::Math::Interpolator inter(200, ROOT::Math::Interpolation::kCSPLINE);
  sigma->setRange(0.00005, 100.);

  TH2F* hint;
  TH2F* hsig;
  if (!onshell){
    hint= new TH2F("hint", "", 101, 119.95, 130.05, 101, -0.0005, 0.1005);
    hsig= new TH2F("hsig", "", 101, 119.95, 130.05, 101, -0.0005, 0.1005);
  }
  else{
    hint= new TH2F("hint", "", 101, 119.95, 130.05, 101, -0.025, 5.025);
    hsig= new TH2F("hsig", "", 101, 119.95, 130.05, 101, -0.025, 5.025);
  }

  for (int i = 0; i < 101; i++){
  //  if (i%10==0) cout << i << endl;
    for (int j = 0; j <101; j++){
      double mv=hint->GetXaxis()->GetBinCenter(i+1);
      double sv=hint->GetYaxis()->GetBinCenter(j+1);
      sigma->setVal(sv);
      mean->setVal(mv);
      //sigma->setVal(0.001*(i+1));
      //mean->setVal(100+0.5*(j+1));
      r->setVal(1);
      double sbi =  final_integral->getVal();
      r->setVal(4);
      double sbi2 =  final_integral->getVal();
      double sexp = ((sbi2-sbi*2)+bexp)/2.;
      double iexp = sbi -sexp -bexp;
      //cout << sigma->getVal() << "\t" << mean->getVal() << endl;
      //float integral = final_integral->getVal();
      hint->Fill(mean->getVal(), sigma->getVal(), iexp);
      hsig->Fill(mean->getVal(), sigma->getVal(), sexp);
    }
  }
  r->setVal(1);

  RooDataHist* hinthist= new RooDataHist("hinthist"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "hinthist"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], RooArgSet(*mean, *sigma), hint);
  RooHistFunc* hintfunc = new RooHistFunc("hintfunc"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "", RooArgSet(*mean, *sigma), *hinthist);
  Width_integral inter_integral("int_integral"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "", *mean, *sigma, RooArgList(*hintfunc));

  RooDataHist* hsighist= new RooDataHist("hsighist"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "hsighist"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], RooArgSet(*mean, *sigma), hsig);
  RooHistFunc* hsigfunc = new RooHistFunc("hsigfunc"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "", RooArgSet(*mean, *sigma), *hsighist);
  Width_integral sig_integral("sig_integral"+chan+Form("%d_%djet", iCat, cate_vbf)+strSystTitle[f], "", *mean, *sigma, RooArgList(*hsigfunc));
  RooFormulaVar overall_integral("ggH_norm"+strSystTitle[f], "", "@0*@2+ @1 + sqrt(@2)*@3", RooArgList(sig_integral, *bkg_integral, *r, inter_integral));
  if (f==0) overall_integral.SetNameTitle("ggH_norm", "ggH_norm");

  Width_conv_integral* convpdf_spline_integral=new Width_conv_integral("ggH", "ggH", *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso, *hsigfunc, *hintfunc), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
  Width_conv_integral convpdf_spline_integral_up("ggH_Res"+chan+"Up", "ggH"+chan+"Up", *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso_up, *hsigfunc, *hintfunc), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
  Width_conv_integral convpdf_spline_integral_dn("ggH_Res"+chan+"Down", "ggH"+chan+"Down", *mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz, dcrReso_dn, *hsigfunc, *hintfunc), *phase_cos_fix, *phase_sin_fix, *effxkf_sig, *effxkf_bkg);
  w.import(*convpdf_spline_integral, RecycleConflictNodes());
  w.import(convpdf_spline_integral_up, RecycleConflictNodes());
  w.import(convpdf_spline_integral_dn, RecycleConflictNodes());

  mean->setVal(125);
  sigma->setVal(0.004);
  cout << "Signal: \t Background: \t Interference: \t" <<endl;
  cout << sig_integral.getVal() << "\t" << bkg_integral->getVal() << "\t" << inter_integral.getVal() << endl;

  std::fstream yieldtxt("yield.txt", ios::app|ios::out);
  yieldtxt<<"Channel: "<<chan<<"\t Category: "<<iCat<<"\t Jet Category: "<<cate_vbf<<endl;
  yieldtxt<<"Signal: \t Background: \t Interference: \t" <<endl;
  yieldtxt<<sig_integral.getVal() << "\t" << bkg_integral->getVal() << "\t" << inter_integral.getVal() << endl;

  overall_integral.plotOn(frame_width);
  cframe[2]->cd(); frame_width->Draw(); fwor->cd();

  mzz->setConstant(0);
  mean->setConstant(0);
  sigma->setConstant(0);
  mreco->setConstant(0);

  mreco->setRange(low_reco, high_reco);
  mean->setVal(125);
  sigma->setVal(0.004);
  r->setVal(1);

  w.import(overall_integral, RecycleConflictNodes());

  mreco->Print("v");
  cout << "mreco nbins: " << mreco->getBins() << endl;

  w.Write();

  for (unsigned int c=0; c<4; c++){
    cframe[c]->cd();
    cframe[c]->Modified();
    cframe[c]->Update();
    fwor->WriteTObject(cframe[c]);
    cframe[c]->Close();
  }

  flo->Close();
  fwor->Close();
  delete a1_p0_0_2nd[0]; 
  delete a2_p0_0_2nd[0];
  delete mean_p0_0_2nd[0];
  delete n1_p0_0_2nd[0];
  delete n2_p0_0_2nd[0];
  delete sigma_p0_0_2nd[0];




    return;
  
}

void clean_quad9_mH125(TString chan="4e", int cat_vbf=0, bool onshell=true, int quad=9){
  gSystem->Exec("mkdir -p ./workspace125_onshell/../workspace125/");
  gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
  gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
  gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");
for(int iCat=6; iCat<quad; iCat++){
  dosomething(chan, cat_vbf, onshell,quad,iCat);
}
  //dosomething("2e2mu",0,0);
  //dosomething("4e",0,0);
  //dosomething("4mu",0,0);
  //dosomething("2e2mu",1,0);
  //dosomething("4e",1,0);
  //dosomething("4mu",1,0);
}

