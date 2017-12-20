import json
import os
import re
from itertools import product
#from utilities import cache

def getfilename(process, subprocess, channel):
	process = process.lower()
	if subprocess == 'bkg': 				bkgstr='Contin'
	elif subprocess=='sig' and process == 'ggzz':		bkgstr='0MH125'
	elif subprocess=='sig' and process == 'vbf':		bkgstr='0PMH125'
	if process == 'qqzz':
		return 'ZZTo4lext/ZZ4lAnalysis.root'
	elif process == 'ggzz':
		return 'ggTo{tag}_{bkgstr}_MCFM701/ZZ4lAnalysis.root'.format(tag=channel,bkgstr=bkgstr)
	elif process == 'vbf':
		return 'VBFTo{tag}JJ_{bkgstr}_phantom128/ZZ4lAnalysis.root'.format(tag=channel,bkgstr=bkgstr)


def getsubprocess(process):
	process = process.lower()
	if process == 'ggzz' or process == 'vbf':
		return ['bkg','sig']
	else:
		return ['bkg']

def getadditionalselection(process,ch):
	if process == 'qqzz':
		if ch =='4e':		return '&&abs(Z1Flav*Z2Flav)==169*169'
		elif ch=='2e2mu':	return '&&abs(Z1Flav*Z2Flav)==169*121'
		elif ch=='4mu':		return '&&abs(Z1Flav*Z2Flav)==121*121'
	else:
		return ''

class tmplobj(object):
	def __init__(self, **kwargs):
		if isinstance(kwargs['process'],list):
			self.process=kwargs['process'][0]
		else:	self.process=kwargs['process']
		self.vars=kwargs['vars']
		self.channels=['4e','4mu','2e2mu']
#		self.weight = float(kwargs['weight'])

	def writetojson(self):
		returnstr=''
		listoffiles=self.getlistoffiles()
		print listoffiles
		for ch, listsubprocessdict in listoffiles.items():
			for subprocessdict in listsubprocessdict:
				for subprocess,fileloc in subprocessdict.items():
					if isinstance(fileloc, str):
						filelocstr=fileloc
						os.system('echo {fileloc} >> log.txt'.format(fileloc=filelocstr))
					else:
						raise typeError('Error in file location')
					jsontmplname = '_'.join([self.process,subprocess,ch])
					additionalselection = getadditionalselection(self.process,ch)
					strtowrite = tmplstr.replace('<jsontmplname>',jsontmplname)
					strtowrite = strtowrite.replace('<listoffile>',filelocstr)
					strtowrite = strtowrite.replace('<var1>',self.vars[0])
					strtowrite = strtowrite.replace('<var2>',self.vars[1])
					strtowrite = strtowrite.replace('<additionalselection>',additionalselection)
#				strtowrite = strtowrite.replace('<weight>',self.weight)
					returnstr+=strtowrite
		return returnstr

#	@cache
	def getlistoffiles(self):
		filepathroot = 'root://lxcms03//data3/Higgs/170623'
		listoffiles={}
		subprocess = getsubprocess(self.process)
		for ch in self.channels:
			listoffiles[ch]=[]	
		for ch,sub in product(self.channels, subprocess):
			filename = getfilename(self.process, sub, ch)
		#	fileloc = os.path.join(filepathroot,filename)
			listoffiles[ch].append({sub:filename})
		return listoffiles

	@property
	def nbins(self, nbinsstr):
		if not isinstance(nbinsstr, str) or "," not in nbinsstr:
			raise TypeError("Input for nbins do not have the right format")
		else:
			nbinsstr = re.split(',',nbinsstr)
			xnbin, ynbin = int(nbinsstr[0]), int(nbinsstr[1])
		return [xnbin, ynbin]


tmplstr="""                {       
                        "name":"<jsontmplname>",
                        "files":[
                                "<listoffile>"
                                ],
                        "tree":"candTree",
                        "variables":["<var1>","<var2>"],
//                        "weight":"<weight>",
                        "selection":"ZZMass>104.875 && ZZMass<141.125 <additionalselection>",
			"conserveSumOfWeights":true,
//                      "assertion":"D_g1_vs_g4_phi0_cal >= 0. && D_g1_vs_g4_phi0_cal <= 1.",
                        "binning":{
                                "type":"fixed",
                                "bins":[70,105.,140.,10,0.,1.0]
                        },
                        "postprocessing":[
                                {"type":"smooth", "kernel":"adaptive"}
//                                {"type":"reweight", "axes":[1]}
                        ]
                },\n"""
