#include "ZLEngine/GameObjects/Obstacle.h"

Obstacle::Obstacle()
{
	ObstacleModel = nullptr;
}

Obstacle::Obstacle(ModelPtr ModelToUse)
{
	this->ObstacleModel = ModelToUse;
}

Obstacle::~Obstacle()
{
	ObstacleModel = nullptr;
}

ModelPtr Obstacle::ReturnObstacleModel()
{
	return ObstacleModel;
}
