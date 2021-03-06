# Utility functions for OpenMORA scripts
#
# Part of OpenMora - https://github.com/OpenMORA

import os, sys, string
import platform
import yaml

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
	
import sys, math

def progress(percent):
	''' source: http://gunslingerc0de.wordpress.com/2010/08/13/python-command-line-progress-bar/ '''
	width = 74
	marks = math.floor(width * (percent / 100.0))
	spaces = math.floor(width - marks)

	loader = '[' + ('=' * int(marks)) + (' ' * int(spaces)) + ']'

	if percent >= 100:
		percent = 100
	sys.stdout.write("%s %d%%\r" % (loader, percent))
	if percent >= 100:
		pass
		sys.stdout.write("\n")
	sys.stdout.flush()


def get_pkgs_root():
	'''Returns the path to the parent directory of mora-base'''
	morabase_dir = get_morabase_dir()
	pkgs_root = os.path.dirname(morabase_dir)
	return pkgs_root
	
	
def read_distro_file():
	'''Returns the yaml contents of the distro file'''
	
	morabase_dir = get_morabase_dir()
	pkgs_root = os.path.dirname(morabase_dir)
	
	sDistroFile = os.path.normpath( morabase_dir + "/distro/openmora-pkgs.yaml")
	assert os.path.exists(sDistroFile)

	assert os.path.exists(pkgs_root + "/mora-base")

	# Parse distro file:
	fil = open(sDistroFile, 'r') 
	distro = yaml.load(fil)
	fil.close()
	#print distro
	return distro

