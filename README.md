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
If you want to use the Intel® oneAPI DPC++/C++ (icpx) compiler change the cmake command to
```
cmake .. -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
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
The doxygen documentation will be generated in the directory `doxys_documentation` (which is inside the project directory)


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
  - 2 - Collision of cuboids using the (performance-enhancing) linked-cell algorithm
- `-b, --bench`: Activates benchmarking: The simulation will be run 10 times without producing any file output or logging. The execution times for all simulations will then be averaged
- `-E, --epsilon`: Epsilon parameter for Lennard-Jones potential
- `-S, --sigma`: Sigma parameter for Lennard-Jones potential



### Unit Tests

To run the unit test, execute the following commands (assuming current directory is `build/`):

```
cd test/ && ctest
```


### Checkpoint File

To create a checkpoint file after running a simulation, add the following element to the xml-file:

```
<output>
      <create_checkpoint_file>true</create_checkpoint_file>
</output>
```

To add a checkpoint file to a new simulation, please include the filepath of the file to the parameters in the xml-file, z.B.:

```
<import_checkpoint>
      <file_path>../input/checker.txt</file_path>
</import_checkpoint>
```

### Velocity-density profile
To output a velocity - density profile, please include the following elements to the parameters of the xml-input-file:

```
<bin_profile>
      <iteration>integer value</iteration>
      <bin_number>integer value</bin_number>
      <base_name>string</base_name>
</bin_profile>
```
While the number of iteration after which a profile should be created must be given, the base name and the number of bins (default: 50) are optional.
The format in the csv-file is: *number_of_particles, density, x-velocity, y-velocity, z-velocity*

### Parallelization

To parallelize the computations, please include the following elements into the xml-file

```
<simulation>
...
      <parallelization>
            <strategy>integer value</strategy>
            <threadNumber>integer value</threadNumber>
      </parallelization>
</simulation>
```
Note: the position of the element is at the bottom of <simulation></simulation>
strategies: 
- `0`: Parallelization of iteration over cells 
- `1`: Parallelization of iteration over particles

threadNumber (optional):
sets number of thread used for strategy 0 (default: 4). Strategy 1 uses always the maximum.



                            