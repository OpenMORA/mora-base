/* +---------------------------------------------------------------------------+
   |                 Open MORA (MObile Robot Arquitecture)                     |
   |                    https://github.com/OpenMORA/                           |
   |                                                                           |
   |   Copyright (C) 2014  University of Malaga, University of Almeria         |
   +---------------------------------------------------------------------------+ */

#pragma once

/* -----------------------------------------------------------------------
Template for main.cpp files in OpenMORA modules

Usage: 

	[main.cpp]
	#include "CMyApp.h" // User C++ app class 
	
	#define MORA_APP_CLASS         CMyApp     
	#define MORA_APP_NAME          "MyApp"    // Default MOOSApp app name
	
	#include <mora_main.h>

-----------------------------------------------------------------------*/


// Note: One can pass the name of the user class #include to this
// file via a macro and generate a dynamic #include, but that would
// require additional -I flags, so I discarded the idea.


#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Utils/CommandLineParser.h"

int main(int argc ,char * argv[])
{
	//here we do some command line parsing...
	MOOS::CommandLineParser P(argc,argv);
	//mission file could be first free parameter
	std::string mission_file = P.GetFreeParameter(0, "Mission.moos");
	//mission name can be the  second free parameter
	std::string app_name = P.GetFreeParameter(1, MORA_APP_NAME);

	//GO!
	MORA_APP_CLASS app;
	app.Run(app_name,mission_file,argc,argv);

	return 0;
}




