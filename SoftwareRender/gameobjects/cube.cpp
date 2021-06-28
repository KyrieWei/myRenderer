#include "cube.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../tools/stb_image.h"


cube::cube()
{
	vertex = 
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

	vertex_color = 
	{
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{1.0, 0.0, 0.0}
	};

	vertex_index = 
	{
		{1, 2, 3},
		{1, 3, 4}, //back
		{4, 3, 6},
		{4, 6, 5}, //right
		{1, 7, 2},
		{1, 8, 7}, //left
		{2, 6, 3},
		{2, 7, 6}, //up
		{4, 5, 8},
		{1, 4, 8}, //bottom
		{5, 6, 7},
		{5, 7, 8}  //front
	};

	tex_coord =
	{
		{0.0, 0.0},
		{1.0, 0.0},
		{1.0, 1.0},
		{0.0, 1.0},
	};

	tex_coord_index =
	{
		{1, 2, 3},
		{1, 3, 4}, //
		{1, 2, 3},
		{1, 3, 4}, //
		{1, 3, 4},
		{1, 2, 3}, //
		{1, 3, 4},
		{1, 2, 3}, //
		{1, 2, 3},
		{4, 1, 3},
		{1, 2, 3}, //
		{1, 3, 4}
	};

	vertex_num = 8;
	face_num = 12;
}

void cube::loadTexture(const char* str, TEXTURE_FILTER texture_filter)
{
	texture_data = stbi_load(str, &texture_width, &texture_height, &texture_channel, 0);

	if (!texture_data)
	{
		std::cout << "failed to load texture: " << str << std::endl;
	}
	else
	{
		std::cout << "successed to load texture: " << str << ", width: " << texture_width << ", height: " << texture_height << ", channel: " << texture_channel << std::endl;
	}

	this->texture_filter = texture_filter;
}

cube::~cube()
{
	if (texture_data)
		stbi_image_free(texture_data);
}