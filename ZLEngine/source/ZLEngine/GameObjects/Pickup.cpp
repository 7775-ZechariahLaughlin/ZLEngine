#include "ZLEngine/GameObjects/Pickup.h"
#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Model.h"
#include "ZLEngine/GameObjects/Player.h"


Pickup::Pickup()
{
	PickupModel = nullptr;
	PointValue = 0;
}

Pickup::Pickup(ModelPtr ModelToUse, zluint PointValue)
{
	this->PickupModel = ModelToUse;
	this->PointValue = PointValue;
}

Pickup::~Pickup()
{
	PickupModel = nullptr;
}

ModelPtr Pickup::ReturnPickupModel()
{
	return PickupModel;
}

zluint Pickup::ReturnPickupPointValue()
{
	return PointValue;
}

