#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../maths/vec2.h"
#include "../maths/vec3.h"
#include "../gameobjects/Mesh.h"

std::vector<std::string> split(const std::string& line, char c);
void loadMesh(const char* str, Mesh& mesh);

