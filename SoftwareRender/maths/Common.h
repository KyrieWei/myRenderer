#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4x4.h"

#define PI 3.141592657

double lerp(double u, double v, double t);
vec2 lerp(const vec2& left, const vec2& right, double t);
vec3 lerp(const vec3& left, const vec3& right, double t);
vec4 lerp(const vec4& left, const vec4& right, double t);

vec3 normalize(const vec3& vec);

double radians(double degree);

mat4x4 getTransposeMatrix(const mat4x4& mat);
mat4x4 getInverseMatrix(const mat4x4& mat);


mat4x4 translate(const mat4x4& mat, const vec3& v);
mat4x4 scale(const mat4x4& mat, const vec3& v);
mat4x4 rotate(const mat4x4& mat, double radians, const vec3& pivot);

mat4x4 perspective(double radians, double ratio, double n, double f);

vec2 viewport(const vec3& ndc, int width, int height);