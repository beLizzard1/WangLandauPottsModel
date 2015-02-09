#define _USE_MATH_DEFINES
#include <cstdio>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <random>
#include <cmath>
#include <cstdlib>
#include "potts.h"

POTTS_MODEL::POTTS_MODEL(unsigned int dim_q, unsigned int o_nn, unsigned int dim_grid, double b){
	q = dim_q;
	size = dim_grid;
	o_nearestneighbour = o_nn;
	beta = b;
	coupling = 1.0;
	grid = new unsigned int* [size];
	seed = std::chrono::system_clock::now().time_since_epoch().count(); //Generating seed
	for(unsigned int i = 0; i < size; i++){
		grid[i] = new unsigned int[size];
	}
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			/* Setting 0 as unassigned */
			grid[i][j] = 0;
		}
	}
}

void POTTS_MODEL::SCRAMBLE_GRID(){
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(1,q);
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			grid[i][j] = distribution(generator); //Generate a random q value for each lattice point
		}
	}
}

void POTTS_MODEL::FORCE_ALIGN_GRID(){
	/* Chooses a random q value and sticks it as the only value on the grid */
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(1,q);
	unsigned int r_q = distribution(generator);
	for(unsigned int j = 0; j< size; j++){
		for(unsigned int i = 0; i< size; i++){
			grid[i][j] = r_q;
		}
	}
}

void POTTS_MODEL::DRAW_LATTICE(){
	/* Currently outputs loads of vectors for a gnuplot vector render */
	double angle, x, y;
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			angle = (2 * M_PI * grid[i][j]) / q;
			x = cos(angle);
			y = sin(angle);
			printf("%lf %lf %lf %lf\n", (double)i,(double)j,x,y);
		}
	}
}

double POTTS_MODEL::ENERGY_CALC(){
	double energy = 0.0;
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			energy += NEAREST_NEIGHBOUR(i,j);
		}
	}

	energy *= -1 * coupling;

	return(energy);	
}

int POTTS_MODEL::NEAREST_NEIGHBOUR(unsigned int i, unsigned int j){
	/* Regular Lattice with a fixed size need to do periodic boundary conditions */
	/* modulo operator should be fine for those terms */
	double counter = 0.0;
	if(o_nearestneighbour == 0){
		return(0);
	} else {
		/* Doesn't actually get the nearest neighbours because it misses the diagonal :( might improve this at some point */
		/* Looks to the nearest neighbours to the right */
		for(unsigned int n = 1; n <= o_nearestneighbour; n++){
			if( (grid[i][j] == grid[(i+n)%size][j]) ){
				counter += 1/n;
			}
		}
		/* Looks to the nearest neighbours to the left */
		for(unsigned int n = 1; n <= o_nearestneighbour; n++){
			if( (grid[i][j] == grid[i][(j+n)%size]) ){
				counter += 1/n;
			}
		}
	}
	return(counter);
}


/*
   double POTTS_MODEL::SPIN_CHANGE_ENERGY_DIFF(unsigned int i, unsigned int j){
   double pre_change, post_change;
   pre_change = ENERGY_CALC();
// Read the q value
unsigned int old_q = grid[i][j];
//Ensuring that q isn't the same as the original value 
double *energy_conf;
energy_conf = new double [q];
energy_conf[0] = target_e - pre_change;
for(unsigned int n = 1; n <= q; n++){
if(n == old_q){
break;
}
grid[i][j] = n;
// Construct an array of energies which correspond to each value of q that was tried 
energy_conf[n] = target_e - ENERGY_CALC();
}
// Now need to choose one that goes towards the target energy
double small = energy_conf[0];
unsigned int whichelement = 0;
for(unsigned int n = 0; n < q; n++){
if( energy_conf[n] <= small ){
small = energy_conf[n];
whichelement = n;
}
}
if(whichelement == 0){
std::cout << "I chose the original q value "<< old_q << std::endl;
grid[i][j] = old_q;
} else {
std::cout << "I chose a new q value "<<whichelement << std::endl;
grid[i][j] = whichelement;
}

for(unsigned int i = 0; i < q; i++){
printf("%lf\n",energy_conf[i]);
}

post_change = ENERGY_CALC();
return(post_change);
}
*/

double POTTS_MODEL::SPIN_CHANGE_ENERGY_DIFF(unsigned int i, unsigned int j){
	unsigned int q_before = grid[i][j];

	double *configuration = new double[q];

	for(unsigned int n = 1; n <= q; n++){
		grid[i][j] = n;
		configuration[n-1] = fabs ( ENERGY_CALC() - target_e );
	}

	double smallest = configuration[0];
	int lowest_conf = 0;
	for(unsigned int n = 0; n < q; n++){
		if(smallest > configuration[n]){
			smallest = configuration[n];
			lowest_conf = n;
		}
	}

	if( lowest_conf == 0 ){
		grid[i][j] = q_before;
	} else {
		grid[i][j] = lowest_conf;
	}

	return(ENERGY_CALC());
}


void POTTS_MODEL::SET_TARGET(double t_energy, double t_width){
	target_e = t_energy;
	target_width = t_width;
}

POTTS_MODEL::~POTTS_MODEL(){
	for(unsigned int i = 0; i < size; i++){
		delete[] grid[i];
	}
	delete[] grid;
}

int POTTS_MODEL::OUTSIDE_ENERGY_BAND(){
	double target_lb, target_ub;
	target_lb = target_e - target_width;
	target_ub = target_e + target_width;
	double energy = ENERGY_CALC();

	if( energy < target_ub && target_lb < energy ){
		return(0);
	}
	return(1);
}
