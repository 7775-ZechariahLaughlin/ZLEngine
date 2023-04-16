#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Model.h"
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
MaterialPtr Game::GetDefaultEngineMaterial()
{
	return Graphics->DefaultEngineMaterial;
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
		TexturePtr TWall = Graphics->CreateTexture("game/textures/RockWall_BC.png");
		TexturePtr TLampBase = Graphics->CreateTexture("game/textures/StreetLamp/germany010.jpg");
		TexturePtr TLampSoil = Graphics->CreateTexture("game/textures/StreetLamp/ground020.jpg");
		TexturePtr TLampPost = Graphics->CreateTexture("game/textures/StreetLamp/Marmite.jpg");
		TexturePtr TGlass = Graphics->CreateTexture("game/textures/GlassBlue.png");
		TexturePtr TWarmth = Graphics->CreateTexture("game/textures/WarmYellow.png");
		TexturePtr TBarrel = Graphics->CreateTexture("game/textures/WoodBarrel/WBarrel_BC.png");
		TexturePtr TCoin = Graphics->CreateTexture("game/textures/GoldCoin/money2.png");
		

		// create the materials
		MaterialPtr MWood = make_shared<Material>();
		MaterialPtr MSquares = make_shared<Material>();
		MaterialPtr MWall = make_shared<Material>();
		MaterialPtr MLampBase= make_shared<Material>();
		MaterialPtr MLampSoil = make_shared<Material>();
		MaterialPtr MLampPost = make_shared<Material>();
		MaterialPtr MGlass = make_shared<Material>();
		MaterialPtr MWarmth = make_shared<Material>();
		MaterialPtr MBarrel = make_shared<Material>();
		MaterialPtr MCoin = make_shared<Material>();
	
		// assign the base colour of the materials using the textures
		MWood->BaseColour.TextureV3 = TWood;
		MSquares->BaseColour.TextureV3 = TSquares;
		MWall->BaseColour.TextureV3 = TWall;
		MLampBase->BaseColour.TextureV3 = TLampBase;
		MLampSoil->BaseColour.TextureV3 = TLampSoil;
		MLampPost->BaseColour.TextureV3 = TLampPost;
		MGlass->BaseColour.TextureV3 = TGlass;
		MWarmth->BaseColour.TextureV3 = TWarmth;
		MBarrel->BaseColour.TextureV3 = TBarrel;
		MCoin->BaseColour.TextureV3 = TCoin;

		// assign any emissive colours for lighting
		MWarmth->EmissiveColour.TextureV3 = TWarmth;
		MWarmth->EmissiveColour.MultiplierV3 =  Vector3(0.5f);

		// create a mesh
		Model = Graphics->ImportModel("game/models/primitives/Cube.fbx", TextureShader);
		Model2 = Graphics->ImportModel("game/models/primitives/Sphere.fbx", TextureShader);
		Floor = Graphics->ImportModel("game/models/primitives/Plane.fbx", TextureShader);
		Lamp1 = Graphics->ImportModel("game/models/Street_Lamp.obj", TextureShader);
		Coin1 = Graphics->ImportModel("game/models/Gold_Coin.obj", TextureShader);
		Barrel1 = Graphics->ImportModel("game/models/Wood_Barrel.obj", TextureShader);

		// handle wall creation, there will be lots so a loop will be easier to create textures, assign materials and scale them (walls will be the same size unless necessary to change)
		for (zluint i = 0; i < 35; i++) {
			WallStack[i] = Graphics->ImportModel("game/models/Stone_Wall.obj", TextureShader);
			WallStack[i]->SetMaterialBySlot(1, MWall);
			WallStack[i]->Transform.Scale = Vector3(0.01f);
		}

		// set materials of the models
		// misc models
		Model->SetMaterialBySlot(0, MWood);
		Model2->SetMaterialBySlot(0, MWarmth);
		Lamp1->SetMaterialBySlot(1, MLampBase);
		Lamp1->SetMaterialBySlot(2, MLampSoil);
		Lamp1->SetMaterialBySlot(3, MLampPost);
		Lamp1->SetMaterialBySlot(4, MLampPost);
		Coin1->SetMaterialBySlot(1, MWarmth);
		Barrel1->SetMaterialBySlot(1, MBarrel);

		// floor
		Floor->SetMaterialBySlot(0, MSquares);
		
		// transform the models location 
		// transform misc models
		Model->Transform.Location = Vector3(1.0f, -1.0f, 1.0f);
		Model2->Transform.Location = Vector3(10.8f, 4.2f, 10.8f);
		Lamp1->Transform.Location = Vector3(10.8f, 0.0f, 10.8f);
		Coin1->Transform.Location = Vector3(10.0f, 0.8f, 10.0f);
		Barrel1->Transform.Location = Vector3(3.0f, 0.0f, -3.0f);
		
		// transform walls
		WallStack[0]->Transform.Location = Vector3(12.0f, 0.0f, 10.6f);
		WallStack[1]->Transform.Location = Vector3(12.0f, 0.0f, 7.8f);
		WallStack[2]->Transform.Location = Vector3(12.0f, 0.0f, 5.0f);
		WallStack[3]->Transform.Location = Vector3(12.0f, 0.0f, 2.2f);

		WallStack[4]->Transform.Location = Vector3(10.6f, 0.0f, 12.0f);
		WallStack[5]->Transform.Location = Vector3(7.8f, 0.0f, 12.0f);
		WallStack[6]->Transform.Location = Vector3(5.0f, 0.0f, 12.0f);
		WallStack[7]->Transform.Location = Vector3(2.2f, 0.0f, 12.0f);

		WallStack[8]->Transform.Location = Vector3(10.6f, 0.0f, 0.8f);
		WallStack[9]->Transform.Location = Vector3(7.8f, 0.0f, 0.8f);

		WallStack[10]->Transform.Location = Vector3(0.8f, 0.0f, 10.6f);
		WallStack[11]->Transform.Location = Vector3(0.8f, 0.0f, 7.8f);

		// rotate walls
		WallStack[4]->Transform.Rotation.y += 90.0f;
		WallStack[5]->Transform.Rotation.y += 90.0f;
		WallStack[6]->Transform.Rotation.y += 90.0f;
		WallStack[7]->Transform.Rotation.y += 90.0f;
		WallStack[8]->Transform.Rotation.y += 90.0f;
		WallStack[9]->Transform.Rotation.y += 90.0f;

		//scale the floor
		Floor->Transform.Scale = Vector3(24.0f);
		Lamp1->Transform.Scale = Vector3(0.4f);
		Model2->Transform.Scale = Vector3(0.33f);
		Barrel1->Transform.Scale = Vector3(0.15f);
		
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
	if (Barrel1->Transform.Location.x >= 0.0f && Barrel1->Transform.Location.x < 10.0f) {
		Barrel1->Transform.Location.x += 1.0f * GetFDeltaTime();
	}
	else if (Barrel1->Transform.Location.x >= 10.0f && Barrel1->Transform.Location.z > -10.0f) {
		Barrel1->Transform.Location.z -= 1.0f * GetFDeltaTime();
	}
	else if (Barrel1->Transform.Location.z <= -10.0f) {
		Barrel1->Transform.Location.y += 1.5f * GetFDeltaTime();
	}

	Model->Transform.Rotation.x += 50.0f * GetFDeltaTime();
	Model->Transform.Rotation.y += 50.0f * GetFDeltaTime();
	Model->Transform.Rotation.z += 50.0f * GetFDeltaTime();
	Lamp1->Transform.Rotation.y += 50.0f * GetFDeltaTime();
	Barrel1->Transform.Rotation.y += 100.0f * GetFDeltaTime();


	Vector3 CameraInput = Vector3(0.0f);
	float MoveSpeed = 5.0f;
	Graphics->EngineDefaultCam->SetCameraSpeed(5.0f);
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
		Graphics->EngineDefaultCam->SetCameraSpeed(10.0f);
		cout << "Movement | Running..." << endl;
	}
	// divide the move speed for crouching
	if (GameInput->IsKeyDown(SDL_SCANCODE_LCTRL)) {
		//TODO Add Crouching
		cout << "Movement | Crouching..." << endl;
		Graphics->EngineDefaultCam->SetCameraSpeed(2.75f);
	}
	// jump
	if (GameInput->IsKeyDown(SDL_SCANCODE_SPACE)) {
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
