#!/bin/bash
rm potts.app
rm param.cfg

cp ../potts.app .
cp template.cfg param.cfg
echo "target_e = $1" >> param.cfg
./potts.app param.cfg
gnuplot plotan.plot
