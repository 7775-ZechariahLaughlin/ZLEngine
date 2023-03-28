#pragma once
#include "ZLEngine/CoreMinimal.h"

class Material {
public:
	Material();
	~Material();

	void Draw(ShaderPtr Shader);

public:
	TexturePtr BaseColour;

};
