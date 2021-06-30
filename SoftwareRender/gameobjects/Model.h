#pragma once
#include "../maths/mat4x4.h"
#include "Mesh.h"
#include "../tools/LoadObj.h"
#include "Material.h"

enum SHADING_TYPE
{
	BLINN,
	PBRM
};

class Model
{
public:
	SHADING_TYPE shading_type;

	std::vector<Material> materials;
	std::vector<mat4x4> transforms;
	std::vector<Mesh> models;

public:
	Model(){}
	Model(const char* str);
	~Model();
};

