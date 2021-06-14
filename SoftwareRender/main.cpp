#include <iostream>
#include "pipelines/FrameBuffer.h"
#include "gui/Window.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;

const char* file_name = "results/test.jpg";

int main()
{
	
	callbacks_t callbacks;

	FrameBuffer fb(WIDTH, HEIGHT, CHANNEL);
	
	vec4 color(0.2, 0.6, 0.9, 1.0);
	vec2 point0(200, 200);
	vec2 point1(600, 200);
	vec2 point2(400, 500);

	fb.draw_background(vec4(0.1, 0.3, 0.4, 1.0));
	fb.draw_filled_triangle(point0, point1, point2, color);

	fb.write_to_image(file_name);

	window_t* window = window_create("software renderer", WIDTH, HEIGHT);

	memset(&callbacks, 0, sizeof(callbacks_t));


	while (!window_should_close(window))
	{
		window_draw_image(window, &fb);
		input_poll_events();
	}

	window_destroy(window);

	return 0;
}