#pragma once
#include "GLM/glm.hpp"

typedef glm::vec3 Vector3;

struct CTransform {
	Vector3 Location = Vector3(0.0f);
	Vector3 Rotation = Vector3(0.0f);
	Vector3 Scale = Vector3(1.0f);

	// create the boolean operations off a CTransform
	bool operator==(CTransform& OtherT) {
		return Location == OtherT.Location && Rotation == OtherT.Rotation && Scale == OtherT.Scale;
	}

	bool operator!=(CTransform& OtherT) {
		return Location != OtherT.Location || Rotation != OtherT.Rotation || Scale != OtherT.Scale;
	}
};
struct CDirection {
	Vector3 Forward = Vector3(0.0f);
	Vector3 Right = Vector3(0.0f);
	Vector3 Up = Vector3(0.0f);
};