#!/bin/bash
for i in `seq 0.0 0.01 1.5`;
do 
	rm -fr $i;
       	rm -f energy.dat;
       	rm -f magnetisation.dat;
       	rm -f acceptance.dat;
	rm -f susceptibility.dat;
	rm -f specificheat.dat;
done

for i in `seq 0.0 0.01 1.5`; do 
	mkdir $i; cp template.cfg $i/param.cfg;
	echo "beta = $i" >> $i/param.cfg;
	cp ../potts.app $i/potts.app;
       	cd $i; ./potts.app param.cfg;
	cd ..;
done

for i in `seq 0.00 0.01 1.5`; do cat $i/energy.dat >> energy.dat; done
for i in `seq 0.00 0.01 1.5`; do cat $i/magnetisation.dat >> magnetisation.dat; done
for i in `seq 0.00 0.01 1.5`; do cat $i/acceptance.dat >> acceptance.dat; done
for i in `seq 0.00 0.01 1.5`; do cat $i/susceptibility.dat >> susceptibility.dat; done
for i in `seq 0.00 0.01 1.5`; do cat $i/specificheat.dat >> specificheat.dat; done
gnuplot plotenergy.plot
gnuplot plotmagnetisation.plot
gnuplot plotacceptance.plot
gnuplot plotsusceptibility.plot
gnuplot plotspecificheat.plot
