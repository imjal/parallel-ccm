#include <stdlib.h>
#include <stdio.h>

#include "mesh/objFile.cuh"
#include "mesh/meshInitialization.cuh"
#include "cudaSubdivision.cuh"

int main(int argc, char *argv[]) {
    ObjFile objFile = readObjFromFile("../models/OpenCube.obj");
    Mesh mesh = meshFromObjFile(objFile);
    freeObjFile(objFile);
	Mesh result = cudaSubdivide(&mesh, 1);
    toObjFile(&result);
}