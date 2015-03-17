# Wang Landau Potts Model Solver

###Requirements
* `libconfig`
* C++ Compiler supporting `-std=gnu+11`
* `<random>`

### To Do
* Improve the energy calculation function.
* Read the configuration file correctly in a way that doesn't require all the arguments to be provided (defaults).
* Add live mathgl rendering of the lattice.
* Identify regions of unique spin on the mathgl render. (use those regions to optimise the energy)
* Find a way to move across the energy landscape until you reach the desired energy band
