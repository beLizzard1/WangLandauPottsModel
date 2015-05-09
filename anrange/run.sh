rm -R -- */
rm output.dat
rm Q*.dat

filename='target.list'
filelines=`cat $filename`

gridsize=16
interface=false


sed -i '/interface\s=\s.*/c\interface = '$interface'' param.cfg
sed -i '/dim_grid\s=\s.*/c\dim_grid = '$gridsize'' param.cfg
sed -i '/target_width\s=\s.*/c\target_width = '$gridsize'.0' param.cfg

for q in 2 3 4 8 10
do
	rm -R -- */
	for i in $filelines
	do
		mkdir energy$i
		cd energy$i
		cp ../param.cfg .
		sed -i '/target_e\s=\s.*/c\target_e = '$i'' param.cfg
		sed -i '/dim_q\s=\s.*/c\dim_q = '$q'' param.cfg
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
		echo "$energy $anaverage" >> Q$q.dat
	done
done
