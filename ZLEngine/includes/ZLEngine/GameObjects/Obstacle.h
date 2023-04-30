#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/GameObjects/GameObject.h"

class Obstacle : public GameObject {
public: 
public:
	Obstacle();
	Obstacle(ModelPtr ModelToUse);
	~Obstacle();

	// returns the obstacle model
	ModelPtr ReturnObstacleModel();

public:

protected:
	// stores the model for the obstacle
	ModelPtr ObstacleModel;

protected:

private:


private:

};
