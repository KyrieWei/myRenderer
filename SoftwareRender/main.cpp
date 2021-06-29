#include <iostream>
#include <ctime>
#include <chrono>

#include "pipelines/pipeline.h"
#include "gameobjects/model.h"
#include "components/camera.h"
#include "gameobjects/cube.h"
#include "gameobjects/plane.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;


void handle_key_pressed(window_t* window);


int main()
{
	std::chrono::time_point<std::chrono::system_clock> start, end;

	//create window
	callbacks_t callbacks;
	window_t* window = window_create("software renderer", WIDTH, HEIGHT);
	memset(&callbacks, 0, sizeof(callbacks_t));

	//triangle definition
	vec3 triangle[3] = { {-1, -1, 0}, {1, -1, 0}, {0, 1, 0} };

	//cube
	cube m_cube;
	m_cube.loadTexture("assets/textures/container.jpg", TEXTURE_FILTER::REPEAT);


	//plane
	plane m_plane;
	m_plane.loadTexture("assets/textures/floor.jpg", TEXTURE_FILTER::REPEAT);
	//std::cout << static_cast<unsigned int>(m_plane.texture_data[0]) << " " << m_plane.texture_data[1] << " " << m_plane.texture_data[2] << std::endl;

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
	int rate = 4.0;
	double angle = 0.0;

	//send data to 
	while (!window_should_close(window))
	{

		start = std::chrono::system_clock::now();

		
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

		m_pipeline.bind_texture(m_cube.texture_data, m_cube.texture_width, m_cube.texture_height, m_cube.texture_channel);

		//m_pipeline.drawArrays(m_cube);

		//draw plane
		setIdentity(model);
		model = scale(model, vec3(6.0, 1, 6.0));
		model = translate(model, vec3(0.0, -0.1, -10.0));

		m_pipeline.setShadingMode(shading_mode::GOURAUD_SHADING);
		m_pipeline.setMVP(model, view, projection);

		m_pipeline.bind_texture(m_plane.texture_data, m_plane.texture_width, m_plane.texture_height, m_plane.texture_channel);

		//m_pipeline.drawArrays(m_plane);
		

		window_draw_image(window, &m_pipeline.forwardBuffer);
		input_poll_events();

		//m_pipeline.swapBuffer();

		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		//std::cout << elapsed_seconds.count() << std::endl;
		std::cerr << "\rtriangle numbers: " << m_pipeline.triangle_num_drawed << " fps: " << static_cast<int>(1.0 / elapsed_seconds.count()) << std::flush;
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