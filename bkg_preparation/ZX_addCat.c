
int findCat(float dmass, int chan){
	float dmassCuts[2];

	int cat;
	if(chan == 1){	//4e
		dmassCuts[0]=0.006816125;
		dmassCuts[1]=0.008640125;
	}
	else if(chan==2){	//4mu
		dmassCuts[0]=0.01348525;
		dmassCuts[1]=0.019206625;
	}
	else if (chan==3){
		dmassCuts[0]=0.0097525;
		dmassCuts[1]=0.014652;
	}
	if(dmass<dmassCuts[0])		cat = 0;
	else if(dmass<dmassCuts[1])	cat = 1;
	else 				cat = 2;

	return cat;
}

void ZX_addCat(){
	TFile *f1 = new TFile("ZX.root","read");
	TTree* t = (TTree*) f1->Get("candTree");
	float dbkg_kin, ZZMass, ZZMassErrCorr, weight, dmass;
	int chan, vbfcate, d2jet,cat;
	t->SetBranchAddress("dbkg_kin",&dbkg_kin);
	t->SetBranchAddress("ZZMass",&ZZMass);
	t->SetBranchAddress("ZZMassErrCorr",&ZZMassErrCorr);
	t->SetBranchAddress("weight",&weight);
	t->SetBranchAddress("chan",&chan);
	t->SetBranchAddress("vbfcate",&vbfcate);
	t->SetBranchAddress("d2jet",&d2jet);

	TFile *fnew = new TFile("ZX_withCat.root","recreate");
	TTree* tnew = new TTree("candTree", "candTree");
	tnew->Branch("ZZMass",&ZZMass,"ZZMass/F");
	tnew->Branch("dbkg_kin",&dbkg_kin,"dbkg_kin/F");
	tnew->Branch("weight",&weight,"weight/F");
	tnew->Branch("cat",&cat,"cat/I");
	tnew->Branch("chan",&chan,"chan/I");
	
	for(int i=0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		dmass = ZZMassErrCorr/ZZMass;
		cat = findCat(dmass, chan);
		tnew->Fill();
	}	
	
	fnew->WriteTObject(tnew);
	fnew->Close();	

return;}
