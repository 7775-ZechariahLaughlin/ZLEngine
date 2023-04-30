#pragma once

#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int zluint;

//class/type definitions

class GraphicsEngine;
typedef shared_ptr<GraphicsEngine> GraphicsEnginePtr;

class VertexArrayObject;
typedef VertexArrayObject VAO;
typedef shared_ptr<VAO> VAOPtr;

class Texture;
typedef shared_ptr<Texture> TexturePtr;
typedef vector<TexturePtr> TexturePtrStack;

class ShaderProgram;
typedef shared_ptr<ShaderProgram> ShaderPtr;

class Mesh;
typedef shared_ptr<Mesh> MeshPtr;
typedef vector<MeshPtr> MeshPtrStack;

class Model;
typedef shared_ptr<Model> ModelPtr;
typedef vector<ModelPtr> ModelPtrStack;

class Material;
typedef shared_ptr<Material> MaterialPtr;
typedef vector<MaterialPtr> MaterialPtrStack;

class Camera;
typedef shared_ptr<Camera> CameraPtr;

class Collision;
typedef shared_ptr<Collision> CollisionPtr;
typedef vector<CollisionPtr> CollisionPtrStack;

class BoxCollision;
typedef shared_ptr<BoxCollision> BoxCollisionPtr;

class GameObject;
typedef shared_ptr<GameObject> GameObjectPtr;

class Gate;
typedef shared_ptr<Gate> GatePtr;

class Pickup;
typedef shared_ptr<Pickup> PickupPtr;

class Obstacle;
typedef shared_ptr<Obstacle> ObstaclePtr;

class ScreenText;
typedef shared_ptr<ScreenText> ScreenTextPtr;

class Player;
typedef shared_ptr<Player> PlayerPtr;

class SpawnHandler;
typedef shared_ptr<SpawnHandler> SpawnHandlerPtr;

//useful for shape data and VAOs Matrices
typedef vector<float> PositionMatrix;
typedef vector<zluint> IndicesMatrix;

//this structure will handle matrices for VAOs
struct ShapeData {
	vector<float> PositionMatrix;
	vector<zluint> IndicesMatrix;
};

// a list of the geometric shapes available in the VAO
enum class GeometricShapes {
	Triangle = 0, 
	Polygon, 
	Circle,
	Pentagon,
	Cube, 
	Floor
};

// store the parameters required to create a shader program
struct VFShaderParams {
	const wchar_t* VertexShaderPath;
	const wchar_t* FragmentShaderPath;
};

// the types of shaders that can be accepted in our shader program
enum class ShaderTypes {
	Vertex = 0, 
	Fragment
};

// the different types of obstacles that are in the game
enum class ObstacleTypes {
	Wall = 0,
	Gate,
	Barrel,
	Box,
	Lamp
};

// the different types of pickups in the game
enum class PickupTypes {
	Coin = 0,
	Skull
};

// the states the player can have, includes EditorMode, PlayerMode
enum class PlayerState {
	EditorMode = 0,
	PlayerMode,
};
