#pragma once
#include "FrameBuffer.h"
#include "../gui/Window.h"

class pipeline
{
public:

	int screenWidth, screenHeight;

	FrameBuffer forwardBuffer;
	FrameBuffer backwardBuffer;

public:
	pipeline();
	pipeline(int width, int height) : screenWidth(width), screenHeight(height), forwardBuffer(screenWidth, screenHeight, 4), backwardBuffer(screenWidth, screenHeight, 4) {}
	void render();
};

