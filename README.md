mora-base
=========

OpenMORA core tools and common libraries: core-moos, essential-moos and scripts


## Download and compile

Get a copy and init the external submodules:

    git clone https://github.com/OpenMORA/mora-base.git
    cd mora-base
    git submodule init && git submodule update

Compile with CMake:

    mkdir build && cd build 
    cmake ..
    make


## Environment variables

The following vars can be defined to affect the configuration of all MORA packages:

  * MORA_EXECUTABLE_OUTPUT: If defined, will override CMake's default EXECUTABLE_OUTPUT_PATH. 


