#pragma once
class vec3
{
public:
	double x, y, z;

public:
	vec3() : x(0), y(0), z(0) {};
	vec3(double elem) : x(elem), y(elem), z(elem) {}
	vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	vec3(const vec3& t) : x(t.x), y(t.y), z(t.z) {}

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

};