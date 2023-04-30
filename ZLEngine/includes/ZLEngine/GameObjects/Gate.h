#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/GameObjects/Obstacle.h"

class Gate : public Obstacle {
public:
	Gate();
	Gate(ModelPtr ModelToUse);
	~Gate();

	// returns the gate model
	ModelPtr ReturnGateModel();

public:



private:
	// stores the model for the gate
	ModelPtr GateModel;

private:

};
