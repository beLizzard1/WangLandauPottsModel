#!/bin/bash

#Need to vary dimensionality, beta and q
#Do Q first
for q in 2 3 4 6 8 10
do
    mkdir $q
    cd $q
    cp ../param.cfg .
    sed -i '/dim_q\s=\s.*/c\dim_q = '$q'' param.cfg

    for dim in 8 12 16 24 32 48 64
    do
        mkdir $dim
        cd $dim
        cp ../param.cfg .
        sed -i '/dim_grid\s=\s.*/c\dim_grid = '$dim'' param.cfg

        for beta in `seq 0.0 0.1 2.8`
        do
            mkdir $beta
            cd $beta
            cp ../param.cfg .
            sed -i '/beta\s=\s.*/c\beta = '$beta'' param.cfg

            #Need to get the executeable in here
            ln -s ../../../potts.app

            cd ..
        done


        cd ..
    done

    cd ..
done
