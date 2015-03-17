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
#include <complex>
#include <cstdlib>
#include <mgl2/mgl.h>
//#include <mgl2/qt.h>
#include "potts.h"


// Class Initialisation, goes though and assigns values
POTTS_MODEL::POTTS_MODEL(){

}

void POTTS_MODEL::write_metropolis_output(){
	std::ofstream acceptancef;
	acceptancef.open("acceptance.dat");
	acceptancef << beta << " " << (double)acceptance / (double)nmeasurements << std::endl;
	acceptancef.close();

	std::ofstream energy;
	energy.open("energy.dat");
	energy << beta << " " << energy_avg << " " << energy_err << std::endl;
	energy.close();

	std::ofstream magnetisation;
	magnetisation.open("magnetisation.dat");
	magnetisation << beta << " " << magnetisation_avg << " " << magnetisation_err << std::endl;
	magnetisation.close();

	std::ofstream susceptibility;
	susceptibility.open("susceptibility.dat");
	susceptibility << beta << " " << susceptibility_avg << " " << susceptibility_err << std::endl;
	susceptibility.close();

	std::ofstream specificheat;
	specificheat.open("specificheat.dat");
	specificheat << beta << " " << specificheat_avg << " " << specificheat_err << std::endl;
	specificheat.close();


	std::ofstream lattice;
	lattice.open ("lattice.lat");
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			lattice << grid[i][j] << " ";
		}
		lattice << std::endl;
	}
	lattice.close();
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
	/*
	   mglGraph gr;
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

}

double POTTS_MODEL::ENERGY_CALC(){

	double energy = 0;
	for(unsigned int j = 0; j < size; j++){
		for(unsigned int i = 0; i < size; i++){
			//energy += NEAREST_NEIGHBOUR(i,j);
			energy += coupling * cos(values[grid[i][j]-1] - values[grid[(i+1)%size][j]-1]);
			energy += coupling * cos(values[grid[i][j]-1] - values[grid[i][(j+1)%size]-1]);
		}
	}
	return(energy);
	energy *= -beta;
	return(energy);
}

int POTTS_MODEL::NEAREST_NEIGHBOUR(unsigned int i, unsigned int j){
	// Regular Lattice with a fixed size need to do periodic boundary conditions
	// modulo operator should be fine for those terms
	double counter = 0.0;
	if(o_nearestneighbour == 0){
		return(0);
	} else {
		// Doesn't actually get the nearest neighbours because it misses the diagonal :( might improve this at some point
		// Looks to the nearest neighbours to the right
		for(unsigned int n = 1; n <= o_nearestneighbour; n++){
			if( (grid[i][j] == grid[(i+n)%size][j]) ){
				counter ++;
			}
		}
		// Looks to the nearest neighbours below
		for(unsigned int n = 1; n <= o_nearestneighbour; n++){
			if( (grid[i][j] == grid[i][(j+n)%size]) ){
				counter ++;
			}
		}
	}
	return(counter);
}

void POTTS_MODEL::DO_MEASUREMENTS(unsigned int k,UPDATE_ALG TYPE){
	switch(TYPE){
		case METROPOLIS:{

					energy[k] = 0.0;
					magnetisation[k] = 0.0;
					for(unsigned int j = 0; j < size; j++){
						for(unsigned int i = 0; i < size; i++){
							magnetisation[k] += cos(values[grid[i][j] - 1]);
						}
					}
					/* Going to use preexisting energy calculation function to do the measurements */
					energy[k] = ENERGY_CALC();
					/* Now to divide by volume */
					//energy[k] /= (size * size);
					magnetisation[k] = fabs(magnetisation[k]) / (size * size);
					//energy[k] /= (size*size);
					specificheat[k] = energy[k] * energy[k];
					susceptibility[k] = magnetisation[k]*magnetisation[k];
					break;
				}
		case WANGLANDAU:{
					energy[k] = 0.0;
					magnetisation[k] = 0.0;
					for(unsigned int j = 0; j < size; j++){
						for(unsigned int i = 0; i < size; i++){
							magnetisation[k] += cos(values[grid[i][j] - 1]);
						}
					}
					energy[k] = ENERGY_CALC();
					energy[k] /= (size * size);
					magnetisation[k] = fabs(magnetisation[k])/(size*size);
					specificheat[k] = energy[k] * energy[k];
					susceptibility[k] = magnetisation[k];
					estar[k] = (ENERGY_CALC() - target_e);
					break;
				}
		default:{
				std::cout << "You should never be seeing this unless you've dun goof!" << std::endl;
				break;
			}
	}
	/* Going to use preexisting energy calculation function to do the measurements */
	energy[k] = ENERGY_CALC();
	/* Now to divide by volume */
	energy[k] /= (size * size);
	magnetisation[k] = fabs(magnetisation[k]) / (size * size);
}

