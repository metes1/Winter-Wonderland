
#include <vector>

class ResourceLoader {

    /***************************************
    *    PUBLIC FUNCTIONS
    **************************************/
    public:


        float** convertImageToHeightmap(GLubyte* image, int heightmapRadius);
        void loadObj(char *fileName, bool pathRelative, std::vector<std::vector<float> >* vertices, std::vector<std::vector<float> >* normals, std::vector<std::vector<int> >* faceIndices, std::vector<std::vector<int> >* normalIndices);


};