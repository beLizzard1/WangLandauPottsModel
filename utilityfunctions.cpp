#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string>

#include <libconfig.h++>

#include "potts.h"
#include "utilityfunctions.h"

using namespace libconfig;

int read_input_libconf(std::string file, POTTS_MODEL *potts){
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

	// Type of Simualation to Run. Either WangLandau (entropic sampling) or Metropolis
	try{
		potts->wanglandau = cfg.lookup("wanglandau");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'wanglandau' parameter found" << std::endl;
		std::cerr << "Unrecoverable Error. Add a wanglandau to the " << file << std::endl;
		return(1);
	}

	// Collect type of simulation invarient parameters here
	try{
		potts->size = cfg.lookup("dim_grid");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'dim_grid' parameter found" << std::endl;
		std::cerr << "Unrecoverable Error. Add a dim_grid to the " << file << std::endl;
		return(1);
	}

	try{
		potts->n_q = cfg.lookup("dim_q");
	}
	catch(const SettingNotFoundException &nfex){
		std::cerr << "No 'dim_q' parameter found" << std::endl;
		std::cerr << "Unrecoverable Error. Add a dim_q to the " << file << std::endl;
		return(1);
	}

	if(potts->wanglandau == true){
		// Collect specifics for that
	} else {
		// Collect specifics for Metropolis algorithm beta etc.
		try{
			potts->beta = cfg.lookup("beta");
		}
		catch(const SettingNotFoundException &nfex){
			std::cerr << "No 'beta' parameter found" << std::endl;
			std::cerr << "Unrecoverable Error. Add a beta to the " << file << " or change wanglandau" << std::endl;
			return(1);
		}
	}

	return(0);
}
