//-----------------------------------------------------------------------------
// Mesh.h by 
// 
// Basic OBJ Mesh Importer class
//-----------------------------------------------------------------------------
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLM/glm.hpp"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
public:

	 Mesh();
	~Mesh();

	bool loadOBJ(const std::string& filename);
	void modify(glm::vec3 position, float strength);
	void generatePlane(int div, float width);
	void draw();

private:

	void initBuffers();
	std::vector<GLuint> getPlaneIds(int div);

	bool mLoaded;
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	GLuint mVBO, mVAO, mIBO;
};
#endif //MESH_H
