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
To enable the make target for building a documentation using doxygen, run cmake with the option
```
-DDOC_TARGET=ON
```
If doxygen is installed and the option is enabled, you can then build the doxygen documentation with
```
make doc_doxygen
```
The doxygen documentation will be generated in the directory `doxys_documentation`


### How to run

To run the program, execute the following command
```
./MolSim ../input/eingabe-sonne.txt  
```