#include "Terrain.h"
#include <stdlib.h>
#include <cmath>
#include "mathLib3D.h"

using namespace std;

Terrain::Terrain()
{
	generateHeightMap(); //generates height map
	getMinMax();
	getNormals(); // calculates and stores surface normals
}

void Terrain::generateHeightMap()
{
	//set corners to random values
	heightMap[0][0] = 1;
	heightMap[0][tSize-1] = 1;
	heightMap[tSize-1][0] = 1;
	heightMap[tSize-1][tSize-1] = 1;

	//diamond-square algorithm to generate height values
	int stepSize = tSize/2;
	diamondSquare(stepSize);
}

void Terrain::diamondSquare(int size)
{
	int half = size/2;
	if (half < 1)
		return;

	for (int z = half; z < tSize; z+=size)
		for (int x = half; x < tSize; x+=size)
			square(x % tSize, z % tSize, half);

	int col = 0;
 	for (int x = 0; x < tSize; x += half) {
  		col++;

  		if (col % 2 == 1)
    		for (int z = half; z < tSize; z += size)
    			diamond(x % tSize, z % tSize, half);
  		else
   			for (int z = 0; z < tSize; z += size)
    			diamond(x % tSize, z % tSize, half);
 	}
	diamondSquare(size / 2);
}	

void Terrain::square(int x, int z, int range)
{
	int count = 0;
	float avg = 0.0f;
	if (x - range >= 0 && z - range >= 0)
		if (x - range >= 0 && z - range >= 0) {
	  		avg += heightMap[x-range][z-range];
	  		count++;
	 	}

	if (x - range >= 0 && z + range < tSize) {
		avg += heightMap[x-range][z+range];
	  	count++;
	}

	if (x + range < tSize && z - range >= 0) {
		avg += heightMap[x+range][z-range];
		count++;
	}
	 
	if (x + range < tSize && z + range < tSize) {
		avg += heightMap[x+range][z+range];
		count++;
	}

	avg += (rand() % (range*4)) - range;
	avg /= count;
	heightMap[x][z] = round(avg);
}

void Terrain::diamond(int x, int z, int range)
{
	int count = 0;
	float avg = 0.0f;

	if (x - range >= 0) {
  		avg += heightMap[x-range][z];
  		count++;
 	}

	if (x + range < tSize) {
  		avg += heightMap[x+range][z];
  		count++;
 	}
 	
 	if (z - range >= 0) {
		avg += heightMap[x][z-range];
  		count++;
 	}
 
 	if (z + range < tSize) {
		avg += heightMap[x][z+range];
  		count++;
 	}

 	avg += (rand() % (range*4)) - range;
 	avg /= count;
 	heightMap[x][z] = (int)avg;
}

//calculate vertex normals
void Terrain::getNormals()
{
	for (int z = 1; z < tSize-1; z++){
        for (int x = 1; x < tSize-1; x++){
        	Vec3D vecR(1, heightMap[x+1][z]-heightMap[x][z], 0);
        	Vec3D vecL(-1, heightMap[x-1][z]-heightMap[x][z], 0);
        	Vec3D vecD(0, heightMap[x][z+1]-heightMap[x][z], 1);
        	Vec3D vecU(0, heightMap[x][z-1]-heightMap[x][z], -1);

		    //get cross products
		    Vec3D vecUR = vecU.cross(vecR);
		    Vec3D vecRD = vecR.cross(vecD);
		    Vec3D vecDL = vecD.cross(vecL);
		    Vec3D vecLU = vecL.cross(vecU);

		    //get average normal
		    float aX = (vecUR.dx + vecRD.dx + vecDL.dx + vecLU.dx)/4.0;
		    float aY = (vecUR.dy + vecRD.dy + vecDL.dy + vecLU.dy)/4.0;
		    float aZ = (vecUR.dz + vecRD.dz + vecDL.dz + vecLU.dz)/4.0;
		    Vec3D avgN = Vec3D(aX, aY, aZ);

		    normals[x][z] = avgN.normalize();
        }
    }
}

//get min and max height values
void Terrain::getMinMax()
{
	minHeight = 0;
	maxHeight = 0;

	for (int iX = 0; iX < tSize; iX++) {
		for (int iZ = 0; iZ < tSize; iZ++) {
			if (heightMap[iX][iZ] < minHeight)
				minHeight = heightMap[iX][iZ];
			else if (heightMap[iX][iZ] > maxHeight)
				maxHeight = heightMap[iX][iZ];
		}
	}
}