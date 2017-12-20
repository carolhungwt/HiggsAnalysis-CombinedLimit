HiggsAnalysis-CombinedLimit
===========================

### Official documentation

[Manual to run combine](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#How_to_run_the_tool)

### Standalone compilation in `lxplus`
```
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
source env_standalone.sh 
make -j 8; make # second make fixes compilation error of first

#Signal resolution 
cd resolution
bash submit_reso.txt
#extract resolution parameters 
python extractResoParam.py

#Prepare 2D conditional pdfs
cd TemplateBuilder
cd run
#make necessary json file with writejsondict_2dtmpl.py
python writejsondict_2dtmpl.py -p ggH --output signal_tmpl.root -p VBFH -p VH
#Normalize tmpl with Normalize_TH2_all.cc
root -l -q -b Normalize_TH2_all.cc\(\"signal_tmpl.root\"\)

#Convert signal shapes into splines
cd spline_prod
bash make_rpdfWS.sh 
#combine conditional pdfs together with spline pdfs
bash make_Sp_WS_withCat.sh

#Prepare 2D background pdfs
#prepare qqzz.root with qqzz_withDmass.c
root -l -q -b qqzz_withDmass.c
#construct background workspace 
bash runbkg.sh

#Run mass width fit with datacards 
cd Mass-Width/test_datacard_quad9_withtmpl
#create datacards
python createdatacards.py

```
