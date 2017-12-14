import os
import sys
from itertools import izip

class resubmitfile():
	def __init__(self,ch,cat,prod,jet):
		self.quad = 9
		self.cat = cat
		self.prod = prod
		self.jet = jet
		self.ch = ch
	
	def resubmit(self):
		os.system("bash submit_cus.sh {0} {1} {2} {3}".format(self.cat,self.ch,self.prod,self.jet))
		os.system("sleep 1m")

#	def checkcmd(self):
#		os.system("echo bash submit_cus.sh {0} {1} {2} {3} \n> log.txt".format(self.cat,self.ch,self.prod,self.jet))


def giveprodnum(prodtag):
	if prodtag=='ggH':	return 0
	elif prodtag=='vbf':	return 1
	elif prodtag=='vh':	return 2
	else:
		raise ValueError('check prodtag')

def readfilelist():
	files=[]
	with open('notfoundfilelist.txt','r') as filelist:
		allfiles = filelist.readlines()
		for line in allfiles:
			#2e2mu_rpdfWS_cat6_ggH_0jet_
			copy = line.split('_')
			ch = str(copy[0])
			catstr = copy[2]
			cat = int(catstr[-1])
			prodtag = copy[3]
			prod = giveprodnum(prodtag)
			jetstr = copy[-2]	
			jet = jetstr[0]
			files.append(resubmitfile(ch,cat,prod,jet))
	return files	

resubmitlist = readfilelist()
#for i,resubmitfie in izip(range(1),resubmitlist):
for resubmitfie in resubmitlist:
#	resubmitfie.checkcmd()
	resubmitfie.resubmit()
