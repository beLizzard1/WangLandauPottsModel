dimq=4
beta=1.09861229

rm -R -- */
rm *.dat

for grid in 8 10 12 14 16 18 20 22 24
do
	mkdir $grid
	cd $grid
	cp ../param.cfg .
	cp ../potts.app .
	sed -i '/dim_grid\s=\s.*/c\dim_grid = '$grid';' param.cfg

	for i in `seq 1 20`
	do
		sed -i '/dim_q\s=\s.*/c\dim_q = '$dimq'' param.cfg
		sed -i '/beta\s=\s.*/c\beta = '$beta'' param.cfg
		echo "Current Loop: $i"
		./potts.app param.cfg
		cat energy.dat >> total.dat
	done
	cd ..
done
