#pragma once
#include "FrameBuffer.h"
#include "../gui/Window.h"

class pipeline
{
public:

	int screenWidth, screenHeight;

	mat4x4 model;
	mat4x4 view;
	mat4x4 projection;

	FrameBuffer forwardBuffer;
	FrameBuffer backwardBuffer;

	

public:
	pipeline();
	pipeline(int width, int height) : screenWidth(width), screenHeight(height), forwardBuffer(screenWidth, screenHeight, 4), backwardBuffer(screenWidth, screenHeight, 4) {}

	void setMVP(const mat4x4& mdoel, const mat4x4& view, const mat4x4& projection);
	void drawArrays(vec3 data[], int data_length, int index[][3], int index_length);
	void clean_color(const vec4& color);
	void render();
};

