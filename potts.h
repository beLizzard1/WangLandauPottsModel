#ifndef POTTS_H
#define POTTS_H

#include <random>

enum UPDATE_ALG { METROPOLIS, WANGLANDAU };

class POTTS_MODEL{
	public:
		POTTS_MODEL(); // Constructor
		~POTTS_MODEL(); // Destructor
		bool wanglandau, coldstart;
		bool interface = false; // Type of Simulation to run
		unsigned int size; // Grid Length
		unsigned int n_q; // Number of Possible q states

		unsigned int interfacepoint;
		unsigned int k;

		// Metropolis Algorithm Parameters
		double beta;
		bool randomspin;
		unsigned int n_samples;

		// Wang Landau Algorithm Parameters
		double a0, target_e, target_width;
		unsigned int n_entropic_samples;
		unsigned int n_asamples;

		// Angles
		double *angles;
		// Initialisation functions for grid
		void metropolis();
		void wang_landau();

	private:
		unsigned int **grid;
		unsigned int volume;
		unsigned int n_therm = 100000;
		unsigned int acceptance = 0;

		// Energy and Magnetisation Calculation Functions
		double energycalc();
		double magnetisationcalc();
		double energychange(unsigned int, unsigned int);

		// Update Algorithms
		void metropolis_update();
		void smooth_metropolis_update(unsigned int, unsigned int);
		int outsideenergyband();
		void wanglandau_update();
		void drivetotarget(unsigned int, unsigned int);
		// Measurement for Update
		void metropolis_measurement(unsigned int);
		void wanglandau_measurement(unsigned int);

		std::mt19937_64 generator;

		// Measurement arrays
		double *energy;
		double *magnetisation;
		double *estar;
		double *aguess;
		double cur_a;

		// Analysis
		double metropolis_average(double *);
		double metropolis_error(double *, double);
		double wanglandau_average(double *);
		double wanglandau_error(double *, double);


};

#endif
