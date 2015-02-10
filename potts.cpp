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
#include <mgl2/mgl.h>
#include <mgl2/qt.h>
#include "potts.h"

POTTS_MODEL::POTTS_MODEL(unsigned int dim_q, unsigned int o_nn, unsigned int dim_grid, double b){
	q = dim_q;
	size = dim_grid;
	o_nearestneighbour = o_nn;
	beta = b;
	coupling = 1.0;
	grid = new unsigned int* [size];
	seed = std::chrono::system_clock::now().time_since_epoch().count(); //Generating seed
	generator.seed(seed);
	
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
	std::uniform_int_distribution<unsigned int> distribution(1,q);
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			grid[i][j] = distribution(generator); //Generate a random q value for each lattice point
		}
	}
}

void POTTS_MODEL::FORCE_ALIGN_GRID(){
	/* Chooses a random q value and sticks it as the only value on the grid */
	std::uniform_int_distribution<unsigned int> distribution(1,q);
	unsigned int r_q = distribution(generator);
	for(unsigned int j = 0; j< size; j++){
		for(unsigned int i = 0; i< size; i++){
			grid[i][j] = r_q;
		}
	}
}

void POTTS_MODEL::DRAW(){
	mglGraph gr;
	/*
	double angle;
	unsigned int n_ele = size * size;
	mglData x;
	mglData y;
	double *ax,*ay;
	ax = new double[n_ele];
	ay = new double[n_ele];
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			angle = (2 * M_PI * grid[i][j]) / q;
			ax[i + (size * j)] = cos(angle);
			ay[i + (size * j)] = sin(angle);
		}
	}
	x.Set(ax,n_ele,1,1);
	y.Set(ay,n_ele,1,1);
	gr.Box();
	gr.Vect(x,y,"=.");
	gr.WriteFrame("test.png");
	*/
	mglData a(31,41);
	gr.Fill(a,"-pi*x*exp(-(y+1)^2-4*x^2)");
	gr.Surf(a,"r","yrange 0 1"); gr.Surf(a,"b","yrange 0 -1");
	gr.WriteFrame("test.png");

}

double POTTS_MODEL::ENERGY_CALC(){
	double energy = 0.0;
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			energy += NEAREST_NEIGHBOUR(i,j);
		}
	}
	energy *= -coupling;
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

double POTTS_MODEL::SPIN_CHANGE_ENERGY_DIFF(unsigned int i, unsigned int j){
	unsigned int q_before = grid[i][j];
	double *configuration = new double[q];
	for(unsigned int n = 1; n <= q; n++){
		grid[i][j] = n;
		configuration[n-1] = fabs(target_e - ENERGY_CALC());
	}
	double smallest = configuration[0];
	unsigned int lowest_conf = 0;
	for(unsigned int n = 0; n < q; n++){
		if(smallest < configuration[n]){
			smallest = configuration[n];
			lowest_conf = n;
		}
	}
	if(lowest_conf == 0){
		grid[i][j] = q_before;
	} else {
		grid[i][j] = lowest_conf;
	}
	delete[] configuration;
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

	if( energy <= target_ub && energy >= target_lb ){
		return(0);
	}
	return(1);
}
