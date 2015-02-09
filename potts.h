#ifndef POTTS_H
#define POTTS_H

#include <mgl2/qt.h>

class POTTS_MODEL : public mglDraw{

	public:
		POTTS_MODEL(unsigned int, unsigned int, unsigned int,double);
		~POTTS_MODEL();
		void SCRAMBLE_GRID();
		int Draw(mglGraph *gr);
		void FORCE_ALIGN_GRID();
		void SET_TARGET(double, double);
		double ENERGY_CALC();
		double SPIN_CHANGE_ENERGY_DIFF(unsigned int, unsigned int);
		int OUTSIDE_ENERGY_BAND();
		unsigned int size;
		unsigned int seed;
	private:
		unsigned int q;
		unsigned int o_nearestneighbour;
		unsigned int **grid;
		double beta;
		double coupling;
		double target_e;
		double target_width;
		int NEAREST_NEIGHBOUR(unsigned int, unsigned int);
};

#endif
