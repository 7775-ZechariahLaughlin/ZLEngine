#pragma once
#include "ZlEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

class Input;

class Game {

public:

	//get the game instance or create one if it doesn't exist
	static Game& GetGameInstance();

	//destroy the game, running the destructor
	static void DestroyGameInstance();

	/* - start the game/application
	   - load the window */
	void Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	// get precise delta time
	double GetDeltaTime() { return DeltaTime; }

	// get less precise delta time
	float GetFDeltaTime() { return static_cast<float>(DeltaTime); }

	// return the graphics engine
	GraphicsEnginePtr GetGraphicsEngine() { return Graphics; }

	// set the bIsGameOver to true
	void CloseApp() { bIsGameOver = true; }

	// return the graphics engine default texture
	TexturePtr GetDefaultEngineTexture();

	// return the graphics engine default material
	MaterialPtr GetDefaultEngineMaterial();

	// remove a model from the viewport
	void RemoveModelFromGame(ModelPtr ModelToRemove);

	// used to clean up all models in the game
	void DeleteAllModels();

	// spawns all the objects used in the game
	void SpawnAllObjects();

	// translates all the objects to their start location
	void MoveAllObjects();

	// rotates all objects to their default rotation
	void RotateAllObjects();

	// scales the floors, all other objects are scaled when spawned
	// floors are not as they require uneven scaling
	void ScaleFloors();

	// detect and resolve any collisions between the player camera and objects
	void DetectAndResolveCollisions(Vector3 CameraInput);

private: 

	Game();
	~Game();


	//run the game loop
	void Run(); 

	//handle the input stream
	void ProcessInput();

	//handle the game logic
	void Update(); 

	/* - handle 3D graphics being drawn to the screen
	- will clear and present each frame*/
	void Draw();

	//handles what will happen when the game closes
	void CloseGame();

private: 

	// boolean that ends the game
	bool bIsGameOver;

	// boolean that specifies whether the player is grounded or not
	bool OnFloor;

	// boolean that specifices whether the player is currently jumping
	bool Jumping;

	// shared pointer to the graphics class
	GraphicsEnginePtr Graphics;

	// the time between each frame
	double DeltaTime;

	// read the input of the player
	Input* GameInput;

	// the current mode the player is in
	PlayerState CurrentState;

	// the next position the player should be moving to
	Vector3 PositionToMove;

	// stores the floors
	ModelPtr FloorStack[10];

	// stores pickups
	PickupPtr CoinPickup[11];
	PickupPtr SkullPickup[6];

	// stores all obstacles
	ObstaclePtr BoxObstacle[20];
	ObstaclePtr BarrelObstacle[10];
	ObstaclePtr Bulbs[10];
	ObstaclePtr Lamps[10];
	ObstaclePtr Walls[118];
	GatePtr Archways[10];
};