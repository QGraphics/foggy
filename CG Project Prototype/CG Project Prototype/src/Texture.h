#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <string>

using namespace std;

class Texture {
public:
	Texture();
	virtual ~Texture();

	bool loadTexture(const string &filename, bool genMipMap = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint textUnit = 0);

private:
	GLuint mTexture;
};

#endif // TEXTURE_H
