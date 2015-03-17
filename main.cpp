#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <random>
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
<<<<<<< HEAD

=======
	unsigned int dim_q;
	unsigned int o_nn;
	unsigned int dim_grid;
	unsigned int nmeasurements;
	double beta, target_e, target_width;
	if ( read_input_libconf(filename,&target_e, &target_width, &dim_q, &o_nn, &dim_grid, &beta, &nmeasurements) != 0 ){
		std::cout << "Error parsing input" << std::endl;
		return(1);
	}
	/*
	if ( dim_q == 2){
		std::cout << "Potts q="<<dim_q<<" (Ising) Model on a "<<dim_grid<<"x"<<dim_grid<<" lattice" << std::endl;
	} else {
		std::cout << "Potts q="<<dim_q<<" Model on a "<<dim_grid<<"x"<<dim_grid<<" lattice" << std::endl;
	}
	*/
	target_e *= beta;

	/* Now initialising the class */
	POTTS_MODEL potts(dim_q,o_nn,dim_grid,beta,nmeasurements);
>>>>>>> 4a8bfaa...  Changes to be committed:

	POTTS_MODEL potts();
	read_input_libconfig(filename,&potts);

<<<<<<< HEAD
	
=======
	/* telling the class what target it needs to hit and exist inside */
	potts.SET_TARGET(target_e, target_width);

	/*
	 * This section of main attempts to force the lattice into a configuration that matches the target energy
	 * because we started out with a metropolois algorithm this isn't needed yet
	 *
	std::cout << potts.ENERGY_CALC() << std::endl;
	while(potts.OUTSIDE_ENERGY_BAND()){
		for(unsigned int j = 0; j <potts.size; j++){
			for(unsigned int i = 0; i < potts.size; i++){
				potts.SPIN_CHANGE_ENERGY_DIFF(i,j);
				//std::cout.width(2);
				std::cout << potts.ENERGY_CALC() << std::endl;
			}
		}
	}
	*/

	/* Add a variable that tells DO_UPDATE what type of update algorithm to use */
	UPDATE_ALG ALG;
	ALG = METROPOLIS;
>>>>>>> 4a8bfaa...  Changes to be committed:

	/* Do some thermalisation */
	for(unsigned int i = 0; i < 10000; i++){
		potts.DO_UPDATE(ALG);
	}

	/* At this point, because we aren't doing typical monte carlo we were taught before we can assume we are thermalised */

<<<<<<< HEAD


=======
	for(unsigned int i = 0; i < potts.nmeasurements; i++){
		potts.DO_MEASUREMENTS(i);
		potts.DO_UPDATE(ALG);
	}

	potts.ERROR_CALC();
	std::cout << beta << " " << potts.energy_avg << " " << potts.energy_err << std::endl;
	//std::cout << "Magnetisation: " << potts.magnetisation_avg << "\u00B1" << potts.magnetisation_err << std::endl;




	
	std::ofstream lattice;
	lattice.open ("lattice.lat");
	for(unsigned int j = 0; j < potts.size; j++){
		for(unsigned int i = 0; i < potts.size; i++){
			lattice << potts.grid[i][j] << " ";
		}
		lattice << std::endl;
	}
	lattice.close();


>>>>>>> 4a8bfaa...  Changes to be committed:
	return(0);
}
