mora-base
=========
OpenMORA core tools and common libraries: core-moos, essential-moos and scripts

## Prerequisites 
  * A C++ compiler
  * [CMake](http://www.cmake.org/)
  * [mrpt](http://www.mrpt.org/)
  * Python CLI ([Windows download](https://www.python.org/download/windows))

In Ubuntu, run: 

    sudo apt-get install build-essential cmake python

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

    source [path to mora-base]/scipts/mora-setup.sh

## Building MORA pkgs
TODO: Create list of pkgs? Automated script to download them? 

Clone other MORA pkgs. 
Build them with:

    mora-build.py
    
## Environment variables
The following vars can be defined to affect the configuration of all MORA packages (they are set automatically by `mora-setup.sh` in Linux):

  * `MORA_PATH`: List of directories containing OpenMORA pkgs. Separator is ";" in Windows, ":" in unices. Must point to the *parent* directory of MORA pkgs, such that  `$MORA_PATH/mora-base/` exists.
  * `MORA_EXECUTABLE_OUTPUT`: If defined, will override CMake's default EXECUTABLE_OUTPUT_PATH. Can be used to put all executables into one single directory.
