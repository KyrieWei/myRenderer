#include <iostream>
#include <ctime>

#include "pipelines/pipeline.h"
#include "tools/model.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;


void handle_key_pressed(window_t* window);

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

	//Load model
	Model nier;
	nier.load("assets/nier2b7.obj");

	std::cout << nier.vertex_num << std::endl;
	std::cout << nier.tex_num << std::endl;
	std::cout << nier.face_num << std::endl;

	std::cout << nier.normal_index[0][0] << std::endl;

	pipeline m_pipeline(WIDTH, HEIGHT);

	vec4 bg_color = vec4(0.1, 0.3, 0.4, 1.0);
	m_pipeline.clean_color(bg_color);

	int rate = 4.0;
	double angle = 0.0;
	double scale_factor = 15.0;

	//send data to 
	while (!window_should_close(window))
	{
		m_pipeline.clean_color(bg_color);

		//TODO: add camera
		//handle_key_pressed(window);

		angle += rate;
		if (angle > 360)
			angle = 0;

		//model matrix
		mat4x4 model;
		model = scale(model, vec3(0.3, 0.3, 0.3));
		model = rotate(model, radians(angle), vec3(1.0, 1.0, 0.0));
		model = translate(model, vec3(-2.0, 1.0, 0.0));
		
		//camera back 
		mat4x4 view;
		view = translate(view, vec3(0.0, 0.0, -5.0));

		//perspective projection
		mat4x4 projection = perspective(radians(45.0), (double)WIDTH / HEIGHT, 0.1, 100.0);

		//draw cube
		m_pipeline.setMVP(model, view, projection);
		m_pipeline.drawArrays(cube, vertex_num, cube_index, index_num);

		//draw nier head
		setIdentity(model);
		model = scale(model, vec3(scale_factor, scale_factor, scale_factor));
		//model = rotate(model, radians(angle), vec3(0.0, 1.0, 0.0));
		m_pipeline.setMVP(model, view, projection);
		m_pipeline.drawArrays(nier);

		window_draw_image(window, &m_pipeline.forwardBuffer);
		input_poll_events();
	}

	window_destroy(window);

	m_pipeline.render();

	return 0;
}

void handle_key_pressed(window_t* window)
{
	if (input_key_pressed(window, KEY_A))
		std::cout << "KEY A Pressed" << std::endl;

	if (input_key_pressed(window, KEY_W))
		std::cout << "KEY W Pressed" << std::endl;

	if (input_key_pressed(window, KEY_S))
		std::cout << "KEY S Pressed" << std::endl;

	if (input_key_pressed(window, KEY_D))
		std::cout << "KEY D Pressed" << std::endl;
}