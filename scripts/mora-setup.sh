#!/bin/bash

# Get script current directory:
MORA_SCRIPTS_DIR="$( dirname "$(readlink -e "${BASH_SOURCE[0]}")" && echo X)" && MORA_SCRIPTS_DIR="${MORA_SCRIPTS_DIR%$'\nX'}"
#echo "MORA_SCRIPTS_DIR: $MORA_SCRIPTS_DIR"

# Changing current directory MUST be done as a "source" command, not by invoking the shell script:
alias mora-cd="source $MORA_SCRIPTS_DIR/impl_mora-cd.sh"

# Parent dir:
MORABASE_DIR="$(dirname $MORA_SCRIPTS_DIR)"
# Parent dir:
OPENMORA_PKGS_DIR="$(dirname $MORABASE_DIR)"


# See docs on MORA env vars: https://github.com/OpenMORA/mora-base
export MORA_PATH=$OPENMORA_PKGS_DIR:$MORA_PATH
export MORA_EXECUTABLE_OUTPUT=$OPENMORA_PKGS_DIR/bin

# Allow execution of MORA scripts:
export PATH=$PATH:$MORA_SCRIPTS_DIR:$MORA_EXECUTABLE_OUTPUT

