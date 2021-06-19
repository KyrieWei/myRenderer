#pragma once
#include "../maths/vec2.h"
#include "../maths/vec3.h"
#include "../maths/vec4.h"


struct VertexPositionInputs
{
public:
	vec3 positionOS;
	vec3 positionWS;
	vec3 positionVS;
	vec4 positionCS;
	vec4 positionNDC;

	vec2 positionSS;

	vec3 normalWS;
	vec2 uv;
	vec3 texture;
};

