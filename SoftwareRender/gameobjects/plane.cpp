#include "plane.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"


plane::plane()
{
	vertex =
	{
		{-1, -1, 1},
		{1, -1, 1},
		{1, -1, -1},
		{-1, -1, -1}
	};

	vertex_color =
	{
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0}
	};

	vertex_index =
	{
		{1, 2, 4},
		{2, 3, 4}
	};

	tex_coord =
	{
		{0.0, 0.0},
		{1.0, 0.0},
		{1.0, 1.0},
		{0.0, 1.0}
	};

	tex_coord_index =
	{
		{1, 2, 4},
		{2, 3, 4}
	};

	vertex_num = 4;
	face_num = 2;
}

void plane::loadTexture(const char* str, TEXTURE_FILTER texture_filter)
{
	int width, height, channel;
	texture_data = stbi_load(str, &width, &height, &channel, 0);

	if (!texture_data)
	{
		std::cout << "failed to load texture: " << str << std::endl;
	}
	else
	{
		std::cout << "successed to load texture: " << str << ", width: " << width << ", height: " << height << ", channel: " << channel << std::endl;
	}

	this->texture_filter = texture_filter;
}

plane::~plane()
{
	if (texture_data)
		stbi_image_free(texture_data);
}