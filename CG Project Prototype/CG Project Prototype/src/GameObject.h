#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "Mesh.h"
#include "Texture.h"

class GameObject
{
public:
	GameObject();

	void addForce(glm::vec3 force);
	bool loadOBJ(const std::string& filename);
	bool loadTexture(const std::string& filename);
	void update(float dt);
	void onCollision(GameObject* other);
	bool detectCollision(GameObject* other);
	void render();
	void setPosition(glm::vec3 pos);
	const glm::vec3 getPosition() { return mPosition;  }
	void setScale(glm::vec3 scale);
	const glm::vec3 getScale() { return mScale; }
	const glm::vec3 getMins() { return mMins; }
	const glm::vec3 getMaxes() { return mMaxes; }
	const void setSize(glm::vec3 size) 
	{
		mSize = size;
		setPosition(mPosition);
	}

protected:
	glm::vec3 mPosition;
	glm::vec3 mSize;
	glm::vec3 mRotation;
	glm::vec3 mForce;
	glm::vec3 mVelocity;
	glm::vec3 mMins;
	glm::vec3 mMaxes;
	glm::vec3 mScale;
	Mesh mMesh;
	Texture mTex;

	float mMass;
};

#endif // !GAMEOBJECT_H
