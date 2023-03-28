#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Mesh.h"
#include "ZLEngine/Input.h"
#include "ZLEngine/Graphics/Camera.h"
#include "ZLEngine/Graphics/Material.h"

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
TexturePtr Game::GetDefaultEngineTexture()
{
	return Graphics->DefaultEngineTexture;
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
		GameInput = new Input();

		//create a shader
		ShaderPtr TextureShader = Graphics->CreateShader({
			L"game/shaders/TextureShader/TextureShader.svert",
			L"game/shaders/TextureShader/TextureShader.sfrag"
			});
		// create the textures
		TexturePtr TWood = Graphics->CreateTexture("game/textures/WoodTexture.jpg");
		TexturePtr TSquares = Graphics->CreateTexture("game/textures/GreySquare.jpg");

		// create the materials
		MaterialPtr MWood = make_shared<Material>();
		MaterialPtr MSquares = make_shared<Material>();

		// assign the base colour of the materials using the textures

		//MWood->BaseColour = TWood;
		MSquares->BaseColour = TSquares;
		// create a mesh
		Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, MSquares);
		Cube = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, MWood);
		
		Poly->Transform.Location = Vector3(0.0f, -1.0f, 1.0f);
		Cube->Transform.Location = Vector3(0.0f, -1.0f, 1.0f);
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
	// run the input detection for our game input
	GameInput->ProcessInput();
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

	//TODO: Handle logic
	Poly->Transform.Rotation.x += 50.0f * GetFDeltaTime();
	Poly->Transform.Rotation.y += 50.0f * GetFDeltaTime();
	Poly->Transform.Rotation.z += 50.0f * GetFDeltaTime(); 

	Cube->Transform.Rotation.x += -25.0f * GetFDeltaTime();
	Cube->Transform.Rotation.y += -25.0f * GetFDeltaTime();
	Cube->Transform.Rotation.z += -25.0f * GetFDeltaTime();

	Poly->Transform.Location.z -= 0.1f * GetFDeltaTime();
	Cube->Transform.Location.z -= 0.6f * GetFDeltaTime();
	
	Vector3 CameraInput = Vector3(0.0f);
	float MoveSpeed = 5.0f;
	CDirection CamDirections = Graphics->EngineDefaultCam->GetDirections();

	// move cam forward
	if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
		CameraInput += CamDirections.Forward;
		cout << "Movement | Moving Forward..." << endl;
	}
	// move cam backward
	if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
		CameraInput += -CamDirections.Forward;
		cout << "Movement | Moving Backward..." << endl;
	}
	// move cam right
	if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
		CameraInput += CamDirections.Right;
		cout << "Movement | Moving Right..." << endl;
	}
	// move cam left
	if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
		CameraInput += -CamDirections.Right;
		cout << "Movement | Moving Left..." << endl;
	}
	// move cam up
	if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
		CameraInput += CamDirections.Up;
		cout << "Movement | Moving Up..." << endl;
	 }
	// move cam down
	if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
		CameraInput += -CamDirections.Up;
		cout << "Movement | Moving Down..." << endl;
	}
	// multiply the move speed for running
	if (GameInput->IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		//TODO Add Running
		cout << "Movement | Running..." << endl;
	}
	// divide the move speed for crouching
	if (GameInput->IsKeyDown(SDL_SCANCODE_LCTRL)) {
		//TODO Add Crouching
		cout << "Movement | Crouching..." << endl;
	}
	// jump
	if (GameInput->IsKeyDown(SDL_SCANCODE_SPACE)) {
		CameraInput.y = 8.0f * MoveSpeed;
		cout << "Movement | Jumping..." << endl;
	}
	
	CameraInput *= MoveSpeed * GetFDeltaTime();

	// TODO: Create Gravity
	
	Graphics->EngineDefaultCam->AddMovementInput(CameraInput);

	// check right mouse button is held
	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta * GetFDeltaTime());
		Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta * GetFDeltaTime());
		GameInput->ShowCursor(false);
	}
	else {
		GameInput->ShowCursor(true);
	}
	// test inputs
	if (GameInput->IsMouseButtonDown(MouseButtons::LEFT)) {
		cout << "Movement | Left Mouse button down...";
	}

}

void Game::Draw()
{
	//TODO: Draw graphics to screen
	Graphics->Draw();
}

void Game::CloseGame()
{
	//TODO: Clean up code
	delete GameInput;
}
