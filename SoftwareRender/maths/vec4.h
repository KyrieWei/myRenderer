#pragma once
#include <iostream>
#include "vec3.h"


class vec4
{
public:
	double x, y, z, w;

public:
	vec4() : x(0), y(0), z(0), w(0) {};
	vec4(double elem) : x(elem), y(elem), z(elem), w(elem) {}
	vec4(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4(const vec4& t) : x(t.x), y(t.y), z(t.z), w(t.w) {}
	vec4(const vec3& v, double _w) : x(v.x), y(v.y), z(v.z), w(_w) {}

	double& operator[] (int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		else
			return w;
	}

	double operator[](int i) const
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		else
			return w;
	}

	vec4 operator*(double t) const
	{
		return vec4(x * t, y * t, z * t, w * t);
	}

	vec4 operator/(double t) const
	{
		double fac = 1.0 / t;
		return vec4(x * fac, y * fac, z * fac, w * fac);
	}

	void print(const char* str)
	{
		std::cout << str << ":" << std::endl;
		std::cout << x << " " << y << " " << z << " " << w << std::endl;
		std::cout << std::endl;
	}
};

