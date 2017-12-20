import argparse
from tmplobj import tmplobj

header = """{
        "inputDirectory":"<inputdir>",
        "outputFile":"<outputfile>",
        // template definitions
        "templates":[

                
"""

footer='	]\n}'


def writejsonfile(outputfile, inputdir, tmplobjs):
	strtowrite=header.replace('<inputdir>',inputdir)
	print strtowrite
	strtowrite=strtowrite.replace('<outputfile>', outputfile)
	for obj in tmplobjs:
		strtowrite+=obj.writetojson()
	strtowrite+=footer
	with open('tmpl.json','w+') as f:
		f.write(strtowrite)


if __name__=="__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("-p",dest="process",action="append", required=True, help="processes to write")
	parser.add_argument("--output",dest="outputfile",type=str,help="filename ends with .root")
#	parser.add_argument("--input", dest="inputdir", type=str, help="input json file for template building")
	parser.add_argument("--dir", dest="inputdir", type=str, help="Directory for input root files.", default='/eos/user/w/wahung/Mass_Width_Measurement/Data_Sample/170623/')
	parser.add_argument("--vars", action="append", dest="vars", type=str, help="variables in template, position sensitive. 1st=x 2nd=y.",default=['ZZMass','dbkg_kin'])
	parser.add_argument("--savelog", dest="savelog", type=int, help="Record details of inputfile", default=0)
	parser.add_argument("-n","--nbin", dest="nbin", type=str, help="Number of bins in x and y in form of xbin,ybin. Default is 141,20", default='141,20')
	args = parser.parse_args()

	listofobjs=[]
#	print args.process
	for process in args.process:
#		print type(process)
		listofobjs.append(tmplobj(process=process,vars=args.vars))
	writejsonfile(args.outputfile,args.inputdir,listofobjs)
