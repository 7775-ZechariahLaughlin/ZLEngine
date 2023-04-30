#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

class Collision {
public:
	Collision(Vector3 Location, Vector3 Offset);
	~Collision();

	// detect if two collisions are overlapping
	virtual bool IsOverlapping(Collision& OtherCol) { return false; }

	// get the centre of the collision
	virtual Vector3 FindCenter() { return Vector3(0.0f); }

	// draw the collision for debugging
	virtual void DebugDraw(Vector3 Colour);

	// Transform Functions
	// set the location of the collision
	virtual void SetLocation(Vector3 NewLocation) { Transform.Location = NewLocation; }
	// update the offset
	virtual void SetOffset(Vector3 NewOffset) { Offset = NewOffset; }
	// set the rotation of the collision
	virtual void SetRotation(Vector3 NewRotation) { Transform.Rotation = NewRotation; }
	// set the rotation x of the collision
	virtual void SetRotationX(float Value) { Transform.Rotation.x = Value; }
	// set the rotation y of the collision
	virtual void SetRotationY(float Value) { Transform.Rotation.y = Value; }
	// set the rotation z of the collision
	virtual void SetRotationZ(float Value) { Transform.Rotation.z = Value; }

public:

protected:
	// transforms for the collision in world space
	CTransform Transform;

	// offset for the center location
	Vector3 Offset;

	// visual mesh for debugging
	MeshPtr DebugMesh;
	ShaderPtr DebugShader;
	MaterialPtr DebugMaterial;

protected:

private:

private:
};

class BoxCollision : public Collision {
public:
	BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimensions);
	
	bool IsOverlapping(Collision& OtherCol) override;
	Vector3 FindCenter() override;
	// draw a mesh to the dimension of the box
	// turn the mesh into a wireframe view to not obscure the actual object
	// create the mesh and a shader only if we need to debug the collision
	void DebugDraw(Vector3 Colour) override;
	void SetLocation(Vector3 NewLocation) override;

	// return the size of the box
	Vector3 GetDimensions() const { return Dimensions; }

	// set the dimensions of the box
	void SetDimensions(Vector3 NewDimensions);

public:

protected:
	// this is the bounding box values
	Vector3 Dimensions;

	// the bounding vectors between the minimum and maximum 
	Vector3 Min;
	Vector3 Max;

	// this determines whether the collision should be treated as a floor
	bool FloorCollisionBox;

};