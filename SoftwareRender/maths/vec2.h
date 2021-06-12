#pragma once
class vec2
{
public:
	double x, y;

public:
	vec2() : x(0), y(0){}
	vec2(int _x, int _y) : x(_x), y(_y) {}
	vec2(const vec2& t) : x(t.x), y(t.y) {}

};

