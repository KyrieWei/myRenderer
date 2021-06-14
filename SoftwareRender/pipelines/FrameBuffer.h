#pragma once
#include <iostream>
#include "../maths/Common.h"

class FrameBuffer
{
public:
	int width, height, channel;
	unsigned char* image_buffer;

public:

	FrameBuffer();
	FrameBuffer(const int _width, const int _height, const int _channel);
	~FrameBuffer();

	void draw_background(const vec4& color);
	void draw_pixel(int x, int y, const vec4& color);
	void draw_line(const vec2& point0, const vec2& point1, const vec4& color);
	void draw_filled_triangle(const vec2& point0, const vec2& point1, const vec2& point2, const vec4& color);
	void write_to_image(const char* file_name);

private:
	bool check_buffer();
};

