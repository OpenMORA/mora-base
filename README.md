mora-base
=========
OpenMORA core tools and common libraries: core-moos, essential-moos and scripts

## Prerequisites 
  * A C++ compiler
  * [CMake](http://www.cmake.org/)
  * [mrpt](http://www.mrpt.org/)
  * Python CLI ([Windows download](https://www.python.org/download/windows)) with the modules:
    * YAML

In Ubuntu, run: 

    sudo apt-get install build-essential cmake python python-yaml

## Download and compile
Create an empty directory for OpenMORA packages:

    mkdir openmora && cd openmora

Get a copy of the `mora-base` pkg and init the external submodules:

    git clone https://github.com/OpenMORA/mora-base.git
    cd mora-base
    git submodule init && git submodule update

Compile with CMake:

    mkdir build && cd build 
    cmake ..
    make

## Setting up environment
In Linux, add this line to ~./basrc

    source [path to mora-base]/scripts/mora-setup.sh

## Download MORA pkgs
To download all publicly listed pkgs, simply run: 

    mora-pull.py --all

## Building pkgs

Clone other MORA pkgs. 
Build them with:

    mora-build.py               # Builds pkg at current dir
    mora-build.py sensors-pkg   # Builds specific pkg
    mora-build.py *-pkg
    
## Environment variables
The following vars can be defined to affect the configuration of all MORA packages (they are set automatically by `mora-setup.sh` in Linux):

  * `MORA_PATH`: List of directories containing OpenMORA pkgs. Separator is ";" in Windows, ":" in unices. Must point to the *parent* directory of MORA pkgs, such that  `$MORA_PATH/mora-base/` exists.
  * `MORA_EXECUTABLE_OUTPUT`: If defined, will override CMake's default `EXECUTABLE_OUTPUT_PATH`. Can be used to put all executables into one single directory.
  * `MORA_CMAKE_GENERATOR`: Must be set in Windows to the desired CMake generator, e.g. "Visual Studio 10"

