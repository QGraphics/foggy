#ifndef CELESTIAL_H

#define CELESTIAL_H

#include "GLM//glm.hpp"
#include "Mesh.h"
#include "Texture.h"

class Celestial
{
public:
	Celestial();

	const bool loadTex(const std::string& filename)
	{
		return mTex.loadTexture(filename);
	}

	const bool loadObj(const std::string& filename)
	{
		return mMesh.loadOBJ(filename);
	}

	const void render()
	{
		mTex.bind(0);
		mMesh.draw();
		mTex.unbind(0);
	}

	const glm::vec3 getPosition()
	{
		return mPosition;
	}

	const void setPosition(glm::vec3 position)
	{
		mPosition = position;
	}

	const glm::vec3 getVelocity()
	{
		return mVelocity;
	}

	const void addVelocity(glm::vec3 velocity)
	{
		//cout << mVelocity.x + velocity.x << endl;

		mVelocity = mVelocity + velocity;
	}

	const float getMass()
	{
		return mMass;
	}

	const void setMass(float mass)
	{
		mMass = mass;
	}

	const glm::vec3 getRadii()
	{
		return glm::vec3(mRadius, mRadius, mRadius);
	}

	const void setRadius(double radius)
	{
		mRadius = radius;
	}

protected:
	float mMass;
	float mRadius;
	glm::vec3 mPosition;
	glm::vec3 mVelocity;
	Mesh mMesh;
	Texture mTex;
};

#endif // !CELESTIAL_H
