#include "Texture.h"

#define GLEW_STATIC
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Stb_image/stb_image.h"
#include <iostream>

using namespace std;

Texture::Texture():mTexture(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(const string &filename, bool genMipMap)
{
	//Create textures
	GLint width, height, nComponents;
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//Set texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load image
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(filename.c_str(), &width, &height, &nComponents, 0);

	//std::cout << *texData;

	//printf("%s", texData);

	if (texData == NULL) {
		cout << stbi_failure_reason() << endl;
	}

	if (!texData)
	{
		printf("Failed to find: %s", filename.c_str());
	}

	GLenum format;

	cout << nComponents << endl;

	if (nComponents == 1)
	{
		format = GL_RED;
	}
	else if (nComponents == 3)
	{
		format = GL_RGB;
	}
	else if (nComponents == 4)
	{
		format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, texData);

	if (genMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(texData);

	return true;
}

void Texture::bind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}