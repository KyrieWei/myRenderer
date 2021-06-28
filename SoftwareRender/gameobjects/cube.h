#pragma once
#include "Object.h"

class cube : public Object
{
public:
	std::vector<vec3> vertex_color;

public:
	cube();
	~cube();
	void loadTexture(const char* str, TEXTURE_FILTER filter);
};

