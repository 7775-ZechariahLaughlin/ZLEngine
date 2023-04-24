#include "ZLEngine/Graphics/Camera.h"
#include "GLM/gtc/matrix_transform.hpp"
#include "ZLEngine/Game.h"
#include "ZLEngine/Collisions/Collision.h"

Camera::Camera()
{
	UpdateDirectionVectors();

	Transform.Location = Vector3(8.0f, 2.5f, 8.0f);
	Transform.Rotation.y = 180.0f;

	Transform.Location -= Directions.Forward * 2.0f;

	// @param1 - Position of collision
	// @param2 - Offset of the location
	// @param3 - Size of the camera collision
	CameraCollision = make_shared<BoxCollision>(Transform.Location, Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 2.5f, 1.0f));

}

void Camera::Translate(Vector3 Location)
{
	Transform.Location = Location;
}

void Camera::AddMovementInput(Vector3 Direction)
{
	if (glm::length(Direction) == 0) {
		return;
	}
	// divive the vector by it's length
	// don't normalise if the direction is 0
	Direction = glm::normalize(Direction);

	// set the velocity of the camera using the speed and input direction
	Velocity = Direction * (CameraData.Speed * Game::GetGameInstance().GetFDeltaTime());

	// create a new location for the camera based on it's position and current velocity
	Vector3 NewPosition = Transform.Location + Velocity;

	// make sure the camera has actually been told to move
	if (Transform.Location != NewPosition) {
		// move the camera to the new position
		Translate(NewPosition);
	}
}

glm::mat4 Camera::GetViewMatrix() const
{
	/* Look at Function
	* eye - view position of the camera
	* center - what the eye is looking at (Eye Location + Forward Direction)
	* up - location of up
	*/
	return glm::lookAt(Transform.Location, Transform.Location + Directions.Forward, Directions.Up);
}

void Camera::RotatePitch(float Amount)
{
	// rotating past -89 or 89 will result in a yaw flip and flip the cam
	Transform.Rotation.x += Amount * CameraData.LookSensitivity;

	// clamp the results between the two max values to avoid flip
	if (Transform.Rotation.x > 89.99f) {
		Transform.Rotation.x = 89.99f;
	}

	if (Transform.Rotation.x < -89.99f) {
		Transform.Rotation.x = -89.99f;
	}

	UpdateDirectionVectors();
}

void Camera::RotateYaw(float Amount)
{
	// this can currently increase to a max number
	Transform.Rotation.y += Amount * CameraData.LookSensitivity;

	// when the yaw gets to 360 change it to 0
	Transform.Rotation.y = glm::mod(Transform.Rotation.y, 360.0f);

	UpdateDirectionVectors();
}

void Camera::SetCameraSpeed(float Amount)
{
	CameraData.Speed = Amount;
}

void Camera::SetCameraFOV(float Amount)
{

}

void Camera::Update()
{
	if (CameraCollision != nullptr) {
		CameraCollision->SetLocation(Transform.Location);
	}
}

void Camera::UpdateDirectionVectors()
{
	// create a Vector3 to initialise a 0 direction
	Vector3 ForwardDirection;

	// cosine of the Yaw * cosine of the Pitch
	ForwardDirection.x = cos(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));
	// sine of the pitch
	ForwardDirection.y = sin(glm::radians(Transform.Rotation.x));
	// sine of the Yaw and cosine of the Pitch
	ForwardDirection.z = sin(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));

	// normalise the direction to update the values to be between 0 and 1
	ForwardDirection = glm::normalize(ForwardDirection);

	// set the forward direction
	Directions.Forward = ForwardDirection;

	/* Cross Product
	* - will allow us to get our right and up vectors from the forward vector
	* cross product is the axis that is perpendicular to two other axis
	*/

	// set the right direction to always be perpendicular to the world up
	// this means we strafe on the world x and z
	Directions.Right = glm::normalize(glm::cross(Directions.Forward, Vector3(0.0f, 1.0f, 0.0f)));

	// the up direction is based on the local rotation of the forward and right directions
	Directions.Up = glm::normalize(glm::cross(Directions.Right, Directions.Forward));
}
