mora-base
=========
OpenMORA core tools and common libraries: core-moos, essential-moos and scripts

## Prerequisites 
  * A C++ compiler
  * [CMake](http://www.cmake.org/)
  * [mrpt](http://www.mrpt.org/)
  * Python CLI ([Windows download](https://www.python.org/download/windows)) with the modules:
    * YAML ([Windows download](http://pyyaml.org/wiki/PyYAML))

In Ubuntu, run: 

    sudo apt-get install build-essential cmake python python-yaml
    
In Windows: 
  * Remember to add Git, CMake and Python to the system PATH env var.

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
In Linux, add this line to ~./bashrc

    source [path to mora-base]/scripts/mora-setup.sh
    
In Windows, set these environment vars:

  * `MORA_PATH`: List of directories containing OpenMORA pkgs. Separator is ";" in Windows, ":" in unices. Must point to the *parent* directory of MORA pkgs, such that  `$MORA_PATH/mora-base/` exists.
  * `MORA_CMAKE_GENERATOR`: Must be set in Windows to the desired CMake generator, e.g. "Visual Studio 10 Win64"
  * Manually add `[MORA-BASE_PATH]/scripts/` to the `PATH` env var.
  * `MORA_EXECUTABLE_OUTPUT`: (Optional) If defined, will override CMake's default `EXECUTABLE_OUTPUT_PATH`. Can be used to put all executables into one single directory.
    

## Download & build MORA pkgs
Note that the following commands work on both *Windows* and *Linux*.

To download all publicly listed pkgs, simply run: 

    mora-pull --all

Build by calling CMake for each directory, or do it automatically with:

    mora-build            # Builds pkg at current dir
    mora-build ual*-pkg   # Builds specific pkgs
    
Move into the directory of some module with `mora-cd`

    mora-cd mora-base 

