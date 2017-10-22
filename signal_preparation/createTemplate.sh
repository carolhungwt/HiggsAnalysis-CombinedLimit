for tag in 2e2mu #4mu 4e
	do
	root -l -q dbkgkin_mreco_hist.cc\(\"${tag}\"\)
#	root -l -q dbkgkin_mreco_hist_vbf.cc\(\"${tag}\"\)
done
root -l -q loadLib.C template_spline.cc	#template_spline_vbf.cc
