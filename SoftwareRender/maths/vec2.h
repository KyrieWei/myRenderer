#pragma once
class vec2
{
public:
	double x, y;

public:
	vec2() : x(0), y(0){}
	vec2(int _x, int _y) : x(_x), y(_y) {}
	vec2(const vec2& t) : x(t.x), y(t.y) {}

	vec2 operator+(const vec2& v) const
	{
		return vec2(x + v.x, y + v.y);
	}

	vec2 operator-(const vec2& v) const
	{
		return vec2(x - v.x, y - v.y);
	}

	vec2 operator*(double t) const
	{
		return vec2(x * t, y * t);
	}

};

