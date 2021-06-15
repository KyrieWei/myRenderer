#include "mat4x4.h"
#include <iostream>
#include <assert.h>

// 0,  1,  2,  3
// 4,  5,  6,  7
// 8,  9,  10, 11
// 12, 13, 14, 15

mat4x4::mat4x4()
{
	for (int i = 0; i < 16; i++)
		data[i] = 0;

	data[0] = 1.0;
	data[5] = 1.0;
	data[10] = 1.0;
	data[15] = 1.0;
}

mat4x4::mat4x4(double e0, double e1, double e2, double e3,
			   double e4, double e5, double e6, double e7,
			   double e8, double e9, double e10, double e11,
			   double e12, double e13, double e14, double e15)
{
	data[0] = e0;
	data[1] = e1;
	data[2] = e2;
	data[3] = e3;
	data[4] = e4;
	data[5] = e5;
	data[6] = e6;
	data[7] = e7;
	data[8] = e8;
	data[9] = e9;
	data[10] = e10;
	data[11] = e11;
	data[12] = e12;
	data[13] = e13;
	data[14] = e14;
	data[15] = e15;
}

mat4x4::mat4x4(const mat4x4& mat)
{
	for (int i = 0; i < 16; i++)
		data[i] = mat.data[i];
}

mat4x4 mat4x4::operator+(const mat4x4& mat) const
{
	mat4x4 res;
	for (int i = 0; i < 16; i++)
		res.data[i] = data[i] + mat.data[i];

	return res;
}

mat4x4 mat4x4::operator-(const mat4x4& mat) const
{
	mat4x4 res;
	for (int i = 0; i < 16; i++)
		res.data[i] = data[i] - mat.data[i];

	return res;
}

mat4x4 mat4x4::operator*(const mat4x4& mat) const
{
	mat4x4 res;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			res.data[i * 4 + j] = data[i * 4] * mat.data[j] + data[i * 4 + 1] * mat.data[j + 4] + data[i * 4 + 2] * mat.data[j + 8] + data[i * 4 + 3] * mat.data[j + 12];
	
	return res;
}

mat4x4 mat4x4::operator*(double t) const
{
	mat4x4 res;
	for (int i = 0; i < 16; i++)
		res.data[i] = data[i] * t;

	return res;
}

mat4x4 mat4x4::operator/(double t) const
{
	assert(t != 0);

	mat4x4 res;
	double fac = 1.0 / t;
	
	for (int i = 0; i < 16; i++)
		res.data[i] = data[i] * fac;

	return res;
}


vec4 mat4x4::operator*(const vec4& v) const
{
	vec4 res;

	res.x = data[0] * v.x + data[1] * v.y + data[2] * v.z + data[3] * v.w;
	res.y = data[4] * v.x + data[5] * v.y + data[6] * v.z + data[7] * v.w;
	res.z = data[8] * v.x + data[9] * v.y + data[10] * v.z + data[11] * v.w;
	res.w = data[12] * v.x + data[13] * v.y + data[14] * v.z + data[15] * v.w;

	return res;
}


void mat4x4::print(const char* str)
{
	std::cout << str << ":" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << data[i * 4 + j] << " ";

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

