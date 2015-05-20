rm -R -- */

for q in 2 3 4 8 10
do
	mkdir Q$q
	cd Q$q
		
	for grid in 8 10 12 14 16 18 20 22 24 26 28 30 32
	do
		mkdir $grid
		cd $grid
		cp ../../param.cfg .
		sed -i '/dim_grid\s=\s.*/c\dim_grid = '$grid'' param.cfg
		sed -i '/dim_q\s=\s.*/c\dim_q = '$q'' param.cfg
		betacrit=`echo "l(1+ sqrt($q))"|bc -l`
		echo $betacrit
		sed -i '/beta\s=\s.*/c\beta = '$betacrit'' param.cfg
		cd ..
	done

	seq 8 2 32 | parallel -j16 --eta "cd {}; ../../potts.app param.cfg; cd ../"

	
	
	cd ..
done
