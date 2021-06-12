#include <iostream>
#include "FrameBuffer.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;

const char* file_name = "results/test.jpg";

int main()
{
	
	FrameBuffer fb(WIDTH, HEIGHT, CHANNEL);
	
	vec4 color(1.0, 1.0, 1.0, 1.0);
	vec2 point0(40, 40);
	vec2 point1(700, 300);
	vec2 point2(400, 580);

	fb.draw_filled_triangle(point0, point1, point2, color);

	fb.write_to_image(file_name);

	return 0;
}