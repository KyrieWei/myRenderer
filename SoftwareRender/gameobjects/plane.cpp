#include "plane.h"


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
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

plane::~plane()
{
	if (texture_data)
		stbi_image_free(texture_data);
}