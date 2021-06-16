#include "pipeline.h"


void pipeline::render()
{
	//create window
	callbacks_t callbacks;
	window_t* window = window_create("software renderer", screenWidth, screenHeight);
	memset(&callbacks, 0, sizeof(callbacks_t));

	//triangle definition
	vec3 triangle[3] = { {1, 0, 0}, {0, 3, 0}, {-1, 0, 0} };

	//model matrix
	mat4x4 model;

	//camera back 
	mat4x4 view;
	view = translate(view, vec3(0.0, 0.0, -3.0));

	//perspective projection
	mat4x4 projection = perspective(radians(45.0), (double)screenWidth / screenHeight, 0.1, 100.0);

	forwardBuffer.draw_background(vec4(0.1, 0.3, 0.4, 1.0));

	//triangle color
	vec4 color(0.2, 0.6, 0.9, 1.0);

	while (!window_should_close(window))
	{
		//clip space
		vec4 tri_0 = projection * view * model * vec4(triangle[0], 1.0);
		vec4 tri_1 = projection * view * model * vec4(triangle[1], 1.0);
		vec4 tri_2 = projection * view * model * vec4(triangle[2], 1.0);

		//ndc space
		vec3 tri_one = vec3(tri_0.x / tri_0.w, tri_0.y / tri_0.w, tri_0.z / tri_0.w);
		vec3 tri_two = vec3(tri_1.x / tri_0.w, tri_1.y / tri_1.w, tri_1.z / tri_1.w);
		vec3 tri_three = vec3(tri_2.x / tri_2.w, tri_2.y / tri_2.w, tri_2.z / tri_2.w);

		//screen space
		vec2 ndc_0 = viewport(tri_one, screenWidth, screenHeight);
		vec2 ndc_1 = viewport(tri_two, screenWidth, screenHeight);
		vec2 ndc_2 = viewport(tri_three, screenWidth, screenHeight);

		forwardBuffer.draw_filled_triangle(ndc_0, ndc_1, ndc_2, color);

		window_draw_image(window, &forwardBuffer);
		input_poll_events();
	}

	window_destroy(window);
	

}