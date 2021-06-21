#include <iostream>
#include <ctime>

#include "pipelines/pipeline.h"
#include "tools/model.h"
#include "components/camera.h"
#include "gameobjects/cube.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;

int rate = 4.0;
double angle = 0.0;


void handle_key_pressed(window_t* window);

int main()
{

	//create window
	callbacks_t callbacks;
	window_t* window = window_create("software renderer", WIDTH, HEIGHT);
	memset(&callbacks, 0, sizeof(callbacks_t));

	//triangle definition
	vec3 triangle[3] = { {-1, -1, 0}, {1, -1, 0}, {0, 1, 0} };

	//cube
	cube m_cube;

	vec3 plane[4] =
	{
		{-1, -1, 1},
		{1, -1, 1},
		{1, -1, -1},
		{-1, -1, -1}
	};

	int plane_vertex_num = 4;

	int plane_index[2][3] =
	{
		{0, 1, 3},
		{1, 2, 3}
	};

	int plane_index_num = 2;

	vec3 plane_color[4] =
	{
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0}
	};

	//Load model
	Model nier;
	//nier.load("assets/nier2b7.obj");

	std::cout << nier.vertex_num << std::endl;
	std::cout << nier.tex_num << std::endl;
	std::cout << nier.face_num << std::endl;

	pipeline m_pipeline(WIDTH, HEIGHT);

	vec4 bg_color = vec4(0.1, 0.3, 0.4, 1.0);
	m_pipeline.clean_color(bg_color);

	//pipeline setting

	//Light
	Light sun;
	sun.direction = vec3(-1.0, -1.0, -1.0);
	sun.color = vec3(1.0, 1.0, 1.0);

	//camera
	vec3 camera_pos = vec3(0.0, 8.0, 5.0);
	camera cam(camera_pos, vec3(0.0, 1.0, 0.0));

	double scale_factor = 1.0;

	//send data to 
	while (!window_should_close(window))
	{
		m_pipeline.clean_color(bg_color);
		m_pipeline.clean_depth();
		m_pipeline.triangle_num_drawed = 0;

		angle += rate;
		if (angle > 360)
			angle = 0;

		//model matrix
		mat4x4 model;
		model = scale(model, vec3(1.0, 1.0, 1.0));
		model = rotate(model, radians(angle), vec3(0.0, 1.0, 0.0));
		model = translate(model, vec3(0.0, -0.0, -10.0));

		//camera back 
		mat4x4 view;
		view = cam.GetViewMatrix();

		//perspective projection
		mat4x4 projection = perspective(radians(cam.zoom), (double)WIDTH / HEIGHT, 0.1, 100.0);

		//draw cube
		m_pipeline.setShadingMode(shading_mode::GOURAUD_SHADING);
		m_pipeline.setMVP(model, view, projection);
		m_pipeline.setLight(sun);
		m_pipeline.setCamera(cam.Position);
		m_pipeline.drawArrays(m_cube);

		//draw plane
		setIdentity(model);
		model = scale(model, vec3(6.0, 1, 6.0));
		model = translate(model, vec3(0.0, -0.1, -10.0));

		m_pipeline.setShadingMode(shading_mode::FLAT_SHADING);
		m_pipeline.setMVP(model, view, projection);

		m_pipeline.drawArrays(plane, plane_vertex_num, plane_index, plane_index_num, plane_color);

		window_draw_image(window, &m_pipeline.forwardBuffer);
		input_poll_events();

		std::cerr << "\rtriangle numbers : " << m_pipeline.triangle_num_drawed << std::flush;
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