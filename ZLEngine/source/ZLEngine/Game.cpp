#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Model.h"
#include "ZLEngine/Input.h"
#include "ZLEngine/Graphics/Camera.h"
#include "ZLEngine/Graphics/Material.h"
#include "ZLEngine/Collisions/Collision.h"
#include "ZLEngine/GameObjects/GameObject.h"
#include "ZLEngine/GameObjects/Obstacle.h"
#include "ZLEngine/GameObjects/Pickup.h"
#include "ZLEngine/GameObjects/Player.h"
#include "ZLEngine/GameObjects/Gate.h"

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
	Pickups = make_shared<Pickup>();

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
void Game::RemoveModelFromGame(ModelPtr ModelToRemove)
{
	// remove from the graphics engine
	Graphics->RemoveModel(ModelToRemove);

	// change the reference to nullptr and remove from the game
	ModelToRemove == nullptr;
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
		TexturePtr TConcrete = Graphics->CreateTexture("game/textures/ConcreteWall/Old_Cement_Wall.jpg");
		TexturePtr TWall = Graphics->CreateTexture("game/textures/ConcreteWall/Stone_Wall_BC.jpg");
		TexturePtr TLampBase = Graphics->CreateTexture("game/textures/StreetLamp/germany010.jpg");
		TexturePtr TLampSoil = Graphics->CreateTexture("game/textures/StreetLamp/ground020.jpg");
		TexturePtr TLampPost = Graphics->CreateTexture("game/textures/StreetLamp/Marmite.jpg");
		TexturePtr TGlass = Graphics->CreateTexture("game/textures/DefaultTextures/GlassBlue.png");
		TexturePtr TWarmth = Graphics->CreateTexture("game/textures/DefaultTextures/WarmYellow.png");
		TexturePtr TBarrel = Graphics->CreateTexture("game/textures/WoodBarrel/WBarrel_BC.png");
		TexturePtr TCoin = Graphics->CreateTexture("game/textures/GoldCoin/Gold_Coin.png");
		TexturePtr TArchway = Graphics->CreateTexture("game/textures/Archway/ArchwayLow_BC.jpg");
		TexturePtr TBox = Graphics->CreateTexture("game/textures/WoodBox/WoodBox_BC.png");
		TexturePtr TSkull = Graphics->CreateTexture("game/textures/SkullPickup_BC.png");
		
		// create the materials
		MaterialPtr MWall = make_shared<Material>();
		MaterialPtr MLampBase= make_shared<Material>();
		MaterialPtr MLampSoil = make_shared<Material>();
		MaterialPtr MLampPost = make_shared<Material>();
		MaterialPtr MGlass = make_shared<Material>();
		MaterialPtr MWarmth = make_shared<Material>();
		MaterialPtr MBarrel = make_shared<Material>();
		MaterialPtr MCoin = make_shared<Material>();
		MaterialPtr MConcrete = make_shared<Material>();
		MaterialPtr MArchway = make_shared<Material>();
		MaterialPtr MBox = make_shared<Material>();
		MaterialPtr MSkull = make_shared<Material>();
	
		// assign the base colour of the materials using the textures
		MWall->BaseColour.TextureV3 = TWall;
		MLampBase->BaseColour.TextureV3 = TLampBase;
		MLampSoil->BaseColour.TextureV3 = TLampSoil;
		MLampPost->BaseColour.TextureV3 = TLampPost;
		MGlass->BaseColour.TextureV3 = TGlass;
		MWarmth->BaseColour.TextureV3 = TWarmth;
		MBarrel->BaseColour.TextureV3 = TBarrel;
		MCoin->BaseColour.TextureV3 = TCoin;
		MConcrete->BaseColour.TextureV3 = TConcrete;
		MArchway->BaseColour.TextureV3 = TArchway;
		MBox->BaseColour.TextureV3 = TBox;
		MSkull->BaseColour.TextureV3 = TSkull;

		// assign any emissive colours for lighting
		MWarmth->EmissiveColour.TextureV3 = TWarmth;
		MWarmth->EmissiveColour.MultiplierV3 =  Vector3(0.5f);

		// create a mesh
		Archway1 = Graphics->ImportModel("game/models/Archway.obj", TextureShader);
		Skull1 = Graphics->ImportModel("game/models/Skull_Pickup.fbx", TextureShader);

		//TODO: Tidy up and move into a SpawnHandler which will run as a single function in the main
		// handle wall creation, there will be lots so a loop will be easier to create textures, assign materials and scale them (walls will be the same size unless necessary to change)
		for (zluint i = 0; i < 118; i++) {
			WallStack[i] = Graphics->ImportModel("game/models/Concrete_Wall.obj", TextureShader);
			WallStack[i]->SetMaterialBySlot(1, MWall);
			WallStack[i]->Transform.Scale = Vector3(0.29f, 0.45f, 0.38f);
		}

		// handle floor creation
		for (zluint i = 0; i < 10; i++) {
			FloorStack[i] = Graphics->ImportModel("game/models/primitives/Plane.fbx", TextureShader);
			FloorStack[i]->SetMaterialBySlot(0, MConcrete);
		}

		// handle coin pickup creation
		for (zluint i = 0; i < 11; i++) {
			CoinPickup[i] = make_shared<Pickup>(Graphics->ImportModel("game/models/Gold_Coin.obj", TextureShader), 50);
			CoinPickup[i]->ReturnPickupModel()->SetMaterialBySlot(1, MCoin);
			CoinPickup[i]->ReturnPickupModel()->Transform.Scale = Vector3(0.33f);
		}

		// handle box obstacle creation
		for (zluint i = 0; i < 16; i++) {
			BoxObstacle[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/Wood_Box.obj", TextureShader));
			BoxObstacle[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.08f);
			BoxObstacle[i]->ReturnObstacleModel()->SetMaterialBySlot(1, MBox);
		}
		// handle box obstacle creation
		for (zluint i = 0; i < 10; i++) {
			BarrelObstacle[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/Wood_Barrel.obj", TextureShader));
			BarrelObstacle[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.15f);
			BarrelObstacle[i]->ReturnObstacleModel()->SetMaterialBySlot(1, MBarrel);
		}
		// handle bulb creation
		for (zluint i = 0; i < 10; i++) {
			Bulbs[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/primitives/Sphere.fbx", TextureShader));
			Bulbs[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.38f);
			Bulbs[i]->ReturnObstacleModel()->SetMaterialBySlot(0, MWarmth);

		}
		// handle lamp creation
		for (zluint i = 0; i < 10; i++) {
			Lamps[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/Street_Lamp.obj", TextureShader));
			Lamps[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.45f);
			Lamps[i]->ReturnObstacleModel()->SetMaterialBySlot(1, MLampBase);
			Lamps[i]->ReturnObstacleModel()->SetMaterialBySlot(2, MLampSoil);
			Lamps[i]->ReturnObstacleModel()->SetMaterialBySlot(3, MLampPost);
			Lamps[i]->ReturnObstacleModel()->SetMaterialBySlot(4, MLampPost);
		}



		// set materials of the models
		// misc models
		Archway1->SetMaterialBySlot(0, MArchway);
		Skull1->SetMaterialBySlot(1, MSkull);
		
		// transform the models location 
		// transform misc models
		Bulbs[0]->ReturnObstacleModel()->Transform.Location = Vector3(10.0f, 5.2f, 10.0f);
		Bulbs[1]->ReturnObstacleModel()->Transform.Location = Vector3(-9.0f, 5.2f, 10.8f);
		Bulbs[2]->ReturnObstacleModel()->Transform.Location = Vector3(-10.0f, 5.2f, -10.0f);
		Bulbs[3]->ReturnObstacleModel()->Transform.Location = Vector3(-44.5f, 5.2f, -12.0f);
		Bulbs[4]->ReturnObstacleModel()->Transform.Location = Vector3(-1.0f, 5.2f, -48.0f);
		Bulbs[5]->ReturnObstacleModel()->Transform.Location = Vector3(-38.5f, 5.2f, -50.0f);
		Bulbs[6]->ReturnObstacleModel()->Transform.Location = Vector3(-94.0f, 5.2f, 2.5f);
		Bulbs[7]->ReturnObstacleModel()->Transform.Location = Vector3(-71.5f, 5.2f, -29.0f);
		Bulbs[8]->ReturnObstacleModel()->Transform.Location = Vector3(-102.0f, 5.2f, -53.0f);
		Bulbs[9]->ReturnObstacleModel()->Transform.Location = Vector3(-65.0f, 5.2f, -65.0f);

		Lamps[0]->ReturnObstacleModel()->Transform.Location = Vector3(10.0f, 0.5f, 10.0f);
		Lamps[1]->ReturnObstacleModel()->Transform.Location = Vector3(-9.0f, 0.5f, 10.8f);
		Lamps[2]->ReturnObstacleModel()->Transform.Location = Vector3(-10.0f, 0.5f, -10.0f);
		Lamps[3]->ReturnObstacleModel()->Transform.Location = Vector3(-44.5f, 0.5f, -12.0f);
		Lamps[4]->ReturnObstacleModel()->Transform.Location = Vector3(-1.0f, 0.5f, -48.0f);
		Lamps[5]->ReturnObstacleModel()->Transform.Location = Vector3(-38.5f, 0.5f, -50.0f);
		Lamps[6]->ReturnObstacleModel()->Transform.Location = Vector3(-94.0f, 0.5f, 2.5f);
		Lamps[7]->ReturnObstacleModel()->Transform.Location = Vector3(-71.5f, 0.5f, -29.0f);
		Lamps[8]->ReturnObstacleModel()->Transform.Location = Vector3(-102.0f, 0.5f, -53.0f);
		Lamps[9]->ReturnObstacleModel()->Transform.Location = Vector3(-65.0f, 0.5f, -65.0f);

		Archway1->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
		Skull1->Transform.Location = Vector3(7.0f);

		// transform box obstacles
		BoxObstacle[0]->ReturnObstacleModel()->Transform.Location = Vector3(3.4f, 1.7f, 1.2f);
		BoxObstacle[1]->ReturnObstacleModel()->Transform.Location = Vector3(3.4f, 1.7f, -1.2f);
		BoxObstacle[2]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -3.6f);
		BoxObstacle[3]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -6.0f);
		BoxObstacle[4]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -8.4f);
		BoxObstacle[5]->ReturnObstacleModel()->Transform.Location = Vector3(3.0f, 1.7f, -13.8f);

		// transform barrel obstacles
		BarrelObstacle[0]->ReturnObstacleModel()->Transform.Location = Vector3(3.5f, 0.5f, -3.4f);
		BarrelObstacle[1]->ReturnObstacleModel()->Transform.Location = Vector3(4.8f, 0.5f, -10.5f);
		BarrelObstacle[2]->ReturnObstacleModel()->Transform.Location = Vector3(4.8f, 0.5f, -12.1f);

		// transform pickups
		CoinPickup[0]->ReturnPickupModel()->Transform.Location = Vector3(-9.0f, 0.5f, 9.0f);
		CoinPickup[1]->ReturnPickupModel()->Transform.Location = Vector3(10.0f, 0.5f, -11.5f);
		CoinPickup[2]->ReturnPickupModel()->Transform.Location = Vector3(8.5f, 0.5f, -12.5f);
		CoinPickup[3]->ReturnPickupModel()->Transform.Location = Vector3(8.5f, 0.5f, -10.5f);
		CoinPickup[4]->ReturnPickupModel()->Transform.Location = Vector3(-6.0f, 0.5f, -16.0f);
		CoinPickup[5]->ReturnPickupModel()->Transform.Location = Vector3(-13.0f, 0.5f, -6.0f);
		CoinPickup[6]->ReturnPickupModel()->Transform.Location = Vector3(8.5f, 0.5f, -10.5f);
		CoinPickup[7]->ReturnPickupModel()->Transform.Location = Vector3(-13.0f, 0.5f, -6.0f);
		
		// transform floors
		FloorStack[0]->Transform.Location = Vector3(0.0f, 0.5f, -1.5f);
		FloorStack[1]->Transform.Location = Vector3(-6.0f, 0.5f, -32.0f);
		FloorStack[2]->Transform.Location = Vector3(-30.0f, 0.5f, -6.0f);
		FloorStack[3]->Transform.Location = Vector3(-20.0f, 0.5f, -52.0f);
		FloorStack[4]->Transform.Location = Vector3(-34.0f, 0.5f, -33.0f);
		FloorStack[5]->Transform.Location = Vector3(-72.0f, 0.5f, -12.0f);
		FloorStack[6]->Transform.Location = Vector3(-101.0f, 0.5f, -24.5f);
		FloorStack[7]->Transform.Location = Vector3(-97.5f, 0.5f, -43.5f);
		FloorStack[8]->Transform.Location = Vector3(-76.0f, 0.5f, -55.6f);
		
		// transform walls
		// room one north walls
		WallStack[0]->Transform.Location = Vector3(12.0f, 0.0f, 9.8f);
		WallStack[1]->Transform.Location = Vector3(12.0f, 0.0f, 4.2f);
		WallStack[2]->Transform.Location = Vector3(12.0f, 0.0f, -1.4f);
		WallStack[3]->Transform.Location = Vector3(12.0f, 0.0f, -7.0f);
		WallStack[4]->Transform.Location = Vector3(12.0f, 0.0f, -12.6f);

		// room one east walls
		WallStack[5]->Transform.Location = Vector3(8.7f, 0.0f, 12.3f);
		WallStack[6]->Transform.Location = Vector3(3.1f, 0.0f, 12.3f);
		WallStack[7]->Transform.Location = Vector3(-2.1f, 0.0f, 12.3f);
		WallStack[8]->Transform.Location = Vector3(-7.7f, 0.0f, 12.3f);

		// room one dividers
		WallStack[9]->Transform.Location = Vector3(9.0f, 0.0f, 2.8f);
		WallStack[10]->Transform.Location = Vector3(3.2f, 0.0f, 2.8f);
		WallStack[26]->Transform.Location = Vector3(-2.6f, 0.0f, 2.8f);

		// room one south walls
		WallStack[11]->Transform.Location = Vector3(-11.0f, 0.0f, 9.8f);
		WallStack[12]->Transform.Location = Vector3(-11.0f, 0.0f, 4.2f);

		// room one west walls
		WallStack[13]->Transform.Location = Vector3(9.2f, 0.0f, -15.4f);
		WallStack[14]->Transform.Location = Vector3(3.4f, 0.0f, -15.4f);

		// hallway one north walls
		WallStack[15]->Transform.Location = Vector3(0.2f, 0.0f, -17.9f);
		WallStack[16]->Transform.Location = Vector3(0.2f, 0.0f, -23.7f);
		WallStack[17]->Transform.Location = Vector3(0.2f, 0.0f, -28.5f);
		WallStack[18]->Transform.Location = Vector3(0.2f, 0.0f, -34.3f);
		WallStack[19]->Transform.Location = Vector3(0.2f, 0.0f, -40.1f);
		WallStack[20]->Transform.Location = Vector3(0.2f, 0.0f, -45.9f);

		// hallway one south walls
		WallStack[21]->Transform.Location = Vector3(-11.8f, 0.0f, -14.9f);
		WallStack[22]->Transform.Location = Vector3(-11.8f, 0.0f, -20.7f);
		WallStack[23]->Transform.Location = Vector3(-11.8f, 0.0f, -25.5f);
		WallStack[24]->Transform.Location = Vector3(-11.8f, 0.0f, -31.3f);
		WallStack[25]->Transform.Location = Vector3(-11.8f, 0.0f, -37.1f);
		
		// room two east walls
		WallStack[27]->Transform.Location = Vector3(-15.0f, 0.0f, -39.6f);
		WallStack[28]->Transform.Location = Vector3(-20.8f, 0.0f, -39.6f);
		WallStack[29]->Transform.Location = Vector3(-26.6f, 0.0f, -39.6f);

		// hallway one east walls
		WallStack[30]->Transform.Location = Vector3(-2.3f, 0.0f, -49.2f);
		WallStack[31]->Transform.Location = Vector3(-8.1f, 0.0f, -49.2f);
		
		// hallway two east walls
		WallStack[32]->Transform.Location = Vector3(-13.5f, 0.0f, 0.9f);
		WallStack[33]->Transform.Location = Vector3(-19.2f, 0.0f, 0.9f);
		WallStack[34]->Transform.Location = Vector3(-25.0f, 0.0f, 0.9f);
		WallStack[35]->Transform.Location = Vector3(-30.8f, 0.0f, 0.9f);
		WallStack[36]->Transform.Location = Vector3(-36.6f, 0.0f, 0.9f);
		WallStack[37]->Transform.Location = Vector3(-42.4f, 0.0f, 0.9f);

		// hallway two west walls
		WallStack[38]->Transform.Location = Vector3(-14.3f, 0.0f, -12.4f);
		WallStack[39]->Transform.Location = Vector3(-20.0f, 0.0f, -12.4f);
		WallStack[40]->Transform.Location = Vector3(-25.8f, 0.0f, -12.4f);
		WallStack[41]->Transform.Location = Vector3(-43.2f, 0.0f, -13.0f);

		// room two north walls
		WallStack[42]->Transform.Location = Vector3(-11.4f, 0.0f, -51.7f);
		WallStack[43]->Transform.Location = Vector3(-11.4f, 0.0f, -57.5f);

		// room two west walls
		WallStack[44]->Transform.Location = Vector3(-14.7f, 0.0f, -60.7f);
		WallStack[45]->Transform.Location = Vector3(-20.5f, 0.0f, -60.7f);
		WallStack[46]->Transform.Location = Vector3(-26.3f, 0.0f, -60.7f);

		// room two south walls
		WallStack[47]->Transform.Location = Vector3(-28.7f, 0.0f, -57.4f);
		
		// hallway three west walls
		WallStack[48]->Transform.Location = Vector3(-31.2f, 0.0f, -54.1f);
		WallStack[49]->Transform.Location = Vector3(-37.0f, 0.0f, -54.1f);

		// hallway three north walls
		WallStack[50]->Transform.Location = Vector3(-29.2f, 0.0f, -37.0f);
		WallStack[51]->Transform.Location = Vector3(-29.2f, 0.0f, -31.2f);
		WallStack[52]->Transform.Location = Vector3(-29.2f, 0.0f, -25.4f);
		WallStack[53]->Transform.Location = Vector3(-29.2f, 0.0f, -19.6f);
		WallStack[54]->Transform.Location = Vector3(-29.2f, 0.0f, -13.8f);

		// hallway three south walls
		WallStack[55]->Transform.Location = Vector3(-40.0f, 0.0f, -50.8f);
		WallStack[56]->Transform.Location = Vector3(-40.0f, 0.0f, -45.0f);
		WallStack[57]->Transform.Location = Vector3(-40.0f, 0.0f, -39.2f);
		WallStack[58]->Transform.Location = Vector3(-40.0f, 0.0f, -33.4f);
		WallStack[59]->Transform.Location = Vector3(-40.0f, 0.0f, -27.6f);
		WallStack[60]->Transform.Location = Vector3(-40.0f, 0.0f, -21.8f);
		WallStack[61]->Transform.Location = Vector3(-40.0f, 0.0f, -16.0f);

		// room three north walls
		WallStack[62]->Transform.Location = Vector3(-45.7f, 0.0f, 3.5f);
		WallStack[63]->Transform.Location = Vector3(-46.1f, 0.0f, -15.5f);
		WallStack[64]->Transform.Location = Vector3(-46.1f, 0.0f, -21.3f);
		WallStack[65]->Transform.Location = Vector3(-46.1f, 0.0f, -27.2f);
		
		// room three west walls
		WallStack[66]->Transform.Location = Vector3(-48.6f, 0.0f, -30.2f);
		WallStack[67]->Transform.Location = Vector3(-54.4f, 0.0f, -30.2f);
		WallStack[68]->Transform.Location = Vector3(-60.2f, 0.0f, -30.2f);
		WallStack[69]->Transform.Location = Vector3(-66.0f, 0.0f, -30.2f);
		WallStack[70]->Transform.Location = Vector3(-71.8f, 0.0f, -30.2f);
		WallStack[71]->Transform.Location = Vector3(-77.6f, 0.0f, -30.2f);
		WallStack[72]->Transform.Location = Vector3(-83.4f, 0.0f, -30.2f);
		WallStack[73]->Transform.Location = Vector3(-89.2f, 0.0f, -30.2f);
		WallStack[74]->Transform.Location = Vector3(-95.0f, 0.0f, -30.2f);

		// room three east walls
		WallStack[75]->Transform.Location = Vector3(-48.6f, 0.0f, 6.4f);
		WallStack[76]->Transform.Location = Vector3(-54.4f, 0.0f, 6.4f);
		WallStack[77]->Transform.Location = Vector3(-60.2f, 0.0f, 6.4f);
		WallStack[78]->Transform.Location = Vector3(-66.0f, 0.0f, 6.4f);
		WallStack[79]->Transform.Location = Vector3(-71.8f, 0.0f, 6.4f);
		WallStack[80]->Transform.Location = Vector3(-77.6f, 0.0f, 6.4f);
		WallStack[81]->Transform.Location = Vector3(-83.4f, 0.0f, 6.4f);
		WallStack[82]->Transform.Location = Vector3(-89.2f, 0.0f, 6.4f);
		WallStack[83]->Transform.Location = Vector3(-95.0f, 0.0f, 6.4f);
		
		// room three south walls
		WallStack[84]->Transform.Location = Vector3(-97.9f, 0.0f, 3.8f);
		WallStack[85]->Transform.Location = Vector3(-97.9f, 0.0f, -2.0f);
		WallStack[86]->Transform.Location = Vector3(-97.9f, 0.0f, -7.8f);
		WallStack[87]->Transform.Location = Vector3(-97.9f, 0.0f, -13.6f);

		// hallway four east walls
		WallStack[88]->Transform.Location = Vector3(-100.8f, 0.0f, -16.5f);

		// hallway four south walls
		WallStack[89]->Transform.Location = Vector3(-103.7f, 0.0f, -19.4f);
		WallStack[90]->Transform.Location = Vector3(-103.7f, 0.0f, -25.2f);
		WallStack[91]->Transform.Location = Vector3(-103.7f, 0.0f, -31.0f);
		WallStack[92]->Transform.Location = Vector3(-103.7f, 0.0f, -36.8f);
		WallStack[93]->Transform.Location = Vector3(-103.7f, 0.0f, -42.6f);
		WallStack[94]->Transform.Location = Vector3(-103.7f, 0.0f, -48.4f);
		WallStack[95]->Transform.Location = Vector3(-103.7f, 0.0f, -54.2f);

		// hallway four west walls
		WallStack[96]->Transform.Location = Vector3(-100.4f, 0.0f, -55.8f);
		WallStack[97]->Transform.Location = Vector3(-94.6f, 0.0f, -55.8f);

		// hallway four east walls
		WallStack[98]->Transform.Location = Vector3(-95.0f, 0.0f, -31.2f);

		// hallway four north walls
		WallStack[99]->Transform.Location = Vector3(-92.0f, 0.0f, -34.1f);
		WallStack[100]->Transform.Location = Vector3(-92.0f, 0.0f, -39.9f);

		// room four south walls
		WallStack[101]->Transform.Location = Vector3(-91.4f, 0.0f, -58.4f);
		WallStack[102]->Transform.Location = Vector3(-91.4f, 0.0f, -64.2f);

		// room four east walls
		WallStack[103]->Transform.Location = Vector3(-89.2f, 0.0f, -43.2f);
		WallStack[104]->Transform.Location = Vector3(-83.4f, 0.0f, -43.2f);
		WallStack[105]->Transform.Location = Vector3(-77.6f, 0.0f, -43.2f);
		WallStack[106]->Transform.Location = Vector3(-71.8f, 0.0f, -43.2f);
		WallStack[107]->Transform.Location = Vector3(-66.0f, 0.0f, -43.2f);

		// room four west walls
		WallStack[108]->Transform.Location = Vector3(-89.2f, 0.0f, -67.1f);
		WallStack[109]->Transform.Location = Vector3(-83.4f, 0.0f, -67.1f);
		WallStack[110]->Transform.Location = Vector3(-77.6f, 0.0f, -67.1f);
		WallStack[111]->Transform.Location = Vector3(-71.8f, 0.0f, -67.1f);
		WallStack[112]->Transform.Location = Vector3(-66.0f, 0.0f, -67.1f);

		// room four north walls
		WallStack[113]->Transform.Location = Vector3(-63.1f, 0.0f, -67.1f);
		WallStack[114]->Transform.Location = Vector3(-63.1f, 0.0f, -61.3f);
		WallStack[115]->Transform.Location = Vector3(-63.1f, 0.0f, -55.5f);
		WallStack[116]->Transform.Location = Vector3(-63.1f, 0.0f, -49.7f);
		WallStack[117]->Transform.Location = Vector3(-63.1f, 0.0f, -43.9f);
		
		// rotate walls
		WallStack[0]->Transform.Rotation.y += 90.0f;
		WallStack[1]->Transform.Rotation.y += 90.0f;
		WallStack[2]->Transform.Rotation.y += 90.0f;
		WallStack[3]->Transform.Rotation.y += 90.0f;
		WallStack[4]->Transform.Rotation.y += 90.0f;
		WallStack[7]->Transform.Rotation.y += 180.0f;
		WallStack[8]->Transform.Rotation.y += 180.0f;
		WallStack[11]->Transform.Rotation.y += 270.0f;
		WallStack[12]->Transform.Rotation.y += 270.0f;
		WallStack[15]->Transform.Rotation.y += 270.0f;
		WallStack[16]->Transform.Rotation.y += 270.0f;
		WallStack[17]->Transform.Rotation.y += 270.0f;
		WallStack[18]->Transform.Rotation.y += 270.0f;
		WallStack[19]->Transform.Rotation.y += 270.0f;
		WallStack[20]->Transform.Rotation.y += 270.0f;
		WallStack[21]->Transform.Rotation.y += 270.0f;
		WallStack[22]->Transform.Rotation.y += 270.0f;
		WallStack[23]->Transform.Rotation.y += 270.0f;
		WallStack[24]->Transform.Rotation.y += 270.0f;
		WallStack[25]->Transform.Rotation.y += 270.0f;
		WallStack[42]->Transform.Rotation.y += 90.0f;
		WallStack[43]->Transform.Rotation.y += 90.0f;
		WallStack[47]->Transform.Rotation.y += 90.0f;
		WallStack[50]->Transform.Rotation.y += 90.0f;
		WallStack[51]->Transform.Rotation.y += 90.0f;
		WallStack[52]->Transform.Rotation.y += 90.0f;
		WallStack[53]->Transform.Rotation.y += 90.0f;
		WallStack[54]->Transform.Rotation.y += 90.0f;
		WallStack[55]->Transform.Rotation.y += 90.0f;
		WallStack[56]->Transform.Rotation.y += 90.0f;
		WallStack[57]->Transform.Rotation.y += 90.0f;
		WallStack[58]->Transform.Rotation.y += 90.0f;
		WallStack[59]->Transform.Rotation.y += 90.0f;
		WallStack[60]->Transform.Rotation.y += 90.0f;
		WallStack[61]->Transform.Rotation.y += 90.0f;
		WallStack[62]->Transform.Rotation.y += 90.0f;
		WallStack[63]->Transform.Rotation.y += 90.0f;
		WallStack[64]->Transform.Rotation.y += 90.0f;
		WallStack[65]->Transform.Rotation.y += 90.0f;
		WallStack[84]->Transform.Rotation.y += 90.0f;
		WallStack[85]->Transform.Rotation.y += 90.0f;
		WallStack[86]->Transform.Rotation.y += 90.0f;
		WallStack[87]->Transform.Rotation.y += 90.0f;
		WallStack[89]->Transform.Rotation.y += 90.0f;
		WallStack[90]->Transform.Rotation.y += 90.0f;
		WallStack[91]->Transform.Rotation.y += 90.0f;
		WallStack[92]->Transform.Rotation.y += 90.0f;
		WallStack[93]->Transform.Rotation.y += 90.0f;
		WallStack[94]->Transform.Rotation.y += 90.0f;
		WallStack[95]->Transform.Rotation.y += 90.0f;
		WallStack[99]->Transform.Rotation.y += 90.0f;
		WallStack[100]->Transform.Rotation.y += 90.0f;
		WallStack[101]->Transform.Rotation.y += 90.0f;
		WallStack[102]->Transform.Rotation.y += 90.0f;
		WallStack[113]->Transform.Rotation.y += 90.0f;
		WallStack[114]->Transform.Rotation.y += 90.0f;
		WallStack[115]->Transform.Rotation.y += 90.0f;
		WallStack[116]->Transform.Rotation.y += 90.0f;
		WallStack[117]->Transform.Rotation.y += 90.0f;
		

		//scale the floors
		FloorStack[0]->Transform.Scale = Vector3(24.0f, 1.0f, 27.0f);
		FloorStack[1]->Transform.Scale = Vector3(12.0f, 1.0f, 36.0f);
		FloorStack[2]->Transform.Scale = Vector3(36.0f, 1.0f, 14.0f);
		FloorStack[3]->Transform.Scale = Vector3(17.0f, 1.0f, 24.0f);
		FloorStack[4]->Transform.Scale = Vector3(12.0f, 1.0f, 42.0f);
		FloorStack[5]->Transform.Scale = Vector3(52.0f, 1.0f, 36.0f);
		FloorStack[6]->Transform.Scale = Vector3(6.5f, 1.0f, 15.0f);
		FloorStack[7]->Transform.Scale = Vector3(14.0f, 1.0f, 24.0f);
		FloorStack[8]->Transform.Scale = Vector3(30.0f, 1.0f, 24.0f);
		
		Archway1->Transform.Scale = Vector3(0.6f);

		// add collisions to models
		Archway1->AddCollisionToModel(Vector3(6.0f, 7.0f, 1.5f), Vector3(-0.5f, -3.0f, 9.75f));
		FloorStack[0]->AddCollisionToModel(Vector3(24.0f, 1.0f, 27.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[1]->AddCollisionToModel(Vector3(12.0f, 1.0f, 36.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[2]->AddCollisionToModel(Vector3(36.0f, 1.0f, 12.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[3]->AddCollisionToModel(Vector3(17.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[4]->AddCollisionToModel(Vector3(12.0f, 1.0f, 42.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[5]->AddCollisionToModel(Vector3(52.0f, 1.0f, 36.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[6]->AddCollisionToModel(Vector3(6.5f, 1.0f, 15.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[7]->AddCollisionToModel(Vector3(14.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[8]->AddCollisionToModel(Vector3(30.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));

	

		// add collisions to coins, this is done after transformations and scaling as it needs data from both
		for (zluint i = 0; i < 11; i++) {
			CoinPickup[i]->ReturnPickupModel()->AddCollisionToModel(Vector3(0.8f, 0.8f, 0.8f), Vector3(0.0f, 0.7f, 0.0f));
		}
		// add collisions to box obstacles
		for (zluint i = 0; i < 16; i++) {
			BoxObstacle[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(2.4f, 2.4f, 2.4f), Vector3(0.0f));
		}
		// add collisions to the lamps
		for (int i = 0; i < 10; i++) {
			Lamps[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.5f, 5.4f, 1.5f), Vector3(0.0f, 2.7f, 0.0f));
		}

		
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

	Vector3 CameraInput = Vector3(0.0f);
	float MoveSpeed = 5.0f;
	float MaxJumpHeight = 8.0f;
	Graphics->EngineDefaultCam->SetCameraSpeed(5.0f);
	CDirection CamDirections = Graphics->EngineDefaultCam->GetDirections();

	if (!OnFloor) {
		CameraInput += -CamDirections.Up;
	}
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
		if (!OnFloor) {
			CameraInput += -CamDirections.Up;
			cout << "Movement | Moving Down..." << endl;
		}
		else {
			cout << "Movement | On the floor!" << endl;
		}
	}
	// multiply the move speed for running
	if (GameInput->IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		//TODO Add Running
		Graphics->EngineDefaultCam->SetCameraSpeed(30.0f);
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
		if (!Jumping) {
			for (int i = 0; i <= 6; i++) {
				Graphics->EngineDefaultCam->SetCameraSpeed(30.0f);
				Graphics->EngineDefaultCam->AddMovementInput(CamDirections.Up);
			}
			Jumping = true;
			OnFloor = false;
		}
	}

	CameraInput *= MoveSpeed * GetFDeltaTime();

	Graphics->EngineDefaultCam->AddMovementInput(CameraInput);

	// check right mouse button is held
	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta);
		Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta);
		GameInput->ShowCursor(false);
	}
	else {
		GameInput->ShowCursor(true);
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

	//TODO: Handle logic

	// spin the coins 
	for (zluint i = 0; i < 11; i++) {;
		CoinPickup[i]->ReturnPickupModel()->Transform.Rotation.y += 120.0f * GetFDeltaTime();
	}

	Graphics->EngineDefaultCam->Update();

	// pointer to the camera collision box
	CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();

	//TODO: Any collisions of models in the game with camera
	for (zluint i = 0; i < 9; i++) {
		if (CamCol->IsOverlapping(*FloorStack[i]->GetCollision())) {
			OnFloor = true;
			Jumping = false;
		}
	}
	if (Archway1 != nullptr && CamCol->IsOverlapping(*Archway1->GetCollision())) {
		RemoveModelFromGame(Archway1);
	}

}

void Game::Draw()
{
	Graphics->ClearGraphics();

	Graphics->Draw();

	// pointer to the camera collision box
	CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();

	CamCol->DebugDraw(Vector3(0.0f, 1.0f, 0.0f));

	Lamps[0]->ReturnObstacleModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[0]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[1]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[2]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[3]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[4]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[5]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[6]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[7]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	FloorStack[8]->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	Archway1->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	CoinPickup[0]->ReturnPickupModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	BoxObstacle[0]->ReturnObstacleModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));

	Graphics->PresentGraphics();
}

void Game::CloseGame()
{
	//TODO: Clean up code
	delete GameInput;
}