void POTTS_MODEL::DO_UPDATE(UPDATE_ALG TYPE){
	unsigned int x,y,old_q,new_q;
	double H_old, H_new, rand;
	switch(TYPE){
		case METROPOLIS:{
					std::uniform_int_distribution<unsigned int> distribution(0,size-1);
					x = distribution(generator);
					y = distribution(generator);

					old_q = grid[x][y];

					std::uniform_int_distribution<unsigned int> qdistribution(1,q);

					new_q = qdistribution(generator);

					H_old = ENERGY_CALC();

					grid[x][y] = new_q;

					H_new = ENERGY_CALC();

					std::uniform_real_distribution<double> pdistribution(0,1);
					rand = pdistribution(generator);

					double delta = H_old - H_new;

					if (delta <= 0){
						grid[x][y] = new_q;
						acceptance++;
					}
					if (delta > 0){
						if(exp(-1 * beta * delta) >= rand){
							grid[x][y] = new_q;
							acceptance++;
						} else{
							grid[x][y] = old_q;
						}
					}

					break;
				}
		case WANGLANDAU:{
					std::uniform_int_distribution<unsigned int> distribution(0,size-1);
					x = distribution(generator);
					y = distribution(generator);

					old_q = grid[x][y];

					H_old = ENERGY_CALC();

					std::uniform_int_distribution<unsigned int> qdistribution(1,q);

					new_q = qdistribution(generator);

					grid[x][y] = new_q;
					H_new = ENERGY_CALC();

					double delta = H_new - H_old;
					if( OUTSIDE_ENERGY_BAND() == 1 ){
						grid[x][y] = old_q;
						break;
					} else {
						std::uniform_real_distribution<double> pdistribution(0,1);
						rand = pdistribution(generator);
						if (delta <= 0){
							grid[x][y] = new_q;
							acceptance++;
						}
						if (delta > 0){
							if(exp(-1 * aguess * (H_new - target_e)) >= rand){
								grid[x][y] = new_q;
								acceptance++;
							} else{
								grid[x][y] = old_q;
							}
						}

					}
					break;
				}

					if( exp(beta * (H_new - H_old)) > 1 ){
						grid[x][y] = new_q;
					} else {
						if(rand < exp(beta * (H_new - H_old))){
							grid[x][y] = new_q;
						} else {
							grid[x][y] = old_q;
						}
					}

					break;
				}
		case WANGLANDAU:
				std::cout << "Not yet programmed" << std::endl;
				break;
		default:
				std::cout << "Not needed really" << std::endl;
				break;
	}
}

