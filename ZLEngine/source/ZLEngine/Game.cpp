#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"

Game& Game::GetGameInstance()
{
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();

	delete GameInstance;

}
void Game::Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight)
{
	//Load the window using the graphics class

	//if the window fails to load then bIsGameOver = true

	Graphics = make_shared<GraphicsEngine>();

	if (!Graphics->InitGE(WTitle, bFullScreen, WWidth, WHeight)) {
		bIsGameOver = true;
	}

	Run();

}
Game::Game() 
{
	cout << "Game Initialised!" << endl;

	Graphics = nullptr;
	bIsGameOver = false;
}
Game::~Game()
{
	/*since nothing else is storing graphics in memory 
	this will destroy the graphics from the memory*/
	Graphics = nullptr;
	cout << "Game Over!" << endl;

}

void Game::Run()
{
	if (!bIsGameOver) {
		//create a shader
		Graphics->CreateShader({
			L"game/shaders/TextureShader/TextureShader.svert",
			L"game/shaders/TextureShader/TextureShader.sfrag"
			});
		//create a texture
		Graphics->CreateTexture("game/textures/WoodTexture.jpg");
		//add a VAO to the stack
		Graphics->CreateVAO(GeometricShapes::Polygon);

	}
	//as long as the game isn't over run the loop
	while (!bIsGameOver) {
		//make sure we process user inputs
		ProcessInput();

		//apply the logic based on the inputs and AI logic
		Update();

		//render the screen based on the two prior functions
		Draw();
	}

	//clean the game after it ends
	CloseGame();
}

void Game::ProcessInput()
{
	//TODO: Handle inputs
	SDL_Event PollEvent;

	//this is waiting for inputs to be pressed
	while (SDL_PollEvent(&PollEvent)) {
		//checking what input was pressed
		switch (PollEvent.type) {
		case SDL_QUIT: //on close button pressed
			bIsGameOver = true;
			break;
		default:
			break;
		} 
	}
}

void Game::Update()
{
	//TODO: Handle logic
}

void Game::Draw()
{
	//TODO: Draw graphics to screen
	Graphics->Draw();
}

void Game::CloseGame()
{
	//TODO: Clean up code
}
