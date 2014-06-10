#!/usr/bin/env python

# The core behinds "mora-cd": prints to stdout the resulting full path of a "mora-cd" command.
#
# Part of OpenMora - https://github.com/OpenMORA

from __future__ import print_function
import os
import sys
import string
import morautils

#------   MAIN   -------
def main():
	if (len(sys.argv)==1):
		sCDMod = ''
	else:
		sCDMod = sys.argv[1]

	# Locate the mora-base pkg, which contains the distro file:
	pkgs_root = morautils.get_pkgs_root()
	#print("Using MORA root dir: " + pkgs_root)

	sPkgDir = os.path.normpath( pkgs_root+"/"+sCDMod )
	if not os.path.exists(sPkgDir):
		# Non existing path ----------------------
		print('ERROR: Path does not exist: "%s"' % sPkgDir,file=sys.stderr)
		return 1
	else:
		sys.stdout.write(sPkgDir)
		return 0
		
if __name__ == "__main__":
	sys.exit(main())
