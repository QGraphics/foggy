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
#include "glm/glm.hpp"


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
	void modify(glm::vec3 position, float strength, bool up = true);
	void generatePlane(int div, float width);
	void draw();

private:

	void initBuffers();
	std::vector<GLuint> getPlaneIds(int div);
	static float gaussian(float x, float strength)
	{
		return expf(-powf(x, 2) * strength);
	}

	bool mLoaded;
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	GLuint mVBO, mVAO, mIBO;
};
#endif //MESH_H
