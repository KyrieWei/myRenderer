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

vec3 normalize(const vec3& vec)
{
	double length = 1.0 / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return vec * length;
}


double radians(double degree)
{
	return (degree * PI) / 180.0;
}

void setIdentity(mat4x4& mat)
{
	mat.data[0] = 1.0;
	mat.data[1] = 0.0;
	mat.data[2] = 0.0;
	mat.data[3] = 0.0;
	mat.data[4] = 0.0;
	mat.data[5] = 1.0;
	mat.data[6] = 0.0;
	mat.data[7] = 0.0;
	mat.data[8] = 0.0;
	mat.data[9] = 0.0;
	mat.data[10] = 1.0;
	mat.data[11] = 0.0;
	mat.data[12] = 0.0;
	mat.data[13] = 0.0;
	mat.data[14] = 0.0;
	mat.data[15] = 1.0;
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
	mat_scale.data[10] *= v.z;

	return mat_scale;
}

mat4x4 rotate(const mat4x4& mat, double radians, const vec3& pivot)
{
	mat4x4 mat_rotate;

	vec3 A = normalize(pivot);
	double c = cos(radians);
	double s = sin(radians);

	mat_rotate.data[0] = c + (1 - c) * A.x * A.x;
	mat_rotate.data[1] = (1 - c) * A.x * A.y - s * A.z;
	mat_rotate.data[2] = (1 - c) * A.x * A.z + s * A.y;
	mat_rotate.data[4] = (1 - c) * A.x * A.y + s * A.z;
	mat_rotate.data[5] = c + (1 - c) * A.y * A.y;
	mat_rotate.data[6] = (1 - c) * A.y * A.z - s * A.x;
	mat_rotate.data[8] = (1 - c) * A.x * A.z - s * A.y;
	mat_rotate.data[9] = (1 - c) * A.y * A.z + s * A.x;
	mat_rotate.data[10] = c + (1 - c) * A.z * A.z;

	return mat_rotate * mat;
}

mat4x4 perspective(double radians, double ratio, double n, double f)
{
	assert(radians != 0 && ratio != 0 && (f-n) != 0);
	mat4x4 pers;

	pers.data[0] = 1.0 / (ratio * tan(radians / 2));
	pers.data[5] = 1.0 / tan(radians / 2);
	pers.data[10] = -(f + n) / (f - n);
	pers.data[11] = -2 * n * f / (f - n);
	pers.data[14] = -1;
	pers.data[15] = 0;

	return pers;
}

vec2 viewport(const vec3& ndc, int width, int height)
{
	vec2 res;
	res.x = static_cast<int>((ndc.x + 1) / 2 * width);
	res.y = static_cast<int>((ndc.y + 1) / 2 * height);

	return res;
}
