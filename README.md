MolSim - Group F
===

### How to build

To build the project, run the following commands
```
mkdir build
cd build
cmake ..
make
```

### Documentation
To enable the make target for building a documentation with doxygen, run cmake with the option (this option is turned `OFF` by default)
```
-DDOC_TARGET=ON
```
If doxygen is installed and the option is enabled, you can then build the doxygen documentation with
```
make doc_doxygen
```
The doxygen documentation will be generated in the directory `doxys_documentation`


### Execution

To run the program, execute the following command
```
./src/molsim <input_file> [OPTIONS]
```
For the input file use for example `../input/eingabe-sonne.txt`.

The executable offers the following options:
- `-d, --delta_t`: The size of each timestep. Default value: 0.014
- `-e, --t_end`: Time at which to stop the simulation. Default value: 1000 
- `-l, --log`: Log level. Valid values (high to low):
  'trace', 'debug', 'info', 'warn', 'err', 'critical', 'off'
  (using any other string will result in logging turned 'off'). Default value: 'info'
- `-s, --sim_type`: Type of simulation to run (default: 1):
  - 0 - Planet Simulation (simulation of some planets from our solar system and Halley's Comet which also spins around the sun with a high revolution period)
  - 1 - Collision of Cuboids (simulation of 2 (or more) 2-dimensional cuboids consisting of molecules colliding)
- `-b, --bench`: Activates benchmarking: The simulation will be run 10 times without producing any file output or logging. The execution times for all simulations will then be averaged
- `-E, --epsilon`: Epsilon parameter for Lennard-Jones potential
- `-S, --sigma`: Sigma parameter for Lennard-Jones potential


### Unit Tests

To run the unit test, execute the following commands (assuming current directory is `build/`):

```
cd test/ && ctest
```
