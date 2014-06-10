#!/bin/bash

# cd (change dir) into the given module. Default workspace is the parent dir of mora-base path.
#  Usage:
#   mora-cd                 # goes to default modules workspace directory
#   mora-cd [MODULE_NAME]   # goes to module dir
#
# Part of OpenMora - https://github.com/OpenMORA

# Get script current directory:
MORA_SCRIPTS_DIR="$( dirname "$(readlink -e "${BASH_SOURCE[0]}")" && echo X)" && MORA_SCRIPTS_DIR="${MORA_SCRIPTS_DIR%$'\nX'}"
DIR=$(python $MORA_SCRIPTS_DIR/impl_mora-cd.py $1)
cd $DIR


