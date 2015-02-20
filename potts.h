#ifndef POTTS_H
#define POTTS_H

#include <random>
#include <mgl2/mgl.h>

class POTTS_MODEL{
	public:
		POTTS_MODEL(unsigned int, unsigned int, unsigned int,double,unsigned int);
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
		double *energy;
		double *magnetisation;
		unsigned int nmeasurements;
		void DO_MEASUREMENTS(unsigned int);
		void DO_UPDATE();
	private:
		unsigned int q;
		unsigned int o_nearestneighbour;
		double beta;
		double coupling;
		double target_e;
		double target_width;
		int NEAREST_NEIGHBOUR(unsigned int, unsigned int);
		class REGION *regions;

};

class REGION{
        friend class POTTS_MODEL;
        public:
        int state;
        REGION(class POTTS_MODEL *, unsigned int, unsigned int);
        ~REGION();
        unsigned int regionsize;
        unsigned int **point;
        private:

};


#endif
