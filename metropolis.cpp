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
#include "potts.h"

void POTTS_MODEL::metropolis(){
    grid = new unsigned int *[size]; // 2D array for ease of use
    for(unsigned int i = 0; i < size; i++){
        grid[i] = new unsigned int [size];
    }

    //Setup Arrays for Measurements
    energy = new double[n_samples];
    magnetisation = new double[n_samples];

    // Use a Mersenne Prime Twister Random Number Generator
    std::mt19937_64 generator;
    std::uniform_int_distribution<int> distribution(1,n_q);

    if(coldstart == true){
        // Set everything to a random q value
        unsigned int rand_q = distribution(generator);
        for(unsigned int j = 0; j < size; j++){
            for(unsigned int i = 0; i < size; i++){
                grid[i][j] = rand_q;
            }
        }
    } else {
        // Set every point randomly :)
        for(unsigned int j = 0; j < size; j++){
            for(unsigned int i = 0; i < size; i++){
                grid[i][j] = distribution(generator);
            }
        }
    }
    //std::cout << "Program Gets To JUST BEFORE THERMALISATION" << std::endl;
    // A Metropolis Algorithm needs Thermalising.
    acceptance = 0;
    for(unsigned int i = 0; i < n_therm; i++){
        metropolis_update();
    }
    //std::cout << "Program COMPLETES THERMALISATION" << std::endl;
    // Reset the acceptance
    acceptance = 0;
    for(unsigned int i = 0; i < n_samples; i++){
        metropolis_update();
        metropolis_measurement(i);
    }

    // Errors and Thermodynamic Derived Quantities
    double *specificheat = new double[n_samples];
    double *susceptibility = new double[n_samples];

    for(unsigned int i = 0; i < n_samples; i++){
        specificheat[i] = energy[i] * energy[i];
        susceptibility[i] = magnetisation[i] * magnetisation[i];
    }

    double energy_avg = metropolis_average(energy);
    double magnetisation_avg = metropolis_average(magnetisation);
    double specificheat_avg = metropolis_average(specificheat);
    double susceptibility_avg = metropolis_average(susceptibility);

    double energy_err = metropolis_error(energy,energy_avg);
    double magnetisation_err = metropolis_error(magnetisation,magnetisation_avg);
    double specificheat_err = metropolis_error(specificheat,specificheat_avg);
    double susceptibility_err = metropolis_error(susceptibility,susceptibility_avg);

    delete [] specificheat;
    delete [] susceptibility;

    specificheat_avg -= (energy_avg * energy_avg);
    specificheat_avg *= (beta * beta);

    susceptibility_avg -= (magnetisation_avg * magnetisation_avg);
    susceptibility_avg *= beta;

    specificheat_err = sqrt((specificheat_err * specificheat_err) + (energy_err * energy_err));
    susceptibility_err = sqrt((susceptibility_err * susceptibility_err) + (magnetisation_err * magnetisation_err));

    // Write the Data to File
    std::ofstream file;
	file.open("specificheat.dat");
	file << beta << " " << specificheat_avg << " " << specificheat_err << std::endl;
	file.close();

	file.open("susceptibility.dat");
	file << beta << " " << susceptibility_avg << " " << susceptibility_err << std::endl;
	file.close();

	file.open("energy.dat");
	file << beta << " " << energy_avg << " " << energy_err << std::endl;
	file.close();

	file.open("magnetisation.dat");
	file << beta << " " << magnetisation_avg << " " << magnetisation_err << std::endl;
	file.close();

    file.open("acceptance.dat");
    file << beta << " " << acceptance << " " << std::endl;
    file.close();

}

void POTTS_MODEL::metropolis_measurement(unsigned int k){
    energy[k] = energycalc(); //Total on Lattice
    magnetisation[k] = magnetisationcalc(); // Total on Lattice
    magnetisation[k] = abs(magnetisation[k]); // Absolute Value

    unsigned int volume = size * size;

    energy[k] /= volume; // Per Lattice Site
    magnetisation[k] /= volume; // Per Lattice Site

}


void POTTS_MODEL::metropolis_update(){
    std::mt19937_64 generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned int> distribution(1,n_q);
    std::uniform_int_distribution<unsigned int> coordinates(0,size-1);

    //std::cout << "Program Gets INSDIE metropolis_update()" << std::endl;

    unsigned int x = coordinates(generator);
    unsigned int y = coordinates(generator);
    double energy_pre = energycalc();
    unsigned int old_q = grid[x][y];

    unsigned int new_q = distribution(generator);
    grid[x][y] = new_q;
    double energy_post = energycalc();

    std::uniform_real_distribution<double> pdistribution(0,1);
    double delta = energy_post - energy_pre;
    double rand = pdistribution(generator);

    if( delta < 0.0 ){
        grid[x][y] = new_q;
        acceptance++;
    } else {
        if(exp(-1 * beta * delta) > rand){
            grid[x][y] = new_q;
            acceptance++;
        } else {
            grid[x][y] = old_q;
        }
    }
}

double POTTS_MODEL::metropolis_average(double *array){
    double average = 0.0;
    for(unsigned int i = 0; i < n_samples; i++){
        average += array[i];
    }
    average /= n_samples;
    return(average);
}

double POTTS_MODEL::metropolis_error(double *array, double average){
        double *bin, *jackbins;
        unsigned int numbins = 100;
        bin = new double[numbins];
        jackbins = new double[numbins];

        unsigned int slice = n_samples / numbins;
        double sumbins = 0.0;
        for(unsigned int l = 0; l < numbins; l++){
            bin[l] = 0.0;
            for(unsigned int k = 0; k < slice; k++){
                bin[l] += array[(l * slice)+k];
            }
            bin[l] /= slice;
            sumbins += bin[l];
        }

        // Forming Bins
        for(unsigned int l = 0; l < numbins; l++){
            jackbins[l] = (sumbins - bin[l]) / (numbins - 1.0);
        }

        double error = 0.0;
        for(unsigned int l = 0; l < numbins; l++){
            error += (average - jackbins[l]) * (average - jackbins[l]);
        }
        error *= (numbins - 1.0) / (double)numbins;
        error = sqrt(error);
        return(error);
}
