//
// Created by omar on 10/4/23.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <map>

using namespace std;

class Texture {
public:
    Texture();
    virtual ~Texture();

    bool loadTexture(const string &filename, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);
//    void unbind(GLuint texUnit = 0);

private:
    GLuint mTexture;
//    string mFilename;

};

#endif //TEXTURE.H
