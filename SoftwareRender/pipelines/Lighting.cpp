#include "Lighting.h"
#include "../maths/Common.h"

vec3 compute_flat_normal(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	vec3 p01 = vertex1.positionWS - vertex0.positionWS;
	vec3 p02 = vertex2.positionWS - vertex0.positionWS;

	vec3 norm = cross(p01, p02);

	return normalize(norm);
}

vec3 bling_phong_lighting(const Light& light, const vec3& normal)
{
	vec3 lightDir = normalize(light.direction);
	double fac = dot(-lightDir, normal);

	if (fac < 0.0)
		return vec3(0.0, 0.0, 0.0);

	return light.color * fac;
}