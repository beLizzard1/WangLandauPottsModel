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
	double beta, target_e, target_width;
	if ( read_input_libconf(filename,&target_e, &target_width, &dim_q, &o_nn, &dim_grid, &beta) != 0 ){
		std::cout << "Error parsing input" << std::endl;
		return(1);
	}

	if ( dim_q == 2){
		std::cout << "Potts q="<<dim_q<<" (Ising) Model on a "<<dim_grid<<"x"<<dim_grid<<" lattice" << std::endl;
	} else {
		std::cout << "Potts q="<<dim_q<<" Model on a "<<dim_grid<<"x"<<dim_grid<<" lattice" << std::endl;
	}
	/* Now initialising the class */
	POTTS_MODEL potts(dim_q,o_nn,dim_grid,beta);

	potts.SCRAMBLE_GRID();
	//potts.FORCE_ALIGN_GRID();

	/* telling the class what target it needs to hit and exist inside */
	potts.SET_TARGET(target_e, target_width);
	
	std::cout << potts.ENERGY_CALC() << std::endl;
	while(potts.OUTSIDE_ENERGY_BAND()){
		for(unsigned int j = 0; j <potts.size; j++){
			for(unsigned int i = 0; i < potts.size; i++){
				potts.SPIN_CHANGE_ENERGY_DIFF(i,j);
				std::cout << potts.ENERGY_CALC() << std::endl;
			}
		}
	}

	std::ofstream lattice;
	lattice.open ("lattice.lat");
	for(unsigned int j = 0; j < potts.size; j++){
		for(unsigned int i = 0; i < potts.size; i++){
			lattice << potts.grid[i][j] << " ";
		}
		lattice << std::endl;
	}
	lattice.close();

	return(0);
}
