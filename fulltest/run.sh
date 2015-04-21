#!/bin/bash

#Need to vary dimensionality, beta and q
#Do Q first

parallel 'cd {1}/{2}/{3}; echo $PWD; ./potts.app param.cfg; cd ../../../' ::: 2 3 4 6 8 10 ::: 8 12 16 24 32 48 64 ::: `seq 0.0 0.1 2.8`
