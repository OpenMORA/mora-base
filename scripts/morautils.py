# Utility functions for OpenMORA scripts
#
# Part of OpenMora - https://github.com/OpenMORA

import os
import sys
import string
import platform

def get_mora_paths():
	""" Returns a list of paths with MORA modules, from the env var MORA_PATH
	"""
	if not 'MORA_PATH' in os.environ:
		print('**ERROR** Environment variable MORA_PATH not set')
		sys.exit(1)

	sMoraPaths=os.environ['MORA_PATH'];
	if platform.system()=="Windows":
		sPathDelim = ";"
	else:
		sPathDelim = ":"
	
	morabase_dir="";
	return sMoraPaths.split(sPathDelim)

def get_morabase_dir():
	""" Returns the path of "mora-base" pkg
	"""
	mora_paths = get_mora_paths()  # Get env vars
	
	for p in mora_paths:
		tstPath = os.path.normpath(p + "/mora-base")
		if os.path.exists(tstPath):
			morabase_dir = tstPath

	if (len(morabase_dir)==0) or (not os.path.exists(morabase_dir)):
		print("Couldn't detect mora-base in MORA_PATH!!")
		sys.exit(1)
	return morabase_dir
	

