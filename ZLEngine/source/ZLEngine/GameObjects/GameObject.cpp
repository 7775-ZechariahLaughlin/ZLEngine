#include "ZLEngine/GameObjects/GameObject.h"

GameObject::GameObject()
{
    GameObjectLocation = Vector3(0.0f);
    GameObjectRotation= Vector3(0.0f);
    GameObjectScale = Vector3(1.0f);
}

GameObject::~GameObject()
{
}

Vector3 GameObject::FindLocation()
{
    return Vector3();
}

Vector3 GameObject::ReturnRotation()
{
    return Vector3();
}
