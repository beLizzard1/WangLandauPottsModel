rm -R -- */

for l in 8 10 12 14 16 18 20
do
	echo "Starting $l"
	mkdir grid-$l
	cd grid-$l
	cp ../param.cfg .
	cp ../target$l.list target.list
	filename='target.list'
	filelines=`cat $filename`

	sed -i '/target_width\s=\s.*/c\target_width = '$l'.0' param.cfg
	sed -i '/dim_grid\s=\s.*/c\dim_grid = '$l'' param.cfg
	for i in $filelines
	do
		mkdir energy$i
		cd energy$i
		cp ../param.cfg .
		sed -i '/target_e\s=\s.*/c\target_e = '$i'' param.cfg
		cd ..
	done
	
	parallel -j8 --eta "cd energy{}; ../../potts.app param.cfg; cd ../" :::: target.list

	cd ..
done

for l in 8 10 12 14 16 18 20
do
	cd grid-$l
	filelines=`cat $filename`
	for i in $filelines
	do
		anaverage=`tail -n 50 energy$i/an.dat | awk '{sum+=$1} END {print sum/50}'`
		energy=$i
		echo "$energy $anaverage" >> output.dat
	done
	
	cd ..
done 
