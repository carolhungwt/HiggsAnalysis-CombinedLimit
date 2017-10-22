#vbfcate=0 ggh	1 vbf 2 RSE 3 TLE 4 inclusive
vbfcate=0
highmass=0
for chan in 2e2mu #4e 4mu
do
for cat in 0 1 2
do
	root -l -q bkgWorkspace_with_Dbkgkin_withCat.c\(\"${chan}\",${vbfcate},${highmass},${cat}\)
done
done
