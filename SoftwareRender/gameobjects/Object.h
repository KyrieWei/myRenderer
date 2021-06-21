#pragma once
#include <vector>
#include "../maths/vec2.h"
#include "../maths/vec3.h"

class Object
{
public:
	std::vector<vec3> vertex;
	std::vector<std::vector<int>> vertex_index;

	int vertex_num, face_num;

	Object() : vertex_num(0), face_num(0) {}
};

