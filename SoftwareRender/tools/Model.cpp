#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

std::vector<std::string> split(const std::string& line, char c)
{
	std::vector<std::string> vec;
	int start = 0, end = 0;
	while (end != line.length())
	{
		if (line[end] != c)
			end++;
		else
		{
			vec.push_back(line.substr(start, end - start));

			start = end + 1;
			end++;
		}
	}
	if (start < end)
		vec.push_back(line.substr(start, end - start));

	return vec;
}


void Model::load(const char* str)
{
	std::ifstream m_file(str);
	std::string line;
	
	std::vector<std::string> split_line;
	std::vector<std::string> split_f;

	if (m_file.is_open())
	{
		while (getline(m_file, line))
		{
			split_line = split(line, ' ');

			if (split_line[0] == "v")
			{
				vec3 elem = vec3(stod(split_line[1]), stod(split_line[2]), stod(split_line[3]));
				vertex.push_back(elem);

				vertex_num++;
			}
			else if(split_line[0] == "vt")
			{
				vec2 elem = vec2(stod(split_line[1]), stod(split_line[2]));
				tex_coord.push_back(elem);

				tex_num++;
			}
			else if (split_line[0] == "vn")
			{
				vec3 elem = vec3(stod(split_line[1]), stod(split_line[2]), stod(split_line[3]));
				normal.push_back(elem);

				normal_num++;
			}
			else if (split_line[0] == "f")
			{
				if (!split_line[1].find('/'))
				{
					std::vector<int> elem = { stoi(split_line[1]), stoi(split_line[2]), stoi(split_line[3]) };
					vertex_index.push_back(elem);
					tex_coord_index.push_back(elem);
					normal_index.push_back(elem);
				}
				else
				{
					std::vector<int> v, t, n;
					for (int i = 1; i < split_line.size(); i++)
					{
						split_f = split(split_line[i], '/');
						v.push_back(stoi(split_f[0]));
						t.push_back(stoi(split_f[1]));
						n.push_back(stoi(split_f[2]));
					}

					vertex_index.push_back(v);
					tex_coord_index.push_back(t);
					normal_index.push_back(n);
				}

				face_num++;
			}
			else
			{

			}
		}

		m_file.close();
	}

	else
	{
		std::cout << "unable to open file " << str << std::endl;
	}
}
