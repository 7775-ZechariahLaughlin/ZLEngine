#include "ZLEngine/Graphics/Mesh.h"
#include "ZLEngine/Graphics/ShaderProgram.h"
#include "ZLEngine/Graphics/Texture.h"
#include "ZLEngine/Graphics/VertexArrayObject.h"
#include "GLM/gtc/matrix_transform.hpp"

Mesh::Mesh()
{
	cout << "Mesh || Initialised Mesh... " << endl;
	Transform.Rotation.z = 45.0f;
}

Mesh::~Mesh()
{
	MeshShader = nullptr;
	MeshTextures.clear();
	MeshVAO = nullptr;

	cout << "Mesh || Mesh Destroyed... " << endl;
}

bool Mesh::CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, TexturePtrStack MeshTextures)
{
	cout << "Mesh || Creating Mesh... " << endl;

	// create the VAO
	MeshVAO = make_shared<VAO>(Shape);

	// validate the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh || Mesh failed to be created." << endl;
		return false;
	}

	// assign the shader and textures
	this->MeshShader = MeshShader;
	this->MeshTextures = MeshTextures;

	cout << "Mesh || Mesh created successfully." << endl;

	return true;
}

void Mesh::Draw()
{
	// activate the shader that this mesh uses
	MeshShader->RunShader();

	// activate the required textures for this mesh
	for (zluint Index = 0; Index < MeshTextures.size(); Index++) {
		// activating the texture through OpenGL
		MeshTextures[Index]->ActivateTexture(Index);
		// setting the textures number as the active texture in the shader
		MeshShader->SetInt("TextureColour", Index);
		// binding the texture to the shader
		MeshTextures[Index]->BindTexture();
	}

	// initialise a static variable to check if any changes to transform
	static CTransform OldTransform;

	if (Transform != OldTransform) {
		OldTransform = Transform;

		glm::mat4 MatTransform = glm::mat4(1.0f);

		//translate - rotate - scale ---- in this order
		//translate
		MatTransform = glm::translate(MatTransform, Transform.Location);
		//rotate
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.x), Vector3(1.0f, 0.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.y), Vector3(0.0f, 1.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.z), Vector3(0.0f, 0.0f, 1.0f));
		//scale
		MatTransform = glm::scale(MatTransform, Transform.Scale);

		MeshShader->SetMat4("transform", MatTransform);
	}

	// draw the VAO
	MeshVAO->Draw();
}
