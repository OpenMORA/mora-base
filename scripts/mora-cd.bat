@echo off
REM # cd (change dir) into the given module. Default workspace is the parent dir of mora-base path.
REM #  Usage:
REM #   mora-cd                 # goes to default modules workspace directory
REM #   mora-cd [MODULE_NAME]   # goes to module dir
REM #
REM # Part of OpenMora - https://github.com/OpenMORA

for /f %%i in ('python %~dp0\impl_mora-cd.py %*') do set DIR=%%i
cd %DIR%
