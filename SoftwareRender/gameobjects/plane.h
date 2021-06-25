#pragma once
#include "Object.h"
class plane : public Object
{
public:
	std::vector<vec3> vertex_color;
	int tex_num, normal_num;

public:
	plane();
	~plane();
	void loadTexture(const char* str, TEXTURE_FILTER texture_filter);
};

