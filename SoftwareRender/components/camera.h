#pragma once
#include "../maths/vec3.h"
#include "../maths/mat4x4.h"

enum camera_movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const double YAW		= -90.0;
const double PITCH		= -30.0;
const float SPEED		= 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM		= 45.0f;

class camera
{
public:
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;

	//euler angles
	double Yaw;
	double Pitch;

	//camera options;
	double movementSpeed;
	double mouseSensitivity;
	double zoom;

public:
	camera();
	camera(const vec3& pos, const vec3& up);
	camera(double posX, double posY, double posZ, double upX, double upY, double upZ, double yaw, double pitch);

	mat4x4 GetViewMatrix() const;

	void ProcessKeyboard(camera_movement direction, double deltaTime);

	void ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch = true);

	void ProcessMouseScroll(double yoffset);

private:
	void updateCameraVectors();
};

