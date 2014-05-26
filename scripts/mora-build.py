#!/usr/bin/env python

# Builds the MORA module at CWD, or at $1. This implies:
# - Creating $DIR/build
# - Running cmake 
# - Running make or msbuild
#
# Part of OpenMora - https://github.com/OpenMORA

import os
import sys
import re
import string
import platform
from subprocess import call

#------   MAIN   -------
def main():
	lstToBuild = []
	if (len(sys.argv)==1):
		lstToBuild.append(os.path.abspath(os.getcwd()))
	else:
		for p in sys.argv[1:]:
			lstToBuild.append(os.path.abspath(p))

	# Locate the mora-base pkg, which contains the core libs:
	if not 'MORA_PATH' in os.environ:
		print('**ERROR** Environment variable MORA_PATH not set')
		sys.exit(1)

	mora_paths= os.environ['MORA_PATH'];
	if platform.system()=="Windows":
		sPathDelim = ";"
	else:
		sPathDelim = ":"
	
	morabase_dir="";
	for p in mora_paths.split(sPathDelim):
		tstPath = os.path.normpath(p + "/mora-base")
		if os.path.exists(tstPath):
			morabase_dir = tstPath

	if (len(morabase_dir)==0) or (not os.path.exists(morabase_dir)):
		print("Couldn't detect mora-base in MORA_PATH!!")
		sys.exit(1)
	morabase_build_dir = os.path.normpath(morabase_dir + "/build")
	

	# Loop for each pkg to be built:
	for p in lstToBuild:
		pkg_dir = os.path.normpath(p)
		pkg_name = os.path.basename(pkg_dir) 
		print("[Building] %s @ %s" % (pkg_name, pkg_dir) )
		assert os.path.exists(pkg_dir)  # Make sure the dir exists
		assert os.path.exists(os.path.normpath(pkg_dir+'/CMakeLists.txt'))
		# Build dir for CMake:
		build_dir = os.path.normpath( pkg_dir + "/build" )
		if not os.path.exists(build_dir):
			print("    Mkdir: %s" % build_dir )
			os.mkdir(build_dir)
		else:
			print("    Existed: %s" % build_dir )
			
		# chdir & run cmake:
		os.chdir(build_dir)
		cmake_args = ["cmake", pkg_dir]
		if ('MORA_CMAKE_GENERATOR' in os.environ):
			sMoraGenerator = os.environ['MORA_CMAKE_GENERATOR']
			cmake_args.append("-G")
			cmake_args.append(sMoraGenerator)
		
		# Help CMake to locate MORA:
		cmake_args.append("-DMORA_DIR=" + morabase_build_dir)

		print("    Invoking cmake with args:\n    => %s" % '\n    => '.join(map(str, cmake_args)))
		call(cmake_args)
		
		# Compile:
		print("    Compiling:")
		# TODO: call nmake or msbuild in Windows:
		make_args=["make"];
		call(make_args)


if __name__ == "__main__":
	sys.exit(main())

