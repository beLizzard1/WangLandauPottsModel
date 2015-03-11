#ifndef POTTS_H
#define POTTS_H

#include <random>
#include <mgl2/mgl.h>

enum UPDATE_ALG { METROPOLIS, WANGLANDAU };

class POTTS_MODEL{
	public:
		POTTS_MODEL(unsigned int, unsigned int, unsigned int,double,unsigned int,double);
		~POTTS_MODEL();
		void SCRAMBLE_GRID();
		void DRAW();
		void FORCE_ALIGN_GRID();
		void SET_TARGET(double, double);
		double ENERGY_CALC();
		double SPIN_CHANGE_ENERGY_DIFF(unsigned int, unsigned int);
		int OUTSIDE_ENERGY_BAND();
		unsigned int size;
		unsigned int seed;
		std::default_random_engine generator;
		unsigned int **grid;
		unsigned int numbins = 10;
		
		double *values; 

		double *energy;
		double energy_avg;
		double energy_err;

		double *magnetisation;
		double magnetisation_avg;
		double magnetisation_err;

		double *susceptibility;
		double susceptibility_avg;
		double susceptibility_err;

		double *specificheat;
		double specificheat_avg;
		double specificheat_err;

		double *estar;
		double estar_avg;
		double estart_err;


		double aguess;
		double *arrayofan;

		unsigned int acceptance;
		unsigned int nmeasurements;
		void DO_MEASUREMENTS(unsigned int, UPDATE_ALG);
		void DO_UPDATE(UPDATE_ALG);
		void ERROR_CALC();
		void write_metropolis_output();
		double JACK_KNIFE(double*, double);
		double target_e;
		double target_width;
	private:
		unsigned int q;
		unsigned int o_nearestneighbour;
		double beta;
		double coupling;
		int NEAREST_NEIGHBOUR(unsigned int, unsigned int);
		class REGION *regions;

};


#endif
