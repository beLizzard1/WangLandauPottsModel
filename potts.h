#ifndef POTTS_H
#define POTTS_H

#include <random>

enum UPDATE_ALG { METROPOLIS, WANGLANDAU };

class POTTS_MODEL{
	public:
		POTTS_MODEL(); // Constructor
		~POTTS_MODEL(); // Destructor
		bool wanglandau, coldstart; // Type of Simulation to run
		unsigned int size; // Grid Length
		unsigned int n_q; // Number of Possible q states

		// Metropolis Algorithm Parameters
		double beta;
		unsigned int n_samples;

		// Wang Landau Algorithm Parameters
		double a0, target_e, target_width;
		unsigned int n_entropic_samples;

		// Angles
		double *angles;
		// Initialisation functions for grid
		void metropolis();

	private:
		unsigned int **grid;
		unsigned int volume;
		unsigned int n_therm = 10000;
		unsigned int acceptance = 0;

		// Energy and Magnetisation Calculation Functions
		double energycalc();
		double magnetisationcalc();

		// Update Algorithms
		void metropolis_update();

		// Measurement for Update
		void metropolis_measurement(unsigned int);

		// Measurement arrays
		double *energy;
		double *magnetisation;

		// Analysis
		double metropolis_average(double *);
		double metropolis_error(double *, double);
		

};

#endif
