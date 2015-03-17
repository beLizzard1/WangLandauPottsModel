#ifndef POTTS_H
#define POTTS_H

#include <random>

enum UPDATE_ALG { METROPOLIS, WANGLANDAU };

class POTTS_MODEL{
	public:
		POTTS_MODEL();
		~POTTS_MODEL();
		bool wanglandau;
		unsigned int size;
<<<<<<< HEAD
		unsigned int n_q;

		// Metropolis Algorithm Parameters
=======
		unsigned int seed;
		std::default_random_engine generator;
		unsigned int **grid;
		unsigned int numbins = 100;

		double *energy;
		double energy_avg;
		double energy_err;

		double *magnetisation;
		double magnetisation_avg;
		double magnetisation_err;

		unsigned int nmeasurements;
		void DO_MEASUREMENTS(unsigned int);
		void DO_UPDATE(UPDATE_ALG);
		void ERROR_CALC();
	private:
		unsigned int q;
		unsigned int o_nearestneighbour;
>>>>>>> 4a8bfaa...  Changes to be committed:
		double beta;

		// Wang Landau Algorithm Parameters
		double a0;

<<<<<<< HEAD
	private:

};

=======
>>>>>>> 4a8bfaa...  Changes to be committed:

#endif
