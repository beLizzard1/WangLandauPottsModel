#!/bin/bash
for i in `seq 0.01 0.01 1.0`; do rm -r $i; rm output; rm energy.dat; rm magnetisation.dat; rm acceptance.dat; done
for i in `seq 0.01 0.01 1.0`; do mkdir $i; cp ../template.cfg $i/param.cfg; echo "beta = $i" >> $i/param.cfg; cp ../potts.app $i/potts.app; cd $i; ./$i/potts.app $i/param.cfg >> $i/output; cd ..; done
for i in `seq 0.01 0.01 1.0`; do cat $i/energy.dat >> energy.dat; done
for i in `seq 0.01 0.01 1.0`; do cat $i/magnetisation.dat >> magnetisation.dat; done
for i in `seq 0.01 0.01 1.0`; do cat $i/output >> acceptance.dat; done

gnuplot plotenergy.plot
gnuplot plotmagnetisation.plot
gnuplot plotacceptance.plot
