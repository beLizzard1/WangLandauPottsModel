#!/bin/bash

rm param.cfg
rm -r dim*

echo "dim_q = 4" >> param.cfg
echo "o_nearestneighbour = 1;" >> param.cfg
echo "target_e = -70.0;" >> param.cfg
echo "target_width = 1.0;" >> param.cfg
echo "n_measurements = 10000;" >> param.cfg

for i in `seq 15 1 15`
do
	for j in `seq 0.01 0.01 3.0`
	do
		mkdir -p dim$i/$j
		cp param.cfg dim$i/$j/param.cfg
		echo "beta = $j" >> dim$i/$j/param.cfg
		echo "dim_grid = $i" >> dim$i/$j/param.cfg
		cp ../potts.app dim$i/$j/potts.app
		cd dim$i/$j
		echo "Starting beta=$j dim=$i"
		./potts.app param.cfg
		echo "Finished"
		cat energy.dat >> ../energy.dat
		cat magnetisation.dat >> ../magnetisation.dat
		cd ../..
	done
done
