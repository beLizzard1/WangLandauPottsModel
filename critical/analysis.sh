#!/bin/bash

for i in `seq 10 1 20`
do
	cd dim$i
	gnuplot ../plotenergy.plot
	gnuplot ../plotmagnetisation.plot
	cd ..
done
