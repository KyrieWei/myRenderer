#include <iostream>

#include "pipelines/pipeline.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;

const char* file_name = "results/test.jpg";


int main()
{
	//create window
	callbacks_t callbacks;
	window_t* window = window_create("software renderer", WIDTH, HEIGHT);
	memset(&callbacks, 0, sizeof(callbacks_t));

	//triangle definition
	vec3 triangle[3] = { {-1, -1, 0}, {1, -1, 0}, {0, 1, 0} };

	//cube definition
	vec3 cube[8] =
	{
		{-1, -1, -1},
		{-1,  1, -1},
		{ 1,  1, -1},
		{ 1, -1, -1}, //front
		{ 1, -1,  1},
		{ 1,  1,  1},
		{-1,  1,  1},
		{-1, -1,  1} //back
	};

	int vertex_num = 8;

	//right hand counter-clock
	int cube_index[12][3] =
	{
		{0, 2, 1},
		{0, 3, 2}, //front
		{3, 5, 2},
		{3, 4, 5}, //right
		{0, 1, 6},
		{1, 6, 7}, //left
		{1, 2, 5},
		{2, 5, 6}, //up
		{3, 4, 7},
		{0, 3, 7}, //bottom
		{4, 5, 6},
		{4, 6, 7}  //back
	};

	int index_num = 12;

	//model matrix
	mat4x4 model;

	//camera back 
	mat4x4 view;
	view = translate(view, vec3(0.0, 0.0, -5.0));

	//perspective projection
	mat4x4 projection = perspective(radians(60.0), (double)WIDTH / HEIGHT, 0.1, 100.0);

	pipeline m_pipeline(WIDTH, HEIGHT);

	m_pipeline.clean_color(vec4(0.1, 0.3, 0.4, 1.0));

	//send data to 
	while (!window_should_close(window))
	{



		m_pipeline.setMVP(model, view, projection);

		m_pipeline.drawArrays(cube, vertex_num, cube_index, index_num);

		window_draw_image(window, &m_pipeline.forwardBuffer);
		input_poll_events();
	}

	window_destroy(window);

	m_pipeline.render();

	return 0;
}