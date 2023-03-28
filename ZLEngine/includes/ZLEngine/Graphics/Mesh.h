#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

class Mesh {
public:
	Mesh();
	~Mesh();

	// create the mesh out of VAO, a Shader and Textures
	bool CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, MaterialPtr MeshMaterial);

	// handle the drawing of all the required classes
	void Draw();

	// this holds the position, rotation and scale of the mesh
	CTransform Transform;

private:
	// store the shader this mesh requires
	ShaderPtr MeshShader;

	// assign a material to the mesh
	MaterialPtr MeshMaterial;

	// create a new VAO for the mesh
	VAOPtr MeshVAO;

};