rm -R -- */
rm output.dat

filename='target.list'
filelines=`cat $filename`


for i in $filelines
do
	mkdir energy$i
	cd energy$i
	cp ../param.cfg .
	sed -i '/target_e\s=\s.*/c\target_e = '$i'' param.cfg
	echo "target_e=$i"
	cd ..
done

parallel -j8 --eta "cd energy{}; ../potts.app param.cfg; cd ../" :::: target.list


filelines=`cat $filename`
for i in $filelines
do
	anaverage=`tail -n 30 energy$i/an.dat | awk '{ sum+=$1} END {print sum/30}'`
	energy=$i
	#echo "$anaverage"
	#echo "$energy"
	echo "$energy $anaverage" >> output.dat
done