void POTTS_MODEL::ERROR_CALC(UPDATE_ALG TYPE){

	switch(TYPE){
		case METROPOLIS:{

					double *bin, *jackbins;
					bin = new double[numbins];
					jackbins = new double[numbins];

					double sumbins;

					for(unsigned int l = 0; l < nmeasurements; l++){
						energy_avg += energy[l];
						magnetisation_avg += magnetisation[l];
					}
					energy_avg /= nmeasurements;
					magnetisation_avg /= nmeasurements;

					/* Binning the Data for energy */
					unsigned int slice = nmeasurements / numbins;
					sumbins = 0.0;
					for(unsigned int l = 0; l < numbins; l++){
						bin[l] = 0.0;
						for( unsigned int k = 0; k < slice; k++){
							bin[l] += energy[l * slice + k];
						}
						bin[l] /= slice;
						sumbins += bin[l];
					}
					// Forming the bins
					for(unsigned int l = 0; l < numbins; l++){
						jackbins[l] = (sumbins - bin[l]) / (numbins - 1);
					}

					energy_err = 0.0;
					for(unsigned int l = 0; l < numbins; l++){
						energy_err += (energy_avg - jackbins[l]) * (energy_avg - jackbins[l]);
					}
					energy_err *= (numbins - 1.0) / (double)numbins;
					energy_err = sqrt(energy_err);

					/* Now the same for magnetisation */
					sumbins = 0.0;
					for(unsigned int l = 0; l < numbins; l++){
						bin[l] = 0.0;
						for( unsigned int k = 0; k < slice; k++){
							bin[l] += magnetisation[l * slice + k];
						}
						bin[l] /= slice;
						sumbins += bin[l];
					}
					// Forming the bins
					for(unsigned int l = 0; l < numbins; l++){
						jackbins[l] = (sumbins - bin[l]) / (numbins - 1);
					}

					magnetisation_err = 0.0;
					for(unsigned int l = 0; l < numbins; l++){
						magnetisation_err += (magnetisation_avg - jackbins[l]) * (magnetisation_avg - jackbins[l]);
					}
					magnetisation_err *= (numbins - 1.0) / (double)numbins;
					magnetisation_err = sqrt(magnetisation_err);


					//Specific Heat Cv=beta^2 * average of e^2 - (average e)^2
					specificheat_err = energy_err;

					specificheat_avg = 0;
					for(unsigned int l = 0; l < nmeasurements; l++){
						specificheat_avg += specificheat[l];
					}
					specificheat_avg /= nmeasurements;
					specificheat_avg -= (energy_avg * energy_avg);

					specificheat_avg *= (beta * beta);

					//Susceptibility chi=beta * average of m^2 - (average M)^2
					susceptibility_err = magnetisation_err;

					susceptibility_avg = 0;
					for(unsigned int l = 0; l < nmeasurements; l++){
						susceptibility_avg += susceptibility[l];
					}
					susceptibility_avg /= nmeasurements;
					susceptibility_avg -= (magnetisation_avg * magnetisation_avg);

					susceptibility_avg *= beta;

					break;
				}
		case WANGLANDAU:{
					//E Star
					for(unsigned int l = 0; l < nmeasurements; l++){
						estar_avg += estar[l];
					}
					estar_avg /= nmeasurements;
					break;
				}
		default:{
				     std::cout << "Shouldn't be seeing this" << std::endl;
				     break;
			     }

void POTTS_MODEL::ERROR_CALC(){
	double *bin, *jackbins;
	bin = new double[numbins];
	jackbins = new double[numbins];

	double sumbins;

	for(unsigned int l = 0; l < nmeasurements; l++){
		energy_avg += energy[l];
		magnetisation_avg += magnetisation[l];
	}
	energy_avg /= nmeasurements;
	magnetisation_avg /= nmeasurements;

	/* Binning the Data for energy */
	unsigned int slice = nmeasurements / numbins;
	sumbins = 0.0;
	for(unsigned int l = 0; l < numbins; l++){
		bin[l] = 0.0;
		for( unsigned int k = 0; k < slice; k++){
			bin[l] += energy[l * slice + k];
		}
		bin[l] /= slice;
		sumbins += bin[l];
	}
	// Forming the bins
	for(unsigned int l = 0; l < numbins; l++){
		jackbins[l] = (sumbins - bin[l]) / (numbins - 1);
	}

	energy_err = 0.0;
	for(unsigned int l = 0; l < numbins; l++){
		energy_err += (energy_avg - jackbins[l]) * (energy_avg - jackbins[l]);
	}
	energy_err *= (numbins - 1.0) / (double)numbins;
	energy_err = sqrt(energy_err);

	/* Now the same for magnetisation */
	sumbins = 0.0;
	for(unsigned int l = 0; l < numbins; l++){
		bin[l] = 0.0;
		for( unsigned int k = 0; k < slice; k++){
			bin[l] += magnetisation[l * slice + k];
		}
		bin[l] /= slice;
		sumbins += bin[l];
	}
	// Forming the bins
	for(unsigned int l = 0; l < numbins; l++){
		jackbins[l] = (sumbins - bin[l]) / (numbins - 1);
	}

	magnetisation_err = 0.0;
	for(unsigned int l = 0; l < numbins; l++){
		magnetisation_err += (magnetisation_avg - jackbins[l]) * (magnetisation_avg - jackbins[l]);
	}
	magnetisation_err *= (numbins - 1.0) / (double)numbins;
	magnetisation_err = sqrt(magnetisation_err);
}

double POTTS_MODEL::SPIN_CHANGE_ENERGY_DIFF(unsigned int i, unsigned int j){
	unsigned int q_before = grid[i][j];
	double *configuration = new double[q];
	for(unsigned int n = 1; n <= q; n++){
		grid[i][j] = n;
		configuration[n-1] = fabs(ENERGY_CALC() - target_e);
	}
}


double POTTS_MODEL::JACK_KNIFE(double *observed, double avg){
	unsigned int numbins = 10;
	unsigned int slice = nmeasurements / numbins;
	double sumbins = 0;
	double *bin;
	bin = new double[numbins];

	for(unsigned int l = 0; l < numbins; l++){
		bin[l] = 0.0;
		for(unsigned int k = 0; k < slice; k++){
			bin[l] += observed[l*slice + k];
		}
		bin[l] /= slice;
		sumbins += bin[l];
	}

	double *jackbins;
	jackbins = new double[numbins];
	// Form the jack knife bins
	for(unsigned int l = 0; l < numbins; l++){
		jackbins[l] = (sumbins - bin[l])/(numbins - 1);
	}

	// Compute the Error
	double error = 0;
	for(unsigned int l = 0; l < numbins; l++){
		error += (avg - jackbins[l]) * (avg - jackbins[l]);
	}
	error *= ((double)(numbins - 1))/(double)numbins;
	error = sqrt(error);
	return(error);

}


double POTTS_MODEL::SPIN_CHANGE_ENERGY_DIFF(unsigned int i, unsigned int j){
	unsigned int q_before = grid[i][j];
	double difftar1, difftar2;
	difftar1 = 0.0;
	difftar2 = 0.0;
	difftar1 = abs(ENERGY_CALC() - target_e);
	std::uniform_int_distribution<unsigned int> qdistribution(1,q);
	unsigned int rand_q = qdistribution(generator);
	grid[i][j] = rand_q;
	difftar2 = abs(ENERGY_CALC() - target_e);
	if(difftar1 < difftar2){
		grid[i][j] = q_before;
	} else {
		grid[i][j] = rand_q;
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

	delete[] values;

	delete[] energy;
	delete[] magnetisation;
	delete[] specificheat;
	delete[] susceptibility;
	delete[] estar;
	delete[] arrayofan;
}

int POTTS_MODEL::OUTSIDE_ENERGY_BAND(){
	double target_lb, target_ub;
	target_lb = target_e - (target_width/2);
	target_ub = target_e + (target_width/2);
	double energy = ENERGY_CALC();

	if( energy < target_ub && energy > target_lb){
		return(0);
	}
	return(1);
}
