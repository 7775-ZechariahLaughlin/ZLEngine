#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/GameObjects/GameObject.h"
#include <string>

struct STPlayerData {
	// holds the name of the current player in play
	string Name;
	// holds the current player's points
	zluint Points;
	// holds the movespeed of the player
	float MoveSpeed;
};
class Player : public GameObject {
public:
	// returns the point value to be used in the interface
	zluint GetPoints();
	// returns the player's name to be used in the interface
	string GetName();
	// returns the player's move speed, will be used in calculations for moving
	float GetMoveSpeed();
	// this will iterate the points when a player collides with point modifiers
	void UpdatePoints(zluint Amount);

public:
	// holds the player's model, currently unused as player has no model
	ModelPtr PlayerModel;

private:
	STPlayerData PlayerData;
	
private:

};
