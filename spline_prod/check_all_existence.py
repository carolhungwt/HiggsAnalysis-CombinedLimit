#!/usr/bin/ python

import os

quad = 9
numJetCate = 3
numProdCate = 3
channels = ['4e'] #,'4mu','2e2mu']

def giveprodtag(prod):
	if prod==0:	return 'ggH'
	elif prod==1:	return 'vbf'
	elif prod==2:	return 'vh'
	else:
		raise ValueError('prod number can only be 0,1 or 2')

def check_path_exists(ch, cat, prod, jet):
	notfoundall = False
	file_not_found=''
#	import_dir = '/afs/cern.ch/user/w/wahung/work/CMSSW_8_0_26_patch1/src/HiggsMassConstraint_new/HiggsMassConstraint/test/171010_spline_prod_170623_quad9/spline_WS_withCat_quad9'
	import_dir = os.getcwd()
	prodtag = giveprodtag(prod)
	file_to_check = '{0}_rpdfWS_cat{1}_{2}_{3}jet_.root'.format(ch,cat,prodtag, jet)
#	file_to_check = '{0}_spline_WS_cat{1}_prod{2}_vbf{3}.root'.format(ch, cat, prod, jet)
	check_path = os.path.join(import_dir,'rpdfWS_withCat',file_to_check)
	if not os.path.exists(check_path):
		notfoundall = True
		file_not_found += file_to_check
		file_not_found += '\n'
	return notfoundall, file_not_found

notfoundfilelist=[]
for ch in channels:
	for cat in range(quad):
		for prod in range(numProdCate):
			for jet in range(numJetCate):
				needtorerun, file_not_found = check_path_exists(ch, cat, prod, jet)			
				if needtorerun: 	
					print file_not_found
					notfoundfilelist.append(file_not_found)
if os.path.exists('notfoundfilelist.txt'):
	os.system('rm notfoundfilelist.txt')
with open('notfoundfilelist.txt','w+') as outfile:
	
	outfile.write(''.join(notfoundfilelist))
outfile.close()

