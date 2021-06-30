#pragma once
#include <vector>
#include "../maths/vec4.h"

class Material
{
public:
	vec4 baseColor;
	float shininess;

	unsigned char* diffuse_map;
	unsigned char* specular_map;
	unsigned char* emission_map;

	bool double_sided;
	bool enable_blend;
	float alpha_cutoff;

	int diffuse_map_width;
	int diffuse_map_height;
	int diffuse_map_channel;
	int specular_map_width;
	int specular_map_height;
	int specular_map_channel;
	int emission_map_width;
	int emission_map_height;
	int emission_map_channel;

public:
	~Material();
};

