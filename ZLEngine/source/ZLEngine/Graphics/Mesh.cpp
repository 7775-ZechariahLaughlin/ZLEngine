#include "ZLEngine/Graphics/Mesh.h"
#include "ZLEngine/Graphics/ShaderProgram.h"
#include "ZLEngine/Graphics/Material.h"
#include "ZLEngine/Graphics/VertexArrayObject.h"
#include "GLM/gtc/matrix_transform.hpp"
#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"

Mesh::Mesh()
{
	cout << "Mesh | Initialised Mesh... " << endl;
}

Mesh::~Mesh()
{
	MeshShader = nullptr;
	MeshMaterial = nullptr;
	MeshVAO = nullptr;

	cout << "Mesh | Mesh Destroyed... " << endl;
}

bool Mesh::CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, MaterialPtr MeshMaterial)
{
	cout << "Mesh | Creating Mesh... " << endl;

	// create the VAO
	MeshVAO = make_shared<VAO>(Shape);

	// validate the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh | Mesh failed to be created." << endl;
		return false;
	}

	// assign the shader and textures
	this->MeshShader = MeshShader;
	this->MeshMaterial = MeshMaterial;

	cout << "Mesh | Mesh created successfully." << endl;

	return true;
}

void Mesh::Draw()
{
	// activate the shader that this mesh uses
	MeshShader->RunShader();

	// run the material for this mesh
	// activate all required textures
	if (MeshMaterial != nullptr) {
		MeshMaterial->Draw(MeshShader);
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

		MeshShader->SetMat4("model", MatTransform);
	}

	// create the world and screen transformations for this object
	Game::GetGameInstance().GetGraphicsEngine()->ApplyScreenTransformations(MeshShader);

	// draw the VAO
	MeshVAO->Draw();
}
