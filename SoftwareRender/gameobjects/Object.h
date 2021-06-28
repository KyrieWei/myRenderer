#pragma once
#include <vector>
#include "../maths/vec2.h"
#include "../maths/vec3.h"
#include <iostream>


enum TEXTURE_FILTER
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

class Object
{
public:
	std::vector<vec3> vertex;
	std::vector<std::vector<int>> vertex_index;
	std::vector<vec2> tex_coord;
	std::vector<std::vector<int>> tex_coord_index;
	std::vector<vec3> normal;
	std::vector<std::vector<int>> normal_index;

	int vertex_num, face_num, tex_num, normal_num;

	unsigned char* texture_data;
	TEXTURE_FILTER texture_filter;
	int texture_width, texture_height, texture_channel;

public:
	Object() : vertex_num(0), face_num(0) {}

};

