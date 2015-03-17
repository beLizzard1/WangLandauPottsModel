#ifndef POTTS_H
#define POTTS_H

#include <random>

class POTTS_MODEL{
	public:
		POTTS_MODEL();
		~POTTS_MODEL();
		bool wanglandau;
		unsigned int size;
		unsigned int n_q;

		// Metropolis Algorithm Parameters
		double beta;

		// Wang Landau Algorithm Parameters
		double a0;

	private:

};


#endif
