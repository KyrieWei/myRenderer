#pragma once
#include <vector>
#include "../maths/vec3.h"
#include "../maths/vec2.h"



class Model
{
public:
	std::vector<vec3> vertex;
	std::vector<vec2> tex_coord;
	std::vector<vec3> normal;
	std::vector<std::vector<int>> vertex_index;
	std::vector<std::vector<int>> tex_coord_index;
	std::vector<std::vector<int>> normal_index;

	int vertex_num, tex_num, normal_num, face_num;

public:
	Model() : vertex_num(0), tex_num(0), normal_num(0), face_num(0) {}
	void load(const char* str);
};

