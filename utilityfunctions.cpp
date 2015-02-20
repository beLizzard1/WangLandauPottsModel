#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string>

#include <libconfig.h++>

#include "potts.h"
#include "utilityfunctions.h"

int read_input(std::string file,unsigned int *dim_q, unsigned int *o_nn, unsigned int *dim_grid,double *beta){
	std::fstream FILE;
	char line[256];
	FILE.open(file,std::ifstream::in);
	/* I'm lazy so I will run get line for each value that I know is in the cfg and expand later */

	FILE.getline(line,256);
	*dim_q = (unsigned int)std::strtoul(line,NULL,10);

	FILE.getline(line,256);
	*o_nn =  (unsigned int)std::strtoul(line,NULL,10);

	FILE.getline(line,256);
	*dim_grid = (unsigned int)std::strtoul(line,NULL,10);

	FILE.getline(line,256);
	*beta = std::strtod(line,NULL);

	FILE.close();
	return(0);
}

using namespace libconfig;

int read_input_libconf(std::string file,double *target_e, double *target_width, unsigned int *dim_q, unsigned int *o_nearestneighbour, unsigned int *dim_grid, double *beta, unsigned int *nmeasurements){

	Config cfg;

	try{
		cfg.readFile(file.c_str());
	} catch(const FileIOException &fioex){
		std::cerr << "I/O error while reading file." << std::endl;
		return(1);
	} catch(const ParseException &pex){
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return(1);
	}

	/* Now look for config parameters that need values */
	try{
		*target_e = cfg.lookup("target_e");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'target_e' setting in the configuration file." << std::endl;
		std::cerr << "Unrecoverable error. Please add this parameter to the config file" << std::endl;
		return(1);
	}

	try{
		*target_width = cfg.lookup("target_width");
	} 
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'target_width' setting in the configuration file." << std::endl;
		std::cerr << "Unrecoverable error. Please add this parameter to the config file" << std::endl;
		return(1);
	}

	try{
		*nmeasurements = cfg.lookup("n_measurements");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'n_measurements' setting in the configuration file." << std::endl;
		std::cerr << "Unrecoverable error. Please add this parameter to the config file" << std::endl;
		return(1);
	}


	/* All parameters after this one can use defaults */

	try{
		*dim_q = cfg.lookup("dim_q");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'dim_q' setting in the configuration file." << std::endl;
		std::cerr << "Using default (Ising)" << std::endl;
		*dim_q = 2;
	}

	try{
		*o_nearestneighbour = cfg.lookup("o_nearestneighbour");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'o_nearestneighbour' setting in the configuration file." << std::endl;
		std::cerr << "Using default" << std::endl;
		*o_nearestneighbour = 1;
	}

	try{
		*dim_grid = cfg.lookup("dim_grid");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'dim_grid' setting in the configuration file." << std::endl;
		std::cerr << "Using default" << std::endl;
		*dim_grid = 10;
	}

	try{
		*beta = cfg.lookup("beta");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'beta' setting in the configuration file." << std::endl;
		std::cerr << "Using default" << std::endl;
		*beta = 0.5;
	}

	return(0);

}

