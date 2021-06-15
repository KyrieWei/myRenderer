#pragma once
#include "vec3.h"
#include "vec4.h"

// 0,  1,  2,  3
// 4,  5,  6,  7
// 8,  9,  10, 11
// 12, 13, 14, 15

class mat4x4
{
public:
	double data[16];

public:
	mat4x4();
	mat4x4(double e0, double e1, double e2, double e3,
		   double e4, double e5, double e6, double e7,
		   double e8, double e9, double e10, double e11,
		   double e12, double e13, double e14, double e15);

	mat4x4(const mat4x4& mat);

	mat4x4 operator+(const mat4x4& mat) const;
	mat4x4 operator-(const mat4x4& mat) const;
	mat4x4 operator*(const mat4x4& mat) const;

	mat4x4 operator*(double t) const;
	mat4x4 operator/(double t) const;

	vec4 operator*(const vec4& v) const;

	void print(const char* str);
};

