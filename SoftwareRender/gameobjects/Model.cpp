#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../tools/stb_image.h"


Model::Model(const char* str)
{
	std::ifstream m_file(str);
	std::string line;

	if (m_file.is_open())
	{
		std::vector<std::string> split_line;
		//type
		std::getline(m_file, line);

		//blinn
		if (line == "type: blinn")
		{
			std::getline(m_file, line);

			//lighting:
			std::getline(m_file, line); 
			std::getline(m_file, line);
			std::getline(m_file, line);
			std::getline(m_file, line);
			std::getline(m_file, line);
			std::getline(m_file, line);
			std::getline(m_file, line);

			std::getline(m_file, line);

			std::cout << "loading materials ...";
			//materials
			std::getline(m_file, line);
			split_line = split(line.substr(0, line.length() - 1), ' ');
			int mat_num = std::stoi(split_line[1]);

			for (int i = 0; i < mat_num; i++)
			{
				//material i:
				std::getline(m_file, line);

				Material mat;
				//basecolor
				std::getline(m_file, line);
				split_line = split(line, ' ');
				mat.baseColor = { std::stod(split_line[1]), std::stod(split_line[2]), std::stod(split_line[3]), std::stod(split_line[4]) };
				//shininess
				std::getline(m_file, line);
				split_line = split(line, ' ');
				mat.shininess = std::stof(split_line[1]);
				//diffuse_map
				std::getline(m_file, line);
				split_line = split(line, ' ');
				if (split_line[1] != "null")
				{
					split_line[1] = "assets/" + split_line[1];
					mat.diffuse_map = stbi_load(split_line[1].c_str(), &mat.diffuse_map_width, &mat.diffuse_map_height, &mat.diffuse_map_channel, 0);
					if (!mat.diffuse_map)
					{
						std::cout << "failed to load texture: " << split_line[1] << std::endl;
					}
				}
				else
				{
					mat.diffuse_map = nullptr;
					mat.diffuse_map_width = 0;
					mat.diffuse_map_height = 0;
					mat.diffuse_map_channel = 0;
				}
				//specular_map
				std::getline(m_file, line);
				split_line = split(line, ' ');
				if (split_line[1] != "null")
				{
					split_line[1] = "assets/" + split_line[1];
					mat.specular_map = stbi_load(split_line[1].c_str(), &mat.specular_map_width, &mat.specular_map_height, &mat.specular_map_channel, 0);
					if (!mat.specular_map)
					{
						std::cout << "failed to load texture: " << split_line[1] << std::endl;
					}
				}
				else
				{
					mat.specular_map = nullptr;
					mat.specular_map_width = 0;
					mat.specular_map_height = 0;
					mat.specular_map_channel = 0;
				}
				//emission_map
				std::getline(m_file, line);
				split_line = split(line, ' ');
				if (split_line[1] != "null")
				{
					split_line[1] = "assets/" + split_line[1];
					mat.emission_map = stbi_load(split_line[1].c_str(), &mat.emission_map_width, &mat.emission_map_height, &mat.emission_map_channel, 0);
					if (!mat.emission_map)
					{
						std::cout << "failed to load texture: " << split_line[1] << std::endl;
					}
				}
				else
				{
					mat.emission_map = nullptr;
					mat.emission_map_width = 0;
					mat.emission_map_height = 0;
					mat.emission_map_channel = 0;
				}
				//double_sided
				std::getline(m_file, line);
				split_line = split(line, ' ');
				if (split_line[1] == "off")
					mat.double_sided = false;
				else
					mat.double_sided = true;
				//enable_blend
				std::getline(m_file, line);
				split_line = split(line, ' ');
				if (split_line[1] == "off")
					mat.enable_blend = false;
				else
					mat.enable_blend = true;
				//alpha_cutoff
				std::getline(m_file, line);
				split_line = split(line, ' ');
				mat.alpha_cutoff = std::stof(split_line[1]);
				
				materials.push_back(mat);
			}
			std::cout << " finished" << std::endl;

			std::getline(m_file, line);

			//transforms
			std::getline(m_file, line);
			split_line = split(line.substr(0, line.length() - 1), ' ');
			int trans_num = std::stoi(split_line[1]);

			for (int i = 0; i < trans_num; i++)
			{
				//transform i:
				std::getline(m_file, line);

				mat4x4 trans;
				std::getline(m_file, line);
				split_line = split(line, ' ');
				trans.data[0] = std::stod(split_line[0]);
				trans.data[1] = std::stod(split_line[1]);
				trans.data[2] = std::stod(split_line[2]);
				trans.data[3] = std::stod(split_line[3]);

				std::getline(m_file, line);
				split_line = split(line, ' ');
				trans.data[4] = std::stod(split_line[0]);
				trans.data[5] = std::stod(split_line[1]);
				trans.data[6] = std::stod(split_line[2]);
				trans.data[7] = std::stod(split_line[3]);

				std::getline(m_file, line);
				split_line = split(line, ' ');
				trans.data[8] = std::stod(split_line[0]);
				trans.data[9] = std::stod(split_line[1]);
				trans.data[10] = std::stod(split_line[2]);
				trans.data[11] = std::stod(split_line[3]);

				std::getline(m_file, line);
				split_line = split(line, ' ');
				trans.data[12] = std::stod(split_line[0]);
				trans.data[13] = std::stod(split_line[1]);
				trans.data[14] = std::stod(split_line[2]);
				trans.data[15] = std::stod(split_line[3]);

				transforms.push_back(trans);
			}

			std::getline(m_file, line);

			std::cout << "loading models ...";
			//models
			std::getline(m_file, line);
			split_line = split(line.substr(0, line.length() - 1), ' ');
			int model_num = std::stoi(split_line[1]);

			for (int i = 0; i < model_num; i++)
			{
				std::getline(m_file, line);

				Mesh mesh;
				
				//mesh
				std::getline(m_file, line);
				split_line = split(line, ' ');
				split_line[1] = "assets/" + split_line[1];
				loadMesh(split_line[1].c_str(), mesh);
				//skeleton
				std::getline(m_file, line);
				//attached
				std::getline(m_file, line);
				//material index
				std::getline(m_file, line);
				split_line = split(line, ' ');
				int mat_index = std::stoi(split_line[1]);
				mesh.material_index = mat_index;
				//transform index
				std::getline(m_file, line);
				split_line = split(line, ' ');
				int trans_index = std::stoi(split_line[1]);
				mesh.transform_index = trans_index;

				for (auto &elem : mesh.vertex)
					elem = transforms[trans_index] * elem;

				models.push_back(mesh);
			}

			std::cout << " finished" << std::endl;
		}
		else //pbrm
		{

		}

		m_file.close();
	}
}

Model::~Model()
{
	for (auto elem : materials)
	{
		if (elem.diffuse_map)
			delete[] elem.diffuse_map;
		if (elem.specular_map)
			delete[] elem.specular_map;
		if (elem.emission_map)
			delete[] elem.emission_map;

	}
}
