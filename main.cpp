#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <random>

#include <mgl2/mgl.h>
#include <libconfig.h++>

#include "potts.h"
#include "utilityfunctions.h"

int main(int argc, char **argv) {
	std::string filename;
	if(argc != 2){
		std::cout << "No Configuration File provided" << std::endl;
		exit(1);
	} else {
		filename = argv[1];
	}
	unsigned int dim_q;
	unsigned int o_nn;
	unsigned int dim_grid;
	unsigned int nmeasurements;
	double beta, target_e, target_width, aguess;
	bool wanglandau;

	// Read the configuration/parameters file into variables most of which can be immediately put into the initialisation function
	if ( read_input_libconf(filename,&target_e, &target_width, &dim_q, &o_nn, &dim_grid, &beta, &nmeasurements, &aguess, &wanglandau) != 0 ){
		std::cout << "Error parsing input" << std::endl;
		return(1);
	}

	// Now initialising the class that will contain the simulation
	POTTS_MODEL potts(dim_q,o_nn,dim_grid,beta,nmeasurements,aguess);

	//Check that the target energy is sensible. If it is less than 2L^2 exit cause it's annoying
	if((target_e < 0) && (target_e > (-2 * (dim_grid * dim_grid))) ){
		std::cout << "Target Energy is Impossible for current grid size" << std::endl;
		return(2);
	}


	//potts.SCRAMBLE_GRID();
	potts.FORCE_ALIGN_GRID();

	std::cout << "Minimum Energy: " << potts.ENERGY_CALC() << std::endl;
	std::cout << "Maximum Energy: 0" << std::endl;

	// telling the class what target energy band it needs to hit and exist inside
	// required for Wang Landau
	potts.SET_TARGET(target_e, target_width);

	// Add a variable that tells DO_UPDATE what type of update algorithm to use
	UPDATE_ALG ALG;
	if(wanglandau == true){
		ALG = WANGLANDAU;
	} else {
		ALG = METROPOLIS;
	}

	// Force lattice into configuration that we want
	// Kinda like a random heat bath to drive energy to target
	// Required for Entropic Sampling needed in the WANG LANDAU Method
	while(potts.OUTSIDE_ENERGY_BAND() && ALG == WANGLANDAU){
		for(unsigned int j = 0; j < potts.size; j++){
			for(unsigned int i = 0; i < potts.size; i++){
				potts.SPIN_CHANGE_ENERGY_DIFF(i,j);
				std::cout << "Current Energy: " << potts.ENERGY_CALC() << std::endl;
			}
		}
	}

	// Do some thermalisation
	for(unsigned int i = 0; i < 250; i++){
		potts.DO_UPDATE(ALG);
	}

	/* Reset acceptance to 0*/
	potts.acceptance = 0;

	for(unsigned int i = 0; i < potts.nmeasurements; i++){
		potts.DO_UPDATE(ALG);
		potts.DO_MEASUREMENTS(i,ALG);
	}

	potts.ERROR_CALC(ALG);
	if(ALG == WANGLANDAU){
		potts.arrayofan[0] = potts.aguess;
		//Using the Error Calc and the measurements taken, now all you need to do is calculate the next a_n
		potts.aguess += (12 / ((4 * potts.target_width) + (potts.target_width * potts.target_width))) * potts.estar_avg;
		// Now do the a_n iteration loop 100 times should be enough for convergence
		for(unsigned int i = 1; i < potts.nmeasurements; i++){
			for(unsigned int i = 0; i < potts.nmeasurements; i++){
				potts.DO_UPDATE(ALG);
				potts.DO_MEASUREMENTS(i,ALG);
			}
			potts.ERROR_CALC(ALG);
			potts.aguess += (12 / ((4*potts.target_width) + (potts.target_width * potts.target_width))) * potts.estar_avg;
			//Add the new guess to the array
			potts.arrayofan[i] = potts.aguess;
		}


		std::ofstream an;
		an.open("an.dat");
		for(unsigned int l = 0; l < potts.nmeasurements; l++){
			an << l << " " << potts.arrayofan[l] << std::endl;
		}
		an.close();
	}


	if(ALG == METROPOLIS){
		potts.write_metropolis_output();
	}


	return(0);
}
