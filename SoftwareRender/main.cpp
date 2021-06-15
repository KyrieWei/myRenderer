#include <iostream>

#include "pipelines/pipeline.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int CHANNEL = 4;

const char* file_name = "results/test.jpg";


int main()
{

	pipeline m_pipeline(WIDTH, HEIGHT);
	m_pipeline.render();

	//mat4x4 mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//mat4x4 mat_2(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);

	return 0;
}