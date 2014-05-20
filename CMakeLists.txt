# +====================================================================+
# |          Open MObile Robot Architecture (MORA)                     |
# |                                                                    |
# |                 https://github.com/OpenMORA                        |
# |                                                                    |
# | Copyright individual authors (C) 2010-14  - See AUTHORS.txt        |
# | Project released under BSD License                                 |
# +====================================================================+

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)  # Simpler CMake syntax.

PROJECT(mora_base)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11) # Minimum version required by MOOS v10 cfg files

cmake_policy(SET CMP0003 NEW)  # Don't mix rel & abs lib paths
cmake_policy(SET CMP0005 NEW)  # Autoescape -D definitions

#dump libs and executables in the same place
SET( LIBRARY_OUTPUT_PATH ${${PROJECT_NAME}_BINARY_DIR}/bin CACHE PATH    "Output directory for libraries" )
SET( EXECUTABLE_OUTPUT_PATH ${${PROJECT_NAME}_BINARY_DIR}/bin CACHE PATH "Output directory for executables" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/bin CACHE PATH "Output directory for executables" )

# Group projects in "folders"
IF (CMAKE_VERSION VERSION_GREATER "2.8.0")
	SET(ENABLE_SOLUTION_FOLDERS ON CACHE BOOL "Group projects under virtual folders of the compiler IDE (e.g. VisualStudio)")
ENDIF ()

if (ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

# -------------------------------------------------------------------
#   Patched versions of MOOS 
# -------------------------------------------------------------------
SET(MOOS_DIR "${${PROJECT_NAME}_BINARY_DIR}/core-moos/" CACHE PATH "MOOS build directory." )

# Override MOOS defaults for OpenMORA:
OPTION(USE_ASYNC_COMMS  "enable fast asynchronous comms architecture" ON) # Was: OFF
SET(DISABLE_NAMES_LOOKUP ON CACHE BOOL "Disable client names lookup in MOOSDB") # Was: OFF
# MOOS projects:
add_subdirectory(core-moos)
add_subdirectory(essential-moos)
# Group MOOS stuff together:
if(ENABLE_SOLUTION_FOLDERS)
	set_target_properties(MOOS PROPERTIES FOLDER "core-moos")
	set_target_properties(MOOSDB PROPERTIES FOLDER "core-moos")
	set_target_properties(umm PROPERTIES FOLDER "core-moos")
	# --
	set_target_properties(app_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(async_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(capture_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(cmp2sock PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(io_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(periodic_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(queue_test PROPERTIES FOLDER "core-moos [TESTS]")
	set_target_properties(reg_test PROPERTIES FOLDER "core-moos [TESTS]")
	# ---
	set_target_properties(pAntler PROPERTIES FOLDER "essential-moos")
	set_target_properties(pLogger PROPERTIES FOLDER "essential-moos")
	set_target_properties(pMOOSBridge PROPERTIES FOLDER "essential-moos")
	set_target_properties(pScheduler PROPERTIES FOLDER "essential-moos")
	set_target_properties(pShare PROPERTIES FOLDER "essential-moos")
endif()


# -------------------------------------------------------------------
#   Generate config files to be used by the rest of MORA pkgs
# -------------------------------------------------------------------
SET(MORA_INCLUDE_DIRS "${${PROJECT_NAME}_SOURCE_DIR}/modules-common/")
configure_file(
	"${${PROJECT_NAME}_SOURCE_DIR}/templates/MORAConfig.cmake.in"
	"${${PROJECT_NAME}_BINARY_DIR}/MORAConfig.cmake"
	@ONLY )


