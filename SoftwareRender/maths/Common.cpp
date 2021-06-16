#include "Common.h"
#include <assert.h>
#include <cmath>

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

double radians(double degree)
{
	return (degree * PI) / 180.0;
}

mat4x4 getTransposeMatrix(const mat4x4& mat)
{
	mat4x4 mat_trans;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			mat_trans.data[i * 4 + j] = mat.data[j * 4 + i];
	}

	return mat_trans;
}


mat4x4 translate(const mat4x4& mat, const vec3& v)
{
	mat4x4 mat_trans(mat);

	mat_trans.data[3] += v.x;
	mat_trans.data[7] += v.y;
	mat_trans.data[11] += v.z;

	return mat_trans;
}

mat4x4 scale(const mat4x4& mat, const vec3& v)
{
	mat4x4 mat_scale(mat);

	mat_scale.data[0] *= v.x;
	mat_scale.data[5] *= v.y;
	mat_scale.data[11] *= v.z;

	return mat_scale;
}

mat4x4 rotate(const mat4x4& mat, double radians, const vec3& pivot)
{
	mat4x4 mat_rotate;


	return mat_rotate;
}

mat4x4 perspective(double radians, double ratio, double n, double f)
{
	assert(radians != 0 && ratio != 0 && (f-n) != 0);
	mat4x4 pers;

	pers.data[0] = 1.0 / (ratio * tan(radians / 2));
	pers.data[5] = tan(radians / 2);
	pers.data[10] = -(f + n) / (f - n);
	pers.data[11] = -2 * n * f / (f - n);
	pers.data[14] = -1;
	pers.data[15] = 0;

	return pers;
}

vec2 viewport(const vec3& ndc, int width, int height)
{
	vec2 res;
	res.x = (ndc.x + 1) / 2 * width;
	res.y = (ndc.y + 1) / 2 * height;

	return res;
}
