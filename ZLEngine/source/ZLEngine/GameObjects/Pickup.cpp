#include "ZLEngine/GameObjects/Pickup.h"
#include "ZLEngine/Game.h"
#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "ZLEngine/Graphics/Model.h"
#include "ZLEngine/GameObjects/Player.h"


Pickup::Pickup()
{
	Model = nullptr;
	PointValue = 0;
}

Pickup::Pickup(ModelPtr ModelToUse, zluint PointValue)
{
	this->Model = ModelToUse;
	this->PointValue = PointValue;
}

Pickup::~Pickup()
{
	Model = nullptr;
}

ModelPtr Pickup::ReturnPickupModel()
{
	return Model;
}

zluint Pickup::ReturnPickupPointValue()
{
	return PointValue;
}

