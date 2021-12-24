#ifndef OBJ_FILE_CUH
#define OBJ_FILE_CUH

#include <stdlib.h>
#include <stdio.h>

typedef struct ObjFile {
	// these have length numVerts
	float* xCoords;
	float* yCoords;
	float* zCoords;

	// these all have length numHalfEdges
	int** faceIndices;
    int* faceValencies;

  	int numFaces;
  	int numVerts;

} ObjFile;

ObjFile readObjFromFile(char const* objFileName);

#endif // OBJ_FILE_CUH