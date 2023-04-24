#include "ZLEngine/Graphics/Model.h"
#include "ZLEngine/Graphics/Mesh.h"
#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/Vertex.h"
#include "ZLEngine/Collisions/Collision.h"
#include "ZLEngine/GameObjects/Pickup.h"

/* ASSIMP HEADERS */
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model()
{
	ModelFilePath = "";
	ModelCollision = nullptr;
}

Model::~Model()
{
	// clean up the model
	MeshStack.clear();
	MaterialStack.clear();
	ModelShader = nullptr;
}

bool Model::CreateSimpleMesh(GeometricShapes Shape, ShaderPtr MeshShader)
{
	// initialise the mesh
	MeshPtr NewMesh = make_shared<Mesh>();

	// try to create a simple shape
	if (!NewMesh->CreateSimpleShape(Shape, MeshShader, 0)) {
		return false;
	}

	// clear the old data if there is any and assign the new mesh data
	MeshStack.clear();
	MaterialStack.clear();
	this->ModelShader = ModelShader;

	// assign the default engine material to the first material slot
	MaterialStack.push_back(Game::GetGameInstance().GetDefaultEngineMaterial());

	// assign the mesh to the model mesh stack
	MeshStack.push_back(NewMesh);

	return true;
}

bool Model::ImportMeshFromFile(const char* ImportFilePath, ShaderPtr ModelShader)
{
	// create an importer using ASSIMP
	Assimp::Importer Importer;
	// attempt to import the new mesh based on the file path using the importer
	const aiScene* Scene = Importer.ReadFile(ImportFilePath, aiProcess_Triangulate);

	/* check if the file imported correctly
	 * !Scene means the file path probably didnt work
	 * AI_SCENE_FLAGS_INCOMPLETE means the file is probably corrupted or can't be read by ASSIMP
	 * !Scene->mRootNode means it's also either corrupted or not supported by ASSIMP */
	if (!Scene || Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
		cout << "Model | Importer | Error importing model from " << 
			ImportFilePath << " - " << Importer.GetErrorString() << endl;
	}

	// assign the model shader
	this->ModelShader = ModelShader;

	// save the file path
	ModelFilePath = ImportFilePath;

	// find and import all of the meshes using the imported scene
	FindAndImportSceneMeshes(Scene->mRootNode, Scene);

	// run through all the new material slots and assign the default material
	for (zluint i = 0; i < MaterialStack.size(); i++) {
		MaterialStack[i] = Game::GetGameInstance().GetDefaultEngineMaterial();
	}

	return true;
}

void Model::Draw()
{
	//cycle through the meshes and draw each one
	for (MeshPtr LMesh : MeshStack) {
		// assign the model transformations to the mesh
		LMesh->Transform = this->Transform;
		
		// draw the mesh using the material slot it has been assigned
		LMesh->Draw(MaterialStack[LMesh->GetMaterialSlot()]);
	}
	if (ModelCollision != nullptr) {
		ModelCollision->SetLocation(Transform.Location);
	}
}

void Model::SetMaterialBySlot(zluint SlotIndex, MaterialPtr NewMaterial)
{
	// check if the index is valid to the array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | Material Slot | No slot exists of that index ( " 
			<< SlotIndex << " )" << endl;
		return;
	}

	// reassign the array item
	MaterialStack[SlotIndex] = NewMaterial;
}

MaterialPtr Model::GetMaterialBySlot(zluint SlotIndex)
{
	// check if the index is valid to the array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | Material Slot | No slot exists of that index ( "
			<< SlotIndex << " )" << endl;
		return nullptr;
	}

	return MaterialStack[SlotIndex];
}

CollisionPtr Model::AddCollisionToModel(Vector3 Dimensions, Vector3 Offset)
{
	ModelCollision = make_shared<BoxCollision>(Transform.Location, Offset, Dimensions);

	return ModelCollision;
}

void Model::FindAndImportSceneMeshes(aiNode* Node, const aiScene* Scene)
{
	// run through all of the main meshes in the root node of the scene and import those
	for (zluint i = 0; i < Node->mNumMeshes; i++) {
		// get the mesh index
		zluint ImpMeshIndex = Node->mMeshes[i];
		// get one of the meshes
		aiMesh* ImpMesh = Scene->mMeshes[ImpMeshIndex];

		// convert the imported mesh to our engine's mesh
		MeshPtr ConvertedMesh = ConvertImportMeshToEngineMesh(ImpMesh, Scene);

		// add the new converted mesh into our mesh array
		if (ConvertedMesh != nullptr) {
			MeshStack.push_back(ConvertedMesh);
		}
	}
	
	// check through all meshes that are children of the root meshes
	// wont run if the mesh has no children
	for (zluint i = 0; i < Node->mNumChildren; i++) {
		FindAndImportSceneMeshes(Node->mChildren[i], Scene);
	}
}

MeshPtr Model::ConvertImportMeshToEngineMesh(aiMesh* ImportMesh, const aiScene* Scene)
{
	// initialise local versions of the mesh vertices and indices
	vector<Vertex> Vertices;
	vector<zluint> Indices;

	// loop through all of the vertices and store their location, normal value and tex coord
	for (zluint i = 0; i < ImportMesh->mNumVertices; i++) {
		// initialise a single vertex
		Vertex LVertex;

		// find the position of the vertex
		LVertex.Position = Vector3(
			ImportMesh->mVertices[i].x,
			ImportMesh->mVertices[i].y,
			ImportMesh->mVertices[i].z
		);

		// find the normal value of the vertex
		LVertex.Normal = Vector3(
			ImportMesh->mNormals[i].x,
			ImportMesh->mNormals[i].y,
			ImportMesh->mNormals[i].z
		);

		// find the texture coordinates of the vertex
		LVertex.TextCoords = Vector2(
			ImportMesh->mTextureCoords[0][i].x,
			ImportMesh->mTextureCoords[0][i].y
		);

		// add the vertex into the vertices array
		Vertices.push_back(LVertex);
	}

	// make sure there are more than 3 vertices or stop creating the mesh
	if (Vertices.size() < 3) {
		cout << "Model | Converter | One of the imported meshes doesn't have enough vertices at " << ModelFilePath << endl;
		return nullptr;
	}

	// next we find the indices
	// to find the indices we can use a value called faces
	// faces are basically 3 vertices
	for (zluint i = 0; i < ImportMesh->mNumFaces; i++) {
		// store the face
		aiFace Face = ImportMesh->mFaces[i];

		// run through another loop to detect all of the indices in the face
		for (zluint j = 0; j < Face.mNumIndices; j++) {
			// add each index to the indices array
			Indices.push_back(Face.mIndices[j]);
		}
	}

	// find and assign material slots
	zluint MaterialIndex = ImportMesh->mMaterialIndex;

	// resize the material stack to whatever the largest material slot index is
	if (MaterialStack.size() < MaterialIndex + 1) {
		// change the size of the material stack to the largest index + 1
		MaterialStack.resize(MaterialIndex + 1);
	}

	// create and assign the mesh 
	MeshPtr ConvertedMesh = make_shared<Mesh>();
	ConvertedMesh->CreateMesh(Vertices, Indices, ModelShader, MaterialIndex);

	return ConvertedMesh;
}
