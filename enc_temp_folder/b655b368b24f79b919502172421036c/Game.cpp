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
void Game::DeleteAllModels()
{
	// destroy walls
	for (zluint i = 0; i < 118; i++) {
		RemoveModelFromGame(Walls[i]->ReturnObstacleModel());
	}
	// destroy floors
	for (zluint i = 0; i < 10; i++) {
		RemoveModelFromGame(FloorStack[i]);
	}

	// destroy coins
	for (zluint i = 0; i < 11; i++) {
		RemoveModelFromGame(CoinPickup[i]->ReturnPickupModel());
	}

	// destroy boxes
	for (zluint i = 0; i < 20; i++) {
		RemoveModelFromGame(BoxObstacle[i]->ReturnObstacleModel());
	}
	// destroy barrels;
	for (zluint i = 0; i < 10; i++) {
		RemoveModelFromGame(BarrelObstacle[i]->ReturnObstacleModel());
	}
	// destroy bulbs
	for (zluint i = 0; i < 10; i++) {
		RemoveModelFromGame(Bulbs[i]->ReturnObstacleModel());

	}
	// destroy lamps
	for (zluint i = 0; i < 10; i++) {
		RemoveModelFromGame(Lamps[i]->ReturnObstacleModel());
	}
}
void Game::SpawnAllObjects()
{
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
	MaterialPtr MLampBase = make_shared<Material>();
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
	MWarmth->EmissiveColour.MultiplierV3 = Vector3(0.5f);

	//TODO: Tidy up and move into a SpawnHandler which will run as a single function in the main
	// create meshes
	// handle archway/gate creation
	for (zluint i = 0; i < 10; i++) {
		Archways[i] = make_shared<Gate>(Graphics->ImportModel("game/models/Archway.obj", TextureShader));
		Archways[i]->ReturnGateModel()->SetMaterialBySlot(0, MArchway);
		Archways[i]->ReturnGateModel()->Transform.Scale = Vector3(0.6f);
	}
	// handle wall creation
	for (zluint i = 0; i < 118; i++) {
		Walls[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/Concrete_Wall.obj", TextureShader));
		Walls[i]->ReturnObstacleModel()->SetMaterialBySlot(1, MWall);
		Walls[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.29f, 0.45f, 0.38f);
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
	for (zluint i = 0; i < 11; i++) {
		SkullPickup[i] = make_shared<Pickup>(Graphics->ImportModel("game/models/Skull_Pickup.fbx", TextureShader), 100);
		SkullPickup[i]->ReturnPickupModel()->SetMaterialBySlot(0, MSkull);
		SkullPickup[i]->ReturnPickupModel()->Transform.Scale = Vector3(0.15f);
	}

	// handle box obstacle creation
	for (zluint i = 0; i < 20; i++) {
		BoxObstacle[i] = make_shared<Obstacle>(Graphics->ImportModel("game/models/Wood_Box.obj", TextureShader));
		BoxObstacle[i]->ReturnObstacleModel()->Transform.Scale = Vector3(0.08f);
		BoxObstacle[i]->ReturnObstacleModel()->SetMaterialBySlot(1, MBox);
	}
	// handle barrel obstacle creation
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

}
void Game::MoveAllObjects()
{
	// transform the models location 
	// transform lightbulb for each lamp
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

	// transform lamps
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

	// transform archways
	Archways[0]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[1]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[2]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[3]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[4]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[5]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[6]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[7]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[8]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);
	Archways[9]->ReturnGateModel()->Transform.Location = Vector3(-7.8f, 6.0f, -7.0f);

	// transform box obstacles
	BoxObstacle[0]->ReturnObstacleModel()->Transform.Location = Vector3(3.4f, 1.7f, 1.2f);
	BoxObstacle[1]->ReturnObstacleModel()->Transform.Location = Vector3(3.4f, 1.7f, -1.2f);
	BoxObstacle[2]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -3.6f);
	BoxObstacle[3]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -6.0f);
	BoxObstacle[4]->ReturnObstacleModel()->Transform.Location = Vector3(5.8f, 1.7f, -8.4f);
	BoxObstacle[5]->ReturnObstacleModel()->Transform.Location = Vector3(3.0f, 1.7f, -13.8f);
	BoxObstacle[6]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 1.7f, -58.0f);
	BoxObstacle[7]->ReturnObstacleModel()->Transform.Location = Vector3(-26.0f, 1.7f, -56.1f);
	BoxObstacle[8]->ReturnObstacleModel()->Transform.Location = Vector3(-26.0f, 4.1f, -58.5f);
	BoxObstacle[9]->ReturnObstacleModel()->Transform.Location = Vector3(-26.0f, 1.7f, -58.5f);
	BoxObstacle[10]->ReturnObstacleModel()->Transform.Location = Vector3(-23.6f, 3.4f, -58.5f);
	BoxObstacle[11]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[12]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[13]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[14]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[15]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[16]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[17]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[18]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);
	BoxObstacle[19]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 3.4f, -58.0f);

	// transform barrel obstacles
	BarrelObstacle[0]->ReturnObstacleModel()->Transform.Location = Vector3(3.5f, 0.5f, -3.4f);
	BarrelObstacle[1]->ReturnObstacleModel()->Transform.Location = Vector3(4.8f, 0.5f, -10.5f);
	BarrelObstacle[2]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 2.0f, -40.0f);
	BarrelObstacle[3]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 0.5f, -40.0f);

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
	Walls[0]->ReturnObstacleModel()->Transform.Location = Vector3(12.0f, 0.0f, 9.8f);
	Walls[1]->ReturnObstacleModel()->Transform.Location = Vector3(12.0f, 0.0f, 4.2f);
	Walls[2]->ReturnObstacleModel()->Transform.Location = Vector3(12.0f, 0.0f, -1.4f);
	Walls[3]->ReturnObstacleModel()->Transform.Location = Vector3(12.0f, 0.0f, -7.0f);
	Walls[4]->ReturnObstacleModel()->Transform.Location = Vector3(12.0f, 0.0f, -12.6f);

	// room one east walls
	Walls[5]->ReturnObstacleModel()->Transform.Location = Vector3(8.7f, 0.0f, 12.3f);
	Walls[6]->ReturnObstacleModel()->Transform.Location = Vector3(3.1f, 0.0f, 12.3f);
	Walls[7]->ReturnObstacleModel()->Transform.Location = Vector3(-2.1f, 0.0f, 12.3f);
	Walls[8]->ReturnObstacleModel()->Transform.Location = Vector3(-7.7f, 0.0f, 12.3f);

	// room one dividers
	Walls[9]->ReturnObstacleModel()->Transform.Location = Vector3(9.0f, 0.0f, 2.8f);
	Walls[10]->ReturnObstacleModel()->Transform.Location = Vector3(3.2f, 0.0f, 2.8f);
	Walls[26]->ReturnObstacleModel()->Transform.Location = Vector3(-2.6f, 0.0f, 2.8f);

	// room one south walls
	Walls[11]->ReturnObstacleModel()->Transform.Location = Vector3(-11.0f, 0.0f, 9.8f);
	Walls[12]->ReturnObstacleModel()->Transform.Location = Vector3(-11.0f, 0.0f, 4.2f);

	// room one west walls
	Walls[13]->ReturnObstacleModel()->Transform.Location = Vector3(9.2f, 0.0f, -15.4f);
	Walls[14]->ReturnObstacleModel()->Transform.Location = Vector3(3.4f, 0.0f, -15.4f);

	// hallway one north walls
	Walls[15]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -17.9f);
	Walls[16]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -23.7f);
	Walls[17]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -28.5f);
	Walls[18]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -34.3f);
	Walls[19]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -40.1f);
	Walls[20]->ReturnObstacleModel()->Transform.Location = Vector3(0.2f, 0.0f, -45.9f);

	// hallway one south walls
	Walls[21]->ReturnObstacleModel()->Transform.Location = Vector3(-11.8f, 0.0f, -14.9f);
	Walls[22]->ReturnObstacleModel()->Transform.Location = Vector3(-11.8f, 0.0f, -20.7f);
	Walls[23]->ReturnObstacleModel()->Transform.Location = Vector3(-11.8f, 0.0f, -25.5f);
	Walls[24]->ReturnObstacleModel()->Transform.Location = Vector3(-11.8f, 0.0f, -31.3f);
	Walls[25]->ReturnObstacleModel()->Transform.Location = Vector3(-11.8f, 0.0f, -37.1f);

	// room two east walls
	Walls[27]->ReturnObstacleModel()->Transform.Location = Vector3(-15.0f, 0.0f, -39.6f);
	Walls[28]->ReturnObstacleModel()->Transform.Location = Vector3(-20.8f, 0.0f, -39.6f);
	Walls[29]->ReturnObstacleModel()->Transform.Location = Vector3(-26.6f, 0.0f, -39.6f);

	// hallway one east walls
	Walls[30]->ReturnObstacleModel()->Transform.Location = Vector3(-2.3f, 0.0f, -49.2f);
	Walls[31]->ReturnObstacleModel()->Transform.Location = Vector3(-8.1f, 0.0f, -49.2f);

	// hallway two east walls
	Walls[32]->ReturnObstacleModel()->Transform.Location = Vector3(-13.5f, 0.0f, 0.9f);
	Walls[33]->ReturnObstacleModel()->Transform.Location = Vector3(-19.2f, 0.0f, 0.9f);
	Walls[34]->ReturnObstacleModel()->Transform.Location = Vector3(-25.0f, 0.0f, 0.9f);
	Walls[35]->ReturnObstacleModel()->Transform.Location = Vector3(-30.8f, 0.0f, 0.9f);
	Walls[36]->ReturnObstacleModel()->Transform.Location = Vector3(-36.6f, 0.0f, 0.9f);
	Walls[37]->ReturnObstacleModel()->Transform.Location = Vector3(-42.4f, 0.0f, 0.9f);

	// hallway two west walls
	Walls[38]->ReturnObstacleModel()->Transform.Location = Vector3(-14.3f, 0.0f, -12.4f);
	Walls[39]->ReturnObstacleModel()->Transform.Location = Vector3(-20.0f, 0.0f, -12.4f);
	Walls[40]->ReturnObstacleModel()->Transform.Location = Vector3(-25.8f, 0.0f, -12.4f);
	Walls[41]->ReturnObstacleModel()->Transform.Location = Vector3(-43.2f, 0.0f, -13.0f);

	// room two north walls
	Walls[42]->ReturnObstacleModel()->Transform.Location = Vector3(-11.4f, 0.0f, -51.7f);
	Walls[43]->ReturnObstacleModel()->Transform.Location = Vector3(-11.4f, 0.0f, -57.5f);

	// room two west walls
	Walls[44]->ReturnObstacleModel()->Transform.Location = Vector3(-14.7f, 0.0f, -60.7f);
	Walls[45]->ReturnObstacleModel()->Transform.Location = Vector3(-20.5f, 0.0f, -60.7f);
	Walls[46]->ReturnObstacleModel()->Transform.Location = Vector3(-26.3f, 0.0f, -60.7f);

	// room two south walls
	Walls[47]->ReturnObstacleModel()->Transform.Location = Vector3(-28.7f, 0.0f, -57.4f);

	// hallway three west walls
	Walls[48]->ReturnObstacleModel()->Transform.Location = Vector3(-31.2f, 0.0f, -54.1f);
	Walls[49]->ReturnObstacleModel()->Transform.Location = Vector3(-37.0f, 0.0f, -54.1f);

	// hallway three north walls
	Walls[50]->ReturnObstacleModel()->Transform.Location = Vector3(-29.2f, 0.0f, -37.0f);
	Walls[51]->ReturnObstacleModel()->Transform.Location = Vector3(-29.2f, 0.0f, -31.2f);
	Walls[52]->ReturnObstacleModel()->Transform.Location = Vector3(-29.2f, 0.0f, -25.4f);
	Walls[53]->ReturnObstacleModel()->Transform.Location = Vector3(-29.2f, 0.0f, -19.6f);
	Walls[54]->ReturnObstacleModel()->Transform.Location = Vector3(-29.2f, 0.0f, -13.8f);

	// hallway three south walls
	Walls[55]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -50.8f);
	Walls[56]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -45.0f);
	Walls[57]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -39.2f);
	Walls[58]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -33.4f);
	Walls[59]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -27.6f);
	Walls[60]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -21.8f);
	Walls[61]->ReturnObstacleModel()->Transform.Location = Vector3(-40.0f, 0.0f, -16.0f);

	// room three north walls
	Walls[62]->ReturnObstacleModel()->Transform.Location = Vector3(-45.7f, 0.0f, 3.5f);
	Walls[63]->ReturnObstacleModel()->Transform.Location = Vector3(-46.1f, 0.0f, -15.5f);
	Walls[64]->ReturnObstacleModel()->Transform.Location = Vector3(-46.1f, 0.0f, -21.3f);
	Walls[65]->ReturnObstacleModel()->Transform.Location = Vector3(-46.1f, 0.0f, -27.2f);

	// room three west walls
	Walls[66]->ReturnObstacleModel()->Transform.Location = Vector3(-48.6f, 0.0f, -30.2f);
	Walls[67]->ReturnObstacleModel()->Transform.Location = Vector3(-54.4f, 0.0f, -30.2f);
	Walls[68]->ReturnObstacleModel()->Transform.Location = Vector3(-60.2f, 0.0f, -30.2f);
	Walls[69]->ReturnObstacleModel()->Transform.Location = Vector3(-66.0f, 0.0f, -30.2f);
	Walls[70]->ReturnObstacleModel()->Transform.Location = Vector3(-71.8f, 0.0f, -30.2f);
	Walls[71]->ReturnObstacleModel()->Transform.Location = Vector3(-77.6f, 0.0f, -30.2f);
	Walls[72]->ReturnObstacleModel()->Transform.Location = Vector3(-83.4f, 0.0f, -30.2f);
	Walls[73]->ReturnObstacleModel()->Transform.Location = Vector3(-89.2f, 0.0f, -30.2f);
	Walls[74]->ReturnObstacleModel()->Transform.Location = Vector3(-95.0f, 0.0f, -30.2f);

	// room three east walls
	Walls[75]->ReturnObstacleModel()->Transform.Location = Vector3(-48.6f, 0.0f, 6.4f);
	Walls[76]->ReturnObstacleModel()->Transform.Location = Vector3(-54.4f, 0.0f, 6.4f);
	Walls[77]->ReturnObstacleModel()->Transform.Location = Vector3(-60.2f, 0.0f, 6.4f);
	Walls[78]->ReturnObstacleModel()->Transform.Location = Vector3(-66.0f, 0.0f, 6.4f);
	Walls[79]->ReturnObstacleModel()->Transform.Location = Vector3(-71.8f, 0.0f, 6.4f);
	Walls[80]->ReturnObstacleModel()->Transform.Location = Vector3(-77.6f, 0.0f, 6.4f);
	Walls[81]->ReturnObstacleModel()->Transform.Location = Vector3(-83.4f, 0.0f, 6.4f);
	Walls[82]->ReturnObstacleModel()->Transform.Location = Vector3(-89.2f, 0.0f, 6.4f);
	Walls[83]->ReturnObstacleModel()->Transform.Location = Vector3(-95.0f, 0.0f, 6.4f);

	// room three south walls
	Walls[84]->ReturnObstacleModel()->Transform.Location = Vector3(-97.9f, 0.0f, 3.8f);
	Walls[85]->ReturnObstacleModel()->Transform.Location = Vector3(-97.9f, 0.0f, -2.0f);
	Walls[86]->ReturnObstacleModel()->Transform.Location = Vector3(-97.9f, 0.0f, -7.8f);
	Walls[87]->ReturnObstacleModel()->Transform.Location = Vector3(-97.9f, 0.0f, -13.6f);

	// hallway four east walls
	Walls[88]->ReturnObstacleModel()->Transform.Location = Vector3(-100.8f, 0.0f, -16.5f);

	// hallway four south walls
	Walls[89]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -19.4f);
	Walls[90]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -25.2f);
	Walls[91]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -31.0f);
	Walls[92]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -36.8f);
	Walls[93]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -42.6f);
	Walls[94]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -48.4f);
	Walls[95]->ReturnObstacleModel()->Transform.Location = Vector3(-103.7f, 0.0f, -54.2f);

	// hallway four west walls
	Walls[96]->ReturnObstacleModel()->Transform.Location = Vector3(-100.4f, 0.0f, -55.8f);
	Walls[97]->ReturnObstacleModel()->Transform.Location = Vector3(-94.6f, 0.0f, -55.8f);

	// hallway four east walls
	Walls[98]->ReturnObstacleModel()->Transform.Location = Vector3(-95.0f, 0.0f, -31.2f);

	// hallway four north walls
	Walls[99]->ReturnObstacleModel()->Transform.Location = Vector3(-92.0f, 0.0f, -34.1f);
	Walls[100]->ReturnObstacleModel()->Transform.Location = Vector3(-92.0f, 0.0f, -39.9f);

	// room four south walls
	Walls[101]->ReturnObstacleModel()->Transform.Location = Vector3(-91.4f, 0.0f, -58.4f);
	Walls[102]->ReturnObstacleModel()->Transform.Location = Vector3(-91.4f, 0.0f, -64.2f);

	// room four east walls
	Walls[103]->ReturnObstacleModel()->Transform.Location = Vector3(-89.2f, 0.0f, -43.2f);
	Walls[104]->ReturnObstacleModel()->Transform.Location = Vector3(-83.4f, 0.0f, -43.2f);
	Walls[105]->ReturnObstacleModel()->Transform.Location = Vector3(-77.6f, 0.0f, -43.2f);
	Walls[106]->ReturnObstacleModel()->Transform.Location = Vector3(-71.8f, 0.0f, -43.2f);
	Walls[107]->ReturnObstacleModel()->Transform.Location = Vector3(-66.0f, 0.0f, -43.2f);

	// room four west walls
	Walls[108]->ReturnObstacleModel()->Transform.Location = Vector3(-89.2f, 0.0f, -67.1f);
	Walls[109]->ReturnObstacleModel()->Transform.Location = Vector3(-83.4f, 0.0f, -67.1f);
	Walls[110]->ReturnObstacleModel()->Transform.Location = Vector3(-77.6f, 0.0f, -67.1f);
	Walls[111]->ReturnObstacleModel()->Transform.Location = Vector3(-71.8f, 0.0f, -67.1f);
	Walls[112]->ReturnObstacleModel()->Transform.Location = Vector3(-66.0f, 0.0f, -67.1f);

	// room four north walls
	Walls[113]->ReturnObstacleModel()->Transform.Location = Vector3(-63.1f, 0.0f, -67.1f);
	Walls[114]->ReturnObstacleModel()->Transform.Location = Vector3(-63.1f, 0.0f, -61.3f);
	Walls[115]->ReturnObstacleModel()->Transform.Location = Vector3(-63.1f, 0.0f, -55.5f);
	Walls[116]->ReturnObstacleModel()->Transform.Location = Vector3(-63.1f, 0.0f, -49.7f);
	Walls[117]->ReturnObstacleModel()->Transform.Location = Vector3(-63.1f, 0.0f, -43.9f);
}
void Game::RotateAllObjects()
{
	// rotate walls
	Walls[0]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[1]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[2]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[3]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[4]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[7]->ReturnObstacleModel()->Transform.Rotation.y += 180.0f;
	Walls[8]->ReturnObstacleModel()->Transform.Rotation.y += 180.0f;
	Walls[11]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[12]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[15]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[16]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[17]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[18]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[19]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[20]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[21]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[22]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[23]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[24]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[25]->ReturnObstacleModel()->Transform.Rotation.y += 270.0f;
	Walls[42]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[43]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[47]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[50]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[51]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[52]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[53]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[54]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[55]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[56]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[57]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[58]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[59]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[60]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[61]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[62]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[63]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[64]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[65]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[84]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[85]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[86]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[87]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[89]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[90]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[91]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[92]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[93]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[94]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[95]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[99]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[100]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[101]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[102]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[113]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[114]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[115]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[116]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;
	Walls[117]->ReturnObstacleModel()->Transform.Rotation.y += 90.0f;

}
void Game::ScaleFloors()
{
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

}
void Game::DetectAndResolveCollisions(Vector3 CameraInput)
{
	PositionToMove = Graphics->EngineDefaultCam->CalculateMovementInput(CameraInput, CurrentState);
	Graphics->EngineDefaultCam->GetNextPosition()->SetLocation(PositionToMove);

	// loop through walls
	for (zluint i = 0; i < 118; i++) {
		// check if collision with a wall
		if (Walls[i] != nullptr && Graphics->EngineDefaultCam->GetNextPosition()->IsOverlapping(*Walls[i]->ReturnObstacleModel()->GetCollision())) {
			cout << "Colliding Wall" << endl;
			// collision happened, so stay at original location
			PositionToMove = Graphics->EngineDefaultCam->GetTransforms().Location;
		}
	}
	// loop through floors
	for (zluint i = 0; i < 9; i++) {
		// check if collision with floor
		if (FloorStack[i] != nullptr && Graphics->EngineDefaultCam->GetNextPosition()->IsOverlapping(*FloorStack[i]->GetCollision())) {
			cout << "Colliding Wall" << endl;
			// collision happened, so move but stay above the floor
			PositionToMove.y = Graphics->EngineDefaultCam->GetTransforms().Location.y;
		}
	}
	// check if new camera position isn't the same as the current position
	if (PositionToMove != Graphics->EngineDefaultCam->GetTransforms().Location) {
		// move the camera
		Graphics->EngineDefaultCam->Translate(PositionToMove);
	}

}
Game::Game()
{
	cout << "Game Initialised!" << endl;

	CurrentState = PlayerState::PlayerMode;
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

		Graphics->EngineDefaultCam->Translate(Vector3(9.0f, 3.0f, 9.0f));

		SpawnAllObjects();

		MoveAllObjects();

		RotateAllObjects();

		ScaleFloors();

		// add collisions to models
		Archways[0]->ReturnGateModel()->AddCollisionToModel(Vector3(6.0f, 7.0f, 1.5f), Vector3(-0.5f, -3.0f, 9.75f));

		// add collision to floors
		FloorStack[0]->AddCollisionToModel(Vector3(24.0f, 1.0f, 27.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[1]->AddCollisionToModel(Vector3(12.0f, 1.0f, 36.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[2]->AddCollisionToModel(Vector3(36.0f, 1.0f, 12.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[3]->AddCollisionToModel(Vector3(17.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[4]->AddCollisionToModel(Vector3(12.0f, 1.0f, 42.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[5]->AddCollisionToModel(Vector3(52.0f, 1.0f, 36.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[6]->AddCollisionToModel(Vector3(6.5f, 1.0f, 15.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[7]->AddCollisionToModel(Vector3(14.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));
		FloorStack[8]->AddCollisionToModel(Vector3(30.0f, 1.0f, 24.0f), Vector3(0.0f, -0.5f, 0.0f));

		// add collisions to walls rotated 90 degrees
		for (zluint i = 0; i < 5; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 11; i < 13; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 15; i < 26; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 42; i < 44; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		Walls[47]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f),  Vector3(0.0f, 2.7f, 0.0f));
		for (zluint i = 50; i < 66; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 84; i < 96; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 99; i < 103; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f),  Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 113; i < 118; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(1.2f, 5.8f, 5.8f),  Vector3(0.0f, 2.7f, 0.0f));
		}

		// add collisions to normal rotation walls
		for (zluint i = 5; i < 11; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 13; i < 15; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 26; i < 42; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 44; i < 47; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 48; i < 50; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 66; i < 84; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 96; i < 99; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}
		for (zluint i = 103; i < 113; i++) {
			Walls[i]->ReturnObstacleModel()->AddCollisionToModel(Vector3(5.8f, 5.8f, 1.2f), Vector3(0.0f, 2.7f, 0.0f));
		}

		// add collisions to coins
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
		// speed up the player movement
		Graphics->EngineDefaultCam->SetCameraSpeed(15.0f);
		cout << "Movement | Running..." << endl;
	}
	// divide the move speed for crouching
	if (GameInput->IsKeyDown(SDL_SCANCODE_C)) {
		cout << "Movement | Crouching..." << endl;
		// slow down the player movement
		Graphics->EngineDefaultCam->SetCameraSpeed(2.75f);

	}
	// check right mouse button is held, change to editor mode if held
	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		if (CurrentState == PlayerState::PlayerMode) {
			cout << "Mode | Changing to Editor Mode" << endl;
			CurrentState = PlayerState::EditorMode;
			GameInput->ShowCursor(true);
		}
	}
	else {
		if (CurrentState == PlayerState::EditorMode) {
			cout << "Mode | Changing to Player Mode" << endl;
			CurrentState = PlayerState::PlayerMode;
			GameInput->ShowCursor(false);
		}
	}

	CameraInput *= MoveSpeed * GetFDeltaTime();

	DetectAndResolveCollisions(CameraInput);

	Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta);
	Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta);
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

	if (Archways[0] != nullptr && CamCol->IsOverlapping(*Archways[0]->ReturnGateModel()->GetCollision())) {
		RemoveModelFromGame(Archways[0]->ReturnGateModel());
	}

}

void Game::Draw()
{
	Graphics->ClearGraphics();

	Graphics->Draw();

	// pointer to the camera collision box
	CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();

	CamCol->DebugDraw(Vector3(0.0f, 1.0f, 0.0f));
	Graphics->EngineDefaultCam->GetNextPosition()->DebugDraw(Vector3(0.0f, 1.0f, 0.0f));

	Lamps[0]->ReturnObstacleModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	Archways[0]->ReturnGateModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	CoinPickup[0]->ReturnPickupModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));
	BoxObstacle[0]->ReturnObstacleModel()->GetCollision()->DebugDraw(Vector3(1.0f, 0.0f, 0.0f));

	Graphics->PresentGraphics();
}

void Game::CloseGame()
{
	delete GameInput;
}
