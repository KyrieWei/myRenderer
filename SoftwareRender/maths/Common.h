#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

double lerp(double u, double v, double t);
vec2 lerp(const vec2& left, const vec2& right, double t);
vec3 lerp(const vec3& left, const vec3& right, double t);
vec4 lerp(const vec4& left, const vec4& right, double t);