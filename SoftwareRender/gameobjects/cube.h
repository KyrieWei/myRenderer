#pragma once
#include "Object.h"
class cube : public Object
{
public:
	std::vector<vec2> tex_coord;
	std::vector<vec3> normal;
	std::vector<vec3> vertex_color;
	std::vector<std::vector<int>> tex_coord_index;
	std::vector<std::vector<int>> normal_index;
	
	int tex_num, normal_num;

public:
	cube();

};

