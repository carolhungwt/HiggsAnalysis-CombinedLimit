#include "resofit.h"
#include "style.cc"
void  readData(TString channel="4e", int cat=0)
 {

  for (int i=0; i<maxMassBin; i++) {
	cout<<"Now in readData.cc"<<endl;
    sprintf(tempmass,"mh%d",massBin[i]);
    massrc.defineType(tempmass,massBin[i]);

  if(channel=="4e") width[i] = 5.92172+0.00639677*(massBin[i]-600)-1.06763e-06*(massBin[i]-600)*(massBin[i]-600)+4.10566e-09*(massBin[i]-600)*(massBin[i]-600)*(massBin[i]-600);
  else if(channel=="4mu") width[i] = 8.8625+0.023313*(massBin[i]-600)+0.000014677*(massBin[i]-600)*(massBin[i]-600);
  else if(channel=="2e2mu") width[i] = 7.5026+0.0156385*(massBin[i]-600)+7.8748e-06*(massBin[i]-600)*(massBin[i]-600)+2.35478e-09*(massBin[i]-600)*(massBin[i]-600)*(massBin[i]-600);
  

  xMin[i] = width[i]*(-15);
  xMax[i] = width[i]*(10);

  cout << "Individual fit, mass: "<<massBin[i]<<" , range: [" << xMin[i] << " , " << xMax[i] << "]." << endl<<endl;
  }

  RooArgSet ntupleVarSet(x,w,massrc);
  dataset = new RooDataSet("resoM","resoM",ntupleVarSet,WeightVar("myW"));
  

   for (int i=0; i<Nfiles; i++) {

      sprintf(inputfile,"%s/MH%d_%s_quad_9.root",inputDir.Data(),inputfiles[i],channel.Data());

  char catname[100];
  sprintf(catname, "Category_%d",cat); 

  TTree* candTree;
  TFile* ftemp;



   if(!gSystem->AccessPathName(inputfile)){ 
   ftemp = new TFile(inputfile);
   candTree = (TTree*) ftemp->Get(catname);
   cout<< "using sample: "<<inputfile<<endl; 
   cout<<"Reading entries in " << catname<<endl; }
   else cout<<"Cannot find file!"<<inputfile;
   

  //  int  nentries = 10;
    int  nentries = candTree->GetEntries();
    cout<<"entries="<<nentries<<endl;
    //--- ggTree part
    candTree->SetBranchAddress("ZZMass",&m4l);
    candTree->SetBranchAddress("GenHMass",&genM);
    candTree->SetBranchAddress("overallEventWeight",&weight);
    dataset_sub[i] = new RooDataSet(Form("resoM_mh%d",massBin[i]),Form("resoM_mh%d",massBin[i]),ntupleVarSet,WeightVar("myW"));

    int count =0;
    for(int k=0; k<nentries; k++){
      candTree->GetEntry(k);

      		ntupleVarSet.setCatIndex("massrc",massBin[i]);
      		ntupleVarSet.setRealValue(Form("reso",massBin[i]),m4l-genM);
      		ntupleVarSet.setRealValue("myW",weight);

       		dataset_sub[i]->add(ntupleVarSet, weight);
//    }
	count++;
  }
  cout << "number of count: "<<count << endl<<endl;
  cout << "dataset n entries: " << dataset_sub[i]->sumEntries() << endl;
  }
//}
//}
}
