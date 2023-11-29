//
// Created by omar on 10/4/23.
//

#include "Texture.h"
#include "SOIL/SOIL.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

Texture::Texture() : mTexture(0) {

}

Texture::~Texture() {
    glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(const string &filename, bool generateMipMaps) {
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image

    //using soil

//    int width, height;
//    unsigned char *image = SOIL_load_image("../res/img/bricks.jpg", &width, &height, 0, SOIL_LOAD_RGB);
//    //generate texture
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    SOIL_free_image_data(image);
//    glBindTexture(GL_TEXTURE_2D, 0);

    //using stb
    int width, height, bitDepth;
    unsigned char *texData = stbi_load(filename.c_str(), &width, &height, &bitDepth, 0);
    //catch error
    if (!texData) {
        cout << "Failed to find: " << filename.c_str() << std::endl;
        return 0;
    }
    GLenum format;
    if (bitDepth == 1) {
        format = GL_RED;
    } else if (bitDepth == 3) {
        format = GL_RGB;
    } else if (bitDepth == 4) {
        format = GL_RGBA;
    }
    //generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, texData);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    //free memory
    stbi_image_free(texData);
    return true;
}

void Texture::bind(GLuint texUnit) {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

unsigned int Texture::loadCubemap(vector <std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

//void unbind(GLuint texUnit = 0);