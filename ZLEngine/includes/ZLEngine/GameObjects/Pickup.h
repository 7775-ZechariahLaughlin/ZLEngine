#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/GameObjects/GameObject.h"

class Pickup : public GameObject {
public:
	Pickup();
	Pickup(ModelPtr ModelToUse, zluint PointValue);
	~Pickup();

	// returns the pickups model
	ModelPtr ReturnPickupModel();

	// returns the specific point value of the pickup to update player class
	zluint ReturnPickupPointValue();

public:

protected:
	// stores the pickups point value
	zluint PointValue;
	// stores the model for the pickup
	ModelPtr Model;

protected:

private:
	

private:
	
};
