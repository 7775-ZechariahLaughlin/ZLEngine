#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

struct Vertex;

class Mesh {
public:
	Mesh();
	~Mesh();

	// create the mesh out of VAO, a Shader and Textures
	bool CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, zluint MaterialSlot);

	// create a mesh VAO that has a predefined model
	bool CreateMesh(vector<Vertex> Vertices, vector<zluint> Indices, ShaderPtr MeshShader, zluint MaterialSlot);

	// handle the drawing of all the required classes
	void Draw(MaterialPtr MeshMaterial);

	// return material slot assigned by the model to the mesh
	zluint GetMaterialSlot() const { return MaterialSlot; }

public: 
	// this holds the position, rotation and scale of the mesh
	CTransform Transform;

private:
	// store the shader this mesh requires
	ShaderPtr MeshShader;

	// assigned material slot relevant to the model class
	zluint MaterialSlot;

	// create a new VAO for the mesh
	VAOPtr MeshVAO;

};