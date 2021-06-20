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

vec3 barycentirc_coord(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p);

double depth_lerp(double vertex0_ndc_z, double vertex1_ndc_z, double vertex2_ndc_z, const vec3& bary);

vec3 normalize(const vec3& vec);
double dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);


double radians(double degree);

void setIdentity(mat4x4& mat);

mat4x4 getTransposeMatrix(const mat4x4& mat);
mat4x4 getInverseMatrix(const mat4x4& mat);


mat4x4 translate(const mat4x4& mat, const vec3& v);
mat4x4 scale(const mat4x4& mat, const vec3& v);
mat4x4 rotate(const mat4x4& mat, double radians, const vec3& pivot);

mat4x4 perspective(double radians, double ratio, double n, double f);

mat4x4 lookAt(const vec3& position, const vec3& target, const vec3& up);

vec2 viewport(const vec3& ndc, int width, int height);