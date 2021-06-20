#include "camera.h"
#include "../maths/Common.h"

camera::camera()
{
	Position = vec3(0.0, 0.0, 0.0);
	WorldUp = vec3(0.0, 1.0, 0.0);
	Front = vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	zoom = ZOOM;
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();
}

camera::camera(const vec3& pos, const vec3& up) : Front(vec3(0.0, 0.0, -1.0)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	Position = pos;
	WorldUp = up;
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();
}

camera::camera(double posX, double posY, double posZ, double upX, double upY, double upZ, double yaw, double pitch) : Front(vec3(0.0, 0.0, -1.0)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	Position = vec3(posX, posY, posZ);
	WorldUp = vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

mat4x4 camera::GetViewMatrix() const
{
	mat4x4 view;
	view = lookAt(Position, Position + Front, Up);
	return view;
}

void camera::ProcessKeyboard(camera_movement direction, double deltaTime)
{
	double velocity = movementSpeed * deltaTime;

	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void camera::ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	Yaw		+= xoffset;
	Pitch	+= yoffset;

	//make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0)
			Pitch = 89.0;
		if (Pitch < -89.0)
			Pitch = -89.0;
	}

	updateCameraVectors();
}

void camera::ProcessMouseScroll(double yoffset)
{
	zoom -= (double)yoffset;

	if (zoom < 1.0)
		zoom = 1.0;
	if (zoom > 45.0)
		zoom = 45.0;
}

void camera::updateCameraVectors()
{
	vec3 front;
	front.x = cos(radians(Yaw)) * cos(radians(Pitch));
	front.y = sin(radians(Pitch));
	front.z = sin(radians(Yaw)) * cos(radians(Pitch));
	Front = normalize(front);

	Right = normalize(cross(Front, WorldUp));
	Up = normalize(cross(Right, Front));
}
