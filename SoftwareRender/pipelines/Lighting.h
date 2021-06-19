#pragma once
#include "vertexOut.h"
#include "../maths/vec3.h"


struct Light
{
	vec3 direction;
	vec3 position;
	vec3 color;
};


vec3 compute_flat_normal(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);

vec3 bling_phong_lighting(const Light& light, const vec3& normal);

