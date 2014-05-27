#!/usr/bin/env python

# Download all MORA modules and/or pull the most recent changes from 
# their repositories. Default workspace is the parent dir of mora-base path.
#  Usage:
#   mora-pull.py --all  # Use on bootstrap: download all pkgs
#   mora-pull.py        # Synch (update/pull) changes of all pkgs   
#
# Part of OpenMora - https://github.com/OpenMORA

import os
import sys
import string
from subprocess import call
import morautils
import argparse
import yaml
import git 

# Aux class
class GitProgress(git.RemoteProgress):
	op_code = None

	def update(self, op_code, cur_count, max_count=None, message=''):
		if isinstance(max_count, str) and max_count:
			morautils.progress(float(cur_count)/float(max_count)*100)


#------   MAIN   -------
def main():
	parser = argparse.ArgumentParser(description='Download OpenMORA pkgs.')
	parser.add_argument("--all", 
		help="synch all existing pkgs, not only those found in disk",
		action='store_true')
	args = parser.parse_args()
	if args.all:
		print("Downloading *all* OpenMORA pkgs.")

	# Locate the mora-base pkg, which contains the distro file:
	morabase_dir = morautils.get_morabase_dir()
	pkgs_root = os.path.dirname(morabase_dir)
	sDistroFile = os.path.normpath( morabase_dir + "/distro/openmora-pkgs.yaml")
	assert os.path.exists(sDistroFile)

	print("Using MORA root dir: " + pkgs_root)
	assert os.path.exists(pkgs_root + "/mora-base")

	# Parse distro file:
	fil = file(sDistroFile, 'r') 
	distro = yaml.load(fil)
	#print distro

	# Go thru all pkgs:
	for pkg in distro:
		print("Processing pkg: " + pkg)
		sPkgDir = os.path.normpath( pkgs_root+"/"+pkg )
		if not os.path.exists(sPkgDir):
			# Non existing path ----------------------
			if not args.all:
				print("  * Pkg not found, skipping (rerun with --all to download it)")
			else:
				print("  * Pkg not found, will clone it now (-all flag)")
				print("  * Mkdir: %s" % sPkgDir )
				os.mkdir(sPkgDir)
				# Git clone:
				git_url = distro[pkg]["git-url"]
				print("  * Cloning from: " + git_url )
				# Do a git clone:
				git.Repo.clone_from(git_url, sPkgDir)
		else:
			# Already existing path ----------------------
			print("  * Pulling updates...")
			repo = git.Repo(sPkgDir)
			o = repo.remotes.origin
			o.pull( progress=GitProgress() )


if __name__ == "__main__":
	sys.exit(main())

