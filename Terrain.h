#ifndef TERRAIN_H
#define TERRAIN_H

#include "mathLib3D.h"

using namespace std;

class Terrain {
public:
	Terrain();
	int tSize = 257; //size of terrain
	float minHeight;
    float maxHeight;
    float heightMap[257][257];
    Vec3D normals[257][257];

private:
	//height map functions
	void generateHeightMap();
	void diamondSquare(int size);
	void square(int x, int z, int range);
	void diamond(int x, int z, int range);
	void getMinMax();

	//calculates surface normals 
	void getNormals();
};

#endif