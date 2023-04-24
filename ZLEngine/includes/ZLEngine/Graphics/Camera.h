#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "ZLEngine/Math/Transformations.h"

struct STCameraData {
	// how fast the camera can move
	float Speed = 5.0f;

	// basically the zoom
	// how much of the view space the camera can see - in degrees
	// think peripheral vision
	float FOV = 70.0f;

	// how close can models get to the camera before disappearing
	float NearClip = 0.01f;

	// how far can models get from the camera before disappearing
	float FarClip = 1000.0f;

	// how fast the camera rotates
	float LookSensitivity = 0.2f;
};
class Camera {
public: 
	Camera();

	// move camera in 3D space to a location
	void Translate(Vector3 Location);

	// move the camera in a direction in 3D space
	void AddMovementInput(Vector3 Direction);

	// return the transform data
	CTransform GetTransforms() const { return Transform; }

	// return the current facing directions
	CDirection GetDirections() const { return Directions; }

	// return the extra camera information
	// Speed, FOV, Near and Far Clip
	STCameraData GetCameraData() const { return CameraData; }

	// return a view matrix that considers position and rotation
	glm::mat4 GetViewMatrix() const;

	// rotate the camera based on yaw
	void RotatePitch(float Amount);

	// rotate the camera based on pitch
	void RotateYaw(float Amount);

	// access and change the camera speed
	void SetCameraSpeed(float Amount);

	// zoom the camera with scroll wheel
	void SetCameraFOV(float Amount);

	// update camera logic
	void Update();

	// get the collisiuon for the camera
	CollisionPtr GetCameraCollision() const { return CameraCollision; }

private:

	// find the current direction vectors based on the rotation of the YAW and PITCH of the camera
	void UpdateDirectionVectors();

private:
	// Transforms of the camera
	// Location, Rotation, Scale - we dont need that
	CTransform Transform;

	// Forward, Right and Up directions
	CDirection Directions;

	// hold all the extra camera info
	STCameraData CameraData;

	CollisionPtr CameraCollision;

	// the current velocity of the camera
	Vector3 Velocity;
};
