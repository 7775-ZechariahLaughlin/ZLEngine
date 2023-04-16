#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

struct Vec3Texture {
	TexturePtr TextureV3;
	Vector3 MultiplierV3 = Vector3(1.0f);
};

class Material {
public:
	Material();
	~Material();

	void Draw(ShaderPtr Shader);

public:
	// store the colour map and multiplier of each value
	Vec3Texture BaseColour;
	Vec3Texture SpecularColour;
	Vec3Texture EmissiveColour;

	float Shininess;

};
