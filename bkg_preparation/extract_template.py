import ROOT as root
import sys 
import ROOT.RooFit as roofit


if __name__=='__main__':
	filename = str(sys.argv[1])
	selectedCh = int(sys.argv[2])
	f = root.TFile(filename, 'read')
	if 'ZX' in filename:
		tag = 'ZX'
		t = f.Get('candTree')
	if 'qqzz' in filename:
		tag = 'qqzz'
		t = f.Get('SelectedTree')

#	mreco = roofit.RooRealVar('mreco', 'mreco', 125., 105., 140.)
#	dbkg_kin = roofit.RooRealVar('dbkg_kin', 'dbkg_kin', 0., 1.)

	xmin, xmax = 105., 140.
	xbinsize = 1
	xbin = (xmax-xmin)/xbinsize
	ymin, ymax = 0., 1.
	ybinsize = 0.1
	ybin = (ymax-ymin)/ybinsize

	th2f_temp = root.TH2F(tag+'_template', tag+'_template', int(xbin), xmin, xmax, int(ybin), ymin, ymax)
	th2f_temp_up = root.TH2F(tag+'_template_up', tag+'_template_up', int(xbin), xmin, xmax, int(ybin), ymin, ymax)
	th2f_temp_dn = root.TH2F(tag+'_template_dn', tag+'_template_dn', int(xbin), xmin, xmax, int(ybin), ymin, ymax)
	for en in t:
		if en.chan != selectedCh:
			pass
		if 'ZX' in filename:
			th2f_temp.Fill(en.ZZMass, en.dbkg_kin, en.weight)
		if 'qqzz' in filename:
			th2f_temp.Fill(en.mreco, en.dbkg_kin, en.weight)
			th2f_temp_up.Fill(en.mreco, en.dbkg_kin, en.weight_up);
			th2f_temp_dn.Fill(en.mreco, en.dbkg_kin, en.weight_dn);

	c1 = root.TCanvas('','',1000, 800)
	root.gStyle.SetOptStat(0)
	c1.cd()
	th2f_temp.Draw('colz')
	c1.SaveAs('~/www/'+tag+'_check_th2f.png')

	fnew = root.TFile(tag+'_template_unnorm.root', 'recreate')
	fnew.cd()
	fnew.WriteTObject(th2f_temp)
	fnew.WriteTObject(th2f_temp_up)
	fnew.WriteTObject(th2f_temp_dn)
	fnew.Close()
