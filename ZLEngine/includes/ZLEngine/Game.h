#pragma once
#include "CoreMinimal.h"

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

	// shared pointer to the graphics class
	GraphicsEnginePtr Graphics;

	// shared pointer to the pickup class
	PickupPtr Pickups;

	// the time between each frame
	double DeltaTime;

	// read the input of the player
	Input* GameInput;

	//temporary mesh variables
	ModelPtr Model;
	ModelPtr Model2;

	ModelPtr WallStack[50];
	ModelPtr FloorStack[10];

	ModelPtr Lamp1;
	ModelPtr Lamp2;
	ModelPtr Coin1;
	ModelPtr Coin;
	ModelPtr Skull1;
	ModelPtr Barrel1;
	ModelPtr Archway1;
	ModelPtr Archway2;
	// pickup variables

	PickupPtr CoinPickup[11];
};