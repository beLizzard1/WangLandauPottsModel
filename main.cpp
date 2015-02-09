#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <random>

#include <mgl2/qt.h>
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

	//read_input_libconf(std::string file,double *target_e, double *target_width, unsigned int *dim_q, unsigned int *   o_nearestneighbour, unsigned int *dim_grid, double *beta)
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


	std::default_random_engine generator(potts.seed);
	std::uniform_int_distribution<int> distribution(0, dim_grid - 1);

	mglQT gr(&potts,"Draw Lattice");

	while(potts.OUTSIDE_ENERGY_BAND()){
		potts.SPIN_CHANGE_ENERGY_DIFF(distribution(generator),distribution(generator));
		printf("%lf\n",potts.ENERGY_CALC());
		return gr.Run();
	}

	//potts.SPIN_CHANGE_ENERGY_DIFF(distribution(generator),distribution(generator));

	/* Commented out DRAW_LATTICE until I can figure out how to opengl it (much faster and doesn't require time to redraw after each loop */

	return(0);
}
