

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#ifdef _WIN32
#  include <windows.h>
#endif
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <cstring>

#include "ResourceLoader.h"

/***************************************
 * FUCNTION PROTOTYPES
 **************************************/
char* getCurrentDirectoryPath(char* fileName);


/***************************************
* loads a .obj file into passed variables
**************************************/
void ResourceLoader::loadObj(char *fileName, bool pathRelative, std::vector<std::vector<float> >* vertices, std::vector<std::vector<float> >* normals, std::vector<std::vector<int> >* faceIndices, std::vector<std::vector<int> >* normalIndices) {

    //if path is relative to running directory, get absolute path
    if (pathRelative)
        fileName = getCurrentDirectoryPath(fileName);

    FILE *objectFile;
    float x, y, z;

    //read file
    objectFile = fopen(fileName, "r");
    if (objectFile == NULL) {
        printf("Error. File \"%s\" could not be loaded.",fileName);
        exit(0);
    }

    //set up variables for reading data
    std::vector<float> vertexTemp;
    std::vector<float> normalTemp;
    std::vector<int> indexTemp;
    std::string vertex1, vertex2, vertex3;

    char line[128];
    int faceIndex[3], normalIndex[3];
    int res, matches;
    float normalLength = 1;

    //go through each line
    while (1) {

        //end of file
        res = fscanf(objectFile, "%s", line);
        if (res == EOF)
            break;

        //vertex
        if (strcmp(line, "v") == 0) {
            fscanf(objectFile, "%f %f %f\n", &x, &y, &z);
            vertexTemp.push_back(x);
            vertexTemp.push_back(y);
            vertexTemp.push_back(z);
            vertices->push_back(vertexTemp);
            vertexTemp.clear();
        }
        //normal
        else if (strcmp(line, "vn") == 0) {
            fscanf(objectFile, "%f %f %f\n", &x, &y, &z);
            normalLength = x + y + z;
            normalTemp.push_back(x/normalLength);
            normalTemp.push_back(y/normalLength);
            normalTemp.push_back(z/normalLength);
            normals->push_back(normalTemp);
            normalTemp.clear();
        }
        //index
        else if (strcmp(line, "f") == 0) {

            //load the data
            matches = fscanf(objectFile, "%d//%d %d//%d %d//%d\n", &faceIndex[0], &normalIndex[0], &faceIndex[1], &normalIndex[1], &faceIndex[2], &normalIndex[2]);

            //check that .obj file is in right format
            if (matches != 6) {
                printf("Error. Object file %s must have only face and normal indices", fileName);
                exit(1);
            }

            indexTemp.push_back(faceIndex[0]-1);
            indexTemp.push_back(faceIndex[1]-1);
            indexTemp.push_back(faceIndex[2]-1);
            faceIndices->push_back(indexTemp);
            indexTemp.clear();

            indexTemp.push_back(normalIndex[0]-1);
            indexTemp.push_back(normalIndex[1]-1);
            indexTemp.push_back(normalIndex[2]-1);
            normalIndices->push_back(indexTemp);
            indexTemp.clear();
        }

    }
    fclose(objectFile);
}

/***************************************
* returns current directory of running
* executable concatenated with fileName.
**************************************/
char* getCurrentDirectoryPath(char* fileName) {

    //initialize path
    char currentDirPath[1024];

    //if path not too long, return it concatenated with fileName
    if (getcwd(currentDirPath, sizeof(currentDirPath)) != NULL) {

        //create path
        char* filePath = (char*) malloc(strlen(currentDirPath)+strlen(fileName)+1);
        strcpy(filePath, currentDirPath);
        strcat(filePath, fileName);

        return filePath;
    }

    //print error message and exit
    else {
        printf("\nError. Could not get current directory path.");
        exit(0);
    }
}