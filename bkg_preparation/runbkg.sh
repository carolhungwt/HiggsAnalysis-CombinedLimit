#vbfcate=0 ggh	1 vbf 2 RSE 3 TLE 4 inclusive
vbfcate=0
highmass=0
for chan in 4e 4mu
do
cat=0
while [ $cat -lt 9 ]
do
	root -l -q -b bkgWorkspace_withCat.c++\(\"${chan}\",${vbfcate},${highmass},${cat}\)
((cat+=1))
done
done
