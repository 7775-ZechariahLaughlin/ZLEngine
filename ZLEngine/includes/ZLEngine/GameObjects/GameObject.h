#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"
#include "ZLEngine/Graphics/Model.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"

class GameObject {
public: 

	GameObject();
	~GameObject();

	// return the location of the GameObject
	Vector3 FindLocation();
	// return the current facing direction of the GameObject
	Vector3 ReturnRotation();

public: 
	Vector3 GameObjectLocation;

	Vector3 GameObjectScale;

	Vector3 GameObjectRotation;

protected:
	GraphicsEnginePtr Graphics;

protected:

private:

private:

};
