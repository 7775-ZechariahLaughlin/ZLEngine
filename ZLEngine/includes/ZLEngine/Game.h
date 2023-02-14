#pragma once
#include <iostream>
#include "ZLEngine/Graphics/GraphicsEngine.h"
using namespace std;

class Game {

public:

	//get the game instance or create one if it doesn't exist
	static Game& GetGameInstance();

	//destroy the game, running the destructor
	static void DestroyGameInstance();

	/* - start the game/application
	 - load the window */
	void Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

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

	bool bIsGameOver;

	GraphicsEngine* Graphics;



	
};