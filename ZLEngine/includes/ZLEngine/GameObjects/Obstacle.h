#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/GameObjects/GameObject.h"

class Obstacle : public GameObject {
public: 
public:
	Obstacle();
	Obstacle(ModelPtr ModelToUse);
	~Obstacle();

	// returns the pickups model
	ModelPtr ReturnObstacleModel();

public:

protected:
	// stores the model for the pickup
	ModelPtr ObstacleModel;

protected:

private:


private:

};
