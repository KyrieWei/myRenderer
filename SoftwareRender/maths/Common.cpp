#include "Common.h"

double lerp(double u, double v, double t)
{
	return u * (1 - t) + v * t;
}

vec2 lerp(const vec2& left, const vec2& right, double t)
{
	vec2 res;
	res.x = left.x * (1 - t) + right.x * t;
	res.y = left.y * (1 - t) + right.y * t;

	return res;
}

vec3 lerp(const vec3& left, const vec3& right, double t)
{
	vec3 res;
	res.x = left.x * (1 - t) + right.x * t;
	res.y = left.y * (1 - t) + right.y * t;
	res.z = left.z * (1 - t) + right.z * t;

	return res;
}

vec4 lerp(const vec4& left, const vec4& right, double t)
{
	vec4 res;
	res.x = left.x * (1 - t) + right.x * t;
	res.y = left.y * (1 - t) + right.y * t;
	res.z = left.z * (1 - t) + right.z * t;
	res.w = left.w * (1 - t) + right.w * t;

	return res;
}