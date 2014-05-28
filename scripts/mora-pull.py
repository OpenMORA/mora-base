#!/usr/bin/env python

# Download all MORA modules and/or pull the most recent changes from 
# their repositories. Default workspace is the parent dir of mora-base path.
#  Usage:
#   mora-pull.py --all  # Use on bootstrap: download all known pkgs (see distro file)
#   mora-pull.py        # Synch (update/pull) changes of all known pkgs (see distro file)
#
# Part of OpenMora - https://github.com/OpenMORA

import os
import sys
import string
import subprocess
import morautils
import argparse
import yaml

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
	fil = open(sDistroFile, 'r') 
	distro = yaml.load(fil)
	#print distro

	# Go thru all pkgs:
	for pkg in distro:
		print("# Processing pkg: " + pkg)
		sPkgDir = os.path.normpath( pkgs_root+"/"+pkg )
		if not os.path.exists(sPkgDir):
			# Non existing path ----------------------
			if not args.all:
				print("  * Pkg not found, skipping (rerun with --all to download it)")
			else:
				print("  * Pkg not found, will clone it now (-all flag)")
				# Git clone:
				git_url = distro[pkg]["git-url"]
				print("  * Cloning from: " + git_url )
				git_args = ["git", "clone",git_url,sPkgDir]
				print("  * Invoking: %s" % ' '.join(map(str, git_args)))
				if 0!=subprocess.call(git_args):
					print("** ERROR** Last command failed!!")
		else:
			# Already existing path ----------------------
			print("  * Pulling updates...")
			# Git pull:
			os.chdir(sPkgDir)
			git_args = ["git", "pull"]
			print("  * Invoking: %s" % ' '.join(map(str, git_args)))
			if 0!=subprocess.call(git_args):
				print("** ERROR** Last command failed!!")

		# Process optional flag "git-version"
		if 'git-version' in distro[pkg]:
			os.chdir(sPkgDir)
			sGitBranch = distro[pkg]['git-version']
			print("  * Checking out '%s'" % sGitBranch)
			git_args = ["git", "checkout",sGitBranch]
			print("  * Invoking: %s" % ' '.join(map(str, git_args)))
			if 0!=subprocess.call(git_args):
				print("** ERROR** Last command failed!!")
			

if __name__ == "__main__":
	sys.exit(main())

