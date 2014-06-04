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
import glob
import morautils

#------   MAIN   -------
def main():
	lstToBuild = []
	# No arguments: Build current dir:
	if (len(sys.argv)==1):
		lstToBuild.append(os.path.abspath(os.getcwd()))
	else:
		# List of directories:
		for p in sys.argv[1:]:
			# Support expanding wildcard expressions (for Windows):
			for fil in glob.glob(p):
				lstToBuild.append(os.path.abspath(fil))

	# Locate the mora-base pkg, which contains the core libs:
	morabase_dir = morautils.get_morabase_dir()
	morabase_build_dir = os.path.normpath(morabase_dir + "/build")

	# Loop for each pkg to be built:
	for p in lstToBuild:
		pkg_dir = os.path.normpath(p)
		pkg_name = os.path.basename(pkg_dir) 
		print("[Building] %s @ %s" % (pkg_name, pkg_dir) )
		assert os.path.exists(pkg_dir)  # Make sure the dir exists
		if not os.path.exists(os.path.normpath(pkg_dir+'/CMakeLists.txt')):
			print("Directory '%s' doesn't contain a CMakeLists.txt file! Is it a MORA pkg?" % pkg_dir)
			sys.exit(1)
		# Build dir for CMake:
		build_dir = os.path.normpath( pkg_dir + "/build" )
		if not os.path.exists(build_dir):
			print("  * Mkdir: %s" % build_dir )
			os.mkdir(build_dir)
		else:
			print("  * Existed: %s" % build_dir )
			
		# chdir & run cmake:
		os.chdir(build_dir)
		cmake_args = ["cmake", pkg_dir]
		if ('MORA_CMAKE_GENERATOR' in os.environ):
			sMoraGenerator = os.environ['MORA_CMAKE_GENERATOR']
			cmake_args.append("-G")
			cmake_args.append(sMoraGenerator)
		
		# Help CMake to locate MORA:
		cmake_args.append("-DMORA_DIR=" + morabase_build_dir)

		print("  * Invoking cmake: %s" % ' '.join(map(str, cmake_args)))
		call(cmake_args)
		
		# Compile:
		print("  * Compiling:")
		# Call nmake, msbuild or whatever via CMake:
		make_args=["cmake","--build",build_dir,"--config","Release"];
		call(make_args)


if __name__ == "__main__":
	sys.exit(main())

