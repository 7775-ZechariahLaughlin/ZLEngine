#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Mesh.h"

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
		ShaderPtr TextureShader = Graphics->CreateShader({
			L"game/shaders/TextureShader/TextureShader.svert",
			L"game/shaders/TextureShader/TextureShader.sfrag"
			});
		//create textures
		TexturePtr TWood = Graphics->CreateTexture("game/textures/WoodTexture.jpg");
		TexturePtr TSquares = Graphics->CreateTexture("game/textures/GreySquare.jpg");

		// create a mesh
		Graphics->CreateSimpleMeshShape(GeometricShapes::Polygon, TextureShader, { TSquares });
		Graphics->CreateSimpleMeshShape(GeometricShapes::Triangle, TextureShader, { TWood });

		// initial transformations for the meshes
		Poly->Transform.Location.x = 0.5f;
		Tri->Transform.Location.x = -0.5f;

		Poly->Transform.Scale = Vector3(0.5f);
		Tri->Transform.Scale = Vector3(0.9f);

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
	// set delta time first always
	static double LastFrameTime = 0.0;
	// set the current time since the game has passed
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	// find the time difference between the last and current frame
	double NewDeltaTime = CurrentFrameTime - LastFrameTime;
	// set delta time as seconds
	DeltaTime = NewDeltaTime / 1000.0;
	// update the last frame time for the next update
	LastFrameTime = CurrentFrameTime;

	static int MoveUp = 1.0f;

	if (Tri->Transform.Location.y > 0.5) {
		MoveUp = -1.0f;
	}
	if (Tri->Transform.Location.y < -0.5f) {
		MoveUp = 1.0f;
	}

	Tri->Transform.Location.y += (2.0f * MoveUp) * GetFDeltaTime();

	//TODO: Handle logic
	Poly->Transform.Rotation.z += 50.0f * GetFDeltaTime();
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
