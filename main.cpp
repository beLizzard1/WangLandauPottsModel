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

	if ( read_input_libconf(filename,&target_e, &target_width, &dim_q, &o_nn, &dim_grid, &beta, &nmeasurements, &aguess, &wanglandau) != 0 ){
		std::cout << "Error parsing input" << std::endl;
		return(1);
	}

	/* Now initialising the class */
	POTTS_MODEL potts(dim_q,o_nn,dim_grid,beta,nmeasurements);

	potts.SCRAMBLE_GRID();
	//potts.FORCE_ALIGN_GRID();

	/* telling the class what target it needs to hit and exist inside */
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
	while(potts.OUTSIDE_ENERGY_BAND() && ALG == WANGLANDAU){
		for(unsigned int j = 0; j < potts.size; j++){
			for(unsigned int i = 0; i < potts.size; i++){
				potts.SPIN_CHANGE_ENERGY_DIFF(i,j);
				//std::cout << potts.ENERGY_CALC() / (potts.size * potts.size) << std::endl;
			}
		}
	}



	// Do some thermalisation
	for(unsigned int i = 0; i < 100; i++){
		potts.DO_UPDATE(ALG);
	}
	
	/* Reset acceptance to 0*/
	potts.acceptance = 0;

	for(unsigned int i = 0; i < potts.nmeasurements; i++){
		potts.DO_UPDATE(ALG);
		potts.DO_MEASUREMENTS(i,ALG);
		aguessf << potts.aguess << std::endl;
	}

	potts.ERROR_CALC();


	std::ofstream acceptance;
	acceptance.open("acceptance.dat");
	acceptance << beta << " " << (double)potts.acceptance / (double)potts.nmeasurements << std::endl;
	acceptance.close();

	std::ofstream energy;
	energy.open("energy.dat");
	energy << beta << " " << potts.energy_avg << " " << potts.energy_err << std::endl;
	energy.close();

	std::ofstream magnetisation;
	magnetisation.open("magnetisation.dat");
	magnetisation << beta << " " << potts.magnetisation_avg << " " << potts.magnetisation_err << std::endl;
	magnetisation.close();

	//std::ofstream lattice;
	//lattice.open ("lattice.lat");
	//for(unsigned int j = 0; j < potts.size; j++){
	//	for(unsigned int i = 0; i < potts.size; i++){
	//		lattice << potts.grid[i][j] << " ";
	//	}
	//	lattice << std::endl;
	//}
	//lattice.close();

	return(0);
}
