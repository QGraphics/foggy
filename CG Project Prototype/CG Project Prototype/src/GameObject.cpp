#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
	:mForce(0.0f, 0.0f, 0.0f),
	mMass(1.0f),
	mPosition(0.0f, 0.0f, 0.0f),
	mRotation(0.0f, 0.0f, 0.0f),
	mSize(1.0f, 1.0f, 1.0f),
	mScale(1.0f, 1.0f, 1.0f),
	mVelocity(0.0f, 0.0f, 0.0f)
{
	glm::vec3 halfSizes = 0.5f * mSize;

	mMaxes = mPosition + halfSizes;
	mMins = mPosition - halfSizes;
}

bool GameObject::detectCollision(GameObject* other)
{

	bool xColl = 
		mPosition.x >= other->mMins.x &&
		mPosition.x <= other->mMaxes.x
	;

	bool yColl =
		mPosition.y >= other->mMins.y &&
		mPosition.y <= other->mMaxes.y
	;

	bool zColl =
		mPosition.z >= other->mMins.z &&
		mPosition.z <= other->mMaxes.z
	;

	return xColl && yColl && zColl;
}

bool GameObject::loadOBJ(const std::string& filename)
{
	return mMesh.loadOBJ(filename);
}

bool GameObject::loadTexture(const std::string& filename)
{
	return mTex.loadTexture(filename);
}

void GameObject::render()
{
	mTex.bind(0);
	mMesh.draw();
	mTex.unbind(0);
}

void GameObject::setPosition(glm::vec3 pos)
{
	mPosition = pos;
	glm::vec3 halfSizes = 0.5f * mSize;
	mMaxes = mPosition + halfSizes;
	mMins = mPosition - halfSizes;
}

void GameObject::setScale(glm::vec3 scale)
{
	mScale = scale;
	glm::vec3 halfSizes = 0.5f * mSize;
	mMaxes = mPosition + halfSizes;
	mMins = mPosition - halfSizes;
}

void GameObject::update(float dt)
{
	float g = -9.81f;
	mForce.y = g;

	//std::cout << dt << std::endl;

	if (mMins.y >= 0)
	{
		mVelocity.y += mForce.y * dt;
		mPosition.y += mVelocity.y * dt;

		setPosition(mPosition);

		//cout << mVelocity.y << endl;
	}
	else
	{
		mVelocity.y = 0;
	}
}

void GameObject::addForce(glm::vec3 force)
{
	mVelocity += force;
	//std::cout << mForce.y << endl;
}