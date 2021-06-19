#pragma once
//#include "vec4.h"

class vec4;

class vec3
{
public:
	double x, y, z;

public:
	vec3() : x(0), y(0), z(0) {};
	vec3(double elem) : x(elem), y(elem), z(elem) {}
	vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	vec3(const vec3& t) : x(t.x), y(t.y), z(t.z) {}
	vec3(const vec4& v);

	double& operator[] (int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else
			return z;
	}

	double operator[](int i) const
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else
			return z;
	}

	vec3 operator+(const vec3& v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	vec3 operator-(const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator*(double t) const
	{
		return vec3(x * t, y * t, z * t);
	}

	vec3 operator/(double t) const
	{
		double fac = 1.0 / t;
		return vec3(x * fac, y * fac, z * fac);
	}

	vec3 operator-() const
	{
		return vec3(-x, -y, -z);
	}

	friend vec3 operator*(double t, const vec3& v)
	{
		return v * t;
	}

};