#pragma once
#include <vector>
#include "../maths/vec2.h"
#include "../maths/vec3.h"

class Mesh
{
public:
	std::vector<vec3> vertex;
	std::vector<std::vector<int>> vertex_index;
	std::vector<vec2> tex_coord;
	std::vector<std::vector<int>> tex_coord_index;
	std::vector<vec3> normal;
	std::vector<std::vector<int>> normal_index;

	int vertex_num, tri_num, tex_num, normal_num, material_index, transform_index;

public:
	void load();
};

