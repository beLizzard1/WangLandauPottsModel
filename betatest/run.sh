#!/bin/bash
for i in `seq 0.01 0.01 2.0`; do rm -fr $i; rm -f output; rm -f  energy.dat; rm -f magnetisation.dat; rm -f acceptance.dat; done
for i in `seq 0.01 0.01 2.0`; do mkdir $i; cp ../template.cfg $i/param.cfg; echo "beta = $i" >> $i/param.cfg; cp ../potts.app $i/potts.app; cd $i; ./potts.app param.cfg >> output; cd ..; done
for i in `seq 0.01 0.01 2.0`; do cat $i/energy.dat >> energy.dat; done
for i in `seq 0.01 0.01 2.0`; do cat $i/magnetisation.dat >> magnetisation.dat; done
for i in `seq 0.01 0.01 2.0`; do cat $i/output >> acceptance.dat; done

gnuplot plotenergy.plot
gnuplot plotmagnetisation.plot
gnuplot plotacceptance.plot
