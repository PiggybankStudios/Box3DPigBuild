/*
File:   build_script.c
Author: Taylor Robbins
Date:   07\04\2026
Description: 
	** A PigBuild-based build script for Box3D (https://github.com/erincatto/box3d)
*/

#include "pig_build.h"

int main(int argc, char* argv[])
{
	RecompileIfNeeded(StrArray_Empty);
	WriteLine("Hello Build!");
}
