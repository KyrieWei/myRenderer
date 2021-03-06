#include "pipeline.h"

pipeline::pipeline(int width, int height): screenWidth(width), screenHeight(height), forwardBuffer(screenWidth, screenHeight, 4), backwardBuffer(screenWidth, screenHeight, 4)
{
	depthBuffer = new double[width * height];
}

pipeline::~pipeline()
{
	delete[] depthBuffer;
}

void pipeline::setMVP(const mat4x4& model, const mat4x4& view, const mat4x4& projection)
{
	this->model = model;
	this->view = view;
	this->projection = projection;
}

void pipeline::setLight(const Light& light)
{
	this->light = light;
}

void pipeline::setCamera(const vec3& cam)
{
	this->camera_pos = cam;
}

void pipeline::setShadingMode(shading_mode mode)
{
	shadingMode = mode;
}

void pipeline::compute_vertex_WVC(VertexPositionInputs& vertex)
{
	vertex.positionWS = model * vec4(vertex.positionOS, 1.0);
	vertex.positionVS = view * vec4(vertex.positionWS, 1.0);
	vertex.positionCS = projection * vec4(vertex.positionVS, 1.0);

	//vertex.normalWS = model * vec4(vertex.normalWS, 1.0);
}

void pipeline::compute_vertex_NS(VertexPositionInputs& vertex)
{
	vertex.positionNDC = vertex.positionCS / vertex.positionCS.w;
	vertex.positionSS = viewport(vertex.positionNDC, screenWidth, screenHeight);
}



void pipeline::fragment_shader(VertexPositionInputs& vertex)
{
	//TODO:compute lighting
}

bool pipeline::face_culling_wo_normal(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	vec3 view_dir = normalize(vertex0.positionWS - camera_pos);
	vec3 normal = compute_flat_normal(vertex0, vertex1, vertex2);

	if (dot(view_dir, normal) < 0.0)
		return false;
	else
		return true;
}

bool pipeline::face_culling_with_normal(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	vec3 view_dir = normalize(vertex0.positionWS - camera_pos);
	vec3 normal = normalize(vertex0.normalWS + vertex1.normalWS + vertex2.normalWS);

	if (dot(view_dir, normal) < 0.0)
		return false;
	else
		return true;
}

bool pipeline::triangle_clip(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	double abs_w = abs(vertex0.positionCS.w);
	if (abs(vertex0.positionCS.x) > abs_w || abs(vertex0.positionCS.y) > abs_w || abs(vertex0.positionCS.z) > abs_w)
		return true;
	abs_w = abs(vertex1.positionCS.w);
	if (abs(vertex1.positionCS.x) > abs_w || abs(vertex1.positionCS.y) > abs_w || abs(vertex1.positionCS.z) > abs_w)
		return true;
	abs_w = abs(vertex2.positionCS.w);
	if (abs(vertex2.positionCS.x) > abs_w || abs(vertex2.positionCS.y) > abs_w || abs(vertex2.positionCS.z) > abs_w)
		return true;

	return false;
}

void pipeline::depth_persp(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{

}

void pipeline::depth_write(int i, int j, double depth)
{
	int index = (screenHeight - j - 1) * screenWidth + i;
	depthBuffer[index] = depth;
}

bool pipeline::depth_test(int i, int j, double depth)
{
	int index = (screenHeight - j - 1) * screenWidth + i;
	if (depth < depthBuffer[index])
		return true;
	else
		return false;
}

void pipeline::bind_texture(unsigned char* data, int width, int height, int channel)
{
	texture_data = data;
	texture_width = width;
	texture_height = height;
	texture_channle = channel;
}

vec4 pipeline::sample_texture(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2, const vec3& bary_coord)
{
	double z = 1.0 / (bary_coord.x / vertex0.positionVS.z + bary_coord.y / vertex1.positionVS.z + bary_coord.z / vertex2.positionVS.z);

	vec2 uv = (vertex0.uv / vertex0.positionVS.z * bary_coord.x + vertex1.uv / vertex1.positionVS.z * bary_coord.y + vertex2.uv / vertex2.positionVS.z * bary_coord.z) * z;

	uv.x = uv.x - (double)floor(uv.x);
	uv.y = uv.y - (double)floor(uv.y);

	uv.y = 1 - uv.y;
	//uv.x = clamp(uv.x, 0.0, 1.0);
	//uv.y = clamp(uv.y, 0.0, 1.0);

	int width = static_cast<int>(uv.x * (texture_width - 1));
	int height = static_cast<int>(uv.y * (texture_height - 1));

	int index = (texture_width * height + width) * texture_channle;
	
	double r = static_cast<unsigned int>(texture_data[index]) / 255.0;
	double g = static_cast<unsigned int>(texture_data[index + 1]) / 255.0;
	double b = static_cast<unsigned int>(texture_data[index + 2]) / 255.0;

	return vec4(r, g, b, 1.0);
}

vec4 pipeline::texture_repeat_sample(const vec2& texcoord)
{
	double v = texcoord.x - (double)floor(texcoord.x);
	double u = texcoord.y - (double)floor(texcoord.y);

	int col = (int)((texture_width - 1) * u);
	int row = (int)((texture_height - 1) * v);

	int index = (row * texture_width + col) * texture_channle;

	double r = static_cast<unsigned int>(texture_data[index]) / 255.0;
	double g = static_cast<unsigned int>(texture_data[index + 1]) / 255.0;
	double b = static_cast<unsigned int>(texture_data[index + 2]) / 255.0;

	double w = 1.0;
	if (texture_channle == 4)
		w = static_cast<unsigned int>(texture_data[index + 3]) / 255.0;

	return vec4(r, g, b, w);
}



void pipeline::flat_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{

	vec3 normal = compute_flat_normal(vertex0, vertex1, vertex2);
	vec4 color = vec4(bling_phong_lighting(light, normal), 1.0);

	int total_height = vertex2.positionSS.y - vertex0.positionSS.y;

	for (int i = 0; i < total_height; i++)
	{
		bool second_half = i > vertex1.positionSS.y - vertex0.positionSS.y || vertex1.positionSS.y == vertex0.positionSS.y;
		int segment_height = second_half ? vertex2.positionSS.y - vertex1.positionSS.y : vertex1.positionSS.y - vertex0.positionSS.y;
		double alpha = (double)i / total_height;
		double beta = (double)(i - (second_half ? vertex1.positionSS.y - vertex0.positionSS.y : 0)) / segment_height;
		vec2 A = vertex0.positionSS + (vertex2.positionSS - vertex0.positionSS) * alpha;
		vec2 B = second_half ? vertex1.positionSS + (vertex2.positionSS - vertex1.positionSS) * beta : vertex0.positionSS + (vertex1.positionSS - vertex0.positionSS) * beta;

		if (A.x > B.x) std::swap(A, B);
		int k = vertex0.positionSS.y + i;
		for (int j = A.x; j <= B.x; j++)
		{
			vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, k));

			double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

			if (depth_test(j, k, depth))
			{
				depth_write(j, k, depth);			
				forwardBuffer.draw_pixel(j, k, color);
			}
		}
	}
}

void pipeline::gouraud_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	
	int total_height = vertex2.positionSS.y - vertex0.positionSS.y;

	for (int i = 0; i < total_height; i++)
	{
		bool second_half = i > vertex1.positionSS.y - vertex0.positionSS.y || vertex1.positionSS.y == vertex0.positionSS.y;
		int segment_height = second_half ? vertex2.positionSS.y - vertex1.positionSS.y : vertex1.positionSS.y - vertex0.positionSS.y;
		double alpha = (double)i / total_height;
		double beta = (double)(i - (second_half ? vertex1.positionSS.y - vertex0.positionSS.y : 0)) / segment_height;
		vec2 A = vertex0.positionSS + (vertex2.positionSS - vertex0.positionSS) * alpha;
		vec2 B = second_half ? vertex1.positionSS + (vertex2.positionSS - vertex1.positionSS) * beta : vertex0.positionSS + (vertex1.positionSS - vertex0.positionSS) * beta;

		if (A.x > B.x) std::swap(A, B);
		int k = vertex0.positionSS.y + i;
		for (int j = A.x; j <= B.x; j++)
		{
			vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, k));

			double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

			if (depth_test(j, k, depth))
			{
				depth_write(j, k, depth);
				
				//use base color
				vec4 color;
				if (!texture_status)
				{
					color = current_material->baseColor;
				}
				else
				{
					color = sample_texture(vertex0, vertex1, vertex2, bary_coord);
				}
				
				forwardBuffer.draw_pixel(j, k, color);
			}
		}
	}

}

void pipeline::blingPhong_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{

}

void pipeline::rasterization(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	//degenerate triangles
	if (vertex0.positionSS.y == vertex1.positionSS.y && vertex0.positionSS.y == vertex2.positionSS.y)
		return;

	//draw_triangle
	VertexPositionInputs tmp_0(vertex0);
	VertexPositionInputs tmp_1(vertex1);
	VertexPositionInputs tmp_2(vertex2);

	//make sure tmp_2.y > tmp_1.y > tmp_0.y
	if (tmp_0.positionSS.y > tmp_1.positionSS.y) std::swap(tmp_0, tmp_1);
	if (tmp_0.positionSS.y > tmp_2.positionSS.y) std::swap(tmp_0, tmp_2);
	if (tmp_1.positionSS.y > tmp_2.positionSS.y) std::swap(tmp_1, tmp_2);
	 
	switch (shadingMode)
	{
	case FLAT_SHADING:
		flat_shading(tmp_0, tmp_1, tmp_2);
		break;
	case GOURAUD_SHADING:
		gouraud_shading(tmp_0, tmp_1, tmp_2);
		break;
	case BLINGPHONG_SHADING:
		blingPhong_shading(tmp_0, tmp_1, tmp_2);
		break;
	default:
		break;
	}

}

void pipeline::drawArrays(vec3 data[], int data_length, int index[][3], int index_length, vec3 data_color[])
{
	for (int i = 0; i < index_length; i++)
	{
		VertexPositionInputs vertex0, vertex1, vertex2;
		vertex0.positionOS = data[index[i][0]];
		vertex1.positionOS = data[index[i][1]];
		vertex2.positionOS = data[index[i][2]];

		vertex0.texture = data_color[index[i][0]];
		vertex1.texture = data_color[index[i][1]];
		vertex2.texture = data_color[index[i][2]];

		//vertex shader
		compute_vertex_WVC(vertex0);
		compute_vertex_WVC(vertex1);
		compute_vertex_WVC(vertex2);

		//face culling
		if (face_culling_wo_normal(vertex0, vertex1, vertex2))
			continue;

		//triangle clip
		if (triangle_clip(vertex0, vertex1, vertex2))
			continue;

		compute_vertex_NS(vertex0);
		compute_vertex_NS(vertex1);
		compute_vertex_NS(vertex2);

		//fragment shader
		
		//rasterization
		rasterization(vertex0, vertex1, vertex2);

		triangle_num_drawed++;
	}
}

void pipeline::drawArrays(const Object& obj)
{

	for (int i = 0; i < obj.face_num; i++)
	{
		VertexPositionInputs vertex0, vertex1, vertex2;

		vertex0.positionOS = obj.vertex[obj.vertex_index[i][0] - 1];
		vertex1.positionOS = obj.vertex[obj.vertex_index[i][1] - 1];
		vertex2.positionOS = obj.vertex[obj.vertex_index[i][2] - 1];

		if (!obj.tex_coord.empty())
		{
			vertex0.uv = obj.tex_coord[obj.tex_coord_index[i][0] - 1];
			vertex1.uv = obj.tex_coord[obj.tex_coord_index[i][1] - 1];
			vertex2.uv = obj.tex_coord[obj.tex_coord_index[i][2] - 1];
		}

		//vertex shader
		compute_vertex_WVC(vertex0);
		compute_vertex_WVC(vertex1);
		compute_vertex_WVC(vertex2);

		//face culling
		if (face_culling_wo_normal(vertex0, vertex1, vertex2))
			continue;

		//triangle clip
		if (triangle_clip(vertex0, vertex1, vertex2))
			continue;

		compute_vertex_NS(vertex0);
		compute_vertex_NS(vertex1);
		compute_vertex_NS(vertex2);


		//rasterization
		rasterization(vertex0, vertex1, vertex2);

		triangle_num_drawed++;
	}
}

void pipeline::drawArrays(const Model& mod)
{
	//for (auto mesh : mod.models)
	{
		auto mesh = mod.models[7];
		//std::cout << mesh.tri_num << std::endl;
		//bind texture
		current_material = &mod.materials[mesh.material_index];
		if (current_material->diffuse_map == nullptr)
			texture_status = false;
		else
			texture_status = true;

		texture_data = mod.materials[mesh.material_index].diffuse_map;
		texture_width = mod.materials[mesh.material_index].diffuse_map_width;
		texture_height = mod.materials[mesh.material_index].diffuse_map_height;
		texture_channle = mod.materials[mesh.material_index].diffuse_map_channel;

		for (int i = 0; i < mesh.tri_num; i++)
		{
			VertexPositionInputs vertex0, vertex1, vertex2;

			vertex0.positionOS = mesh.vertex[mesh.vertex_index[i][0] - 1];
			vertex1.positionOS = mesh.vertex[mesh.vertex_index[i][1] - 1];
			vertex2.positionOS = mesh.vertex[mesh.vertex_index[i][2] - 1];

			vertex0.normalWS = mesh.normal[mesh.normal_index[i][0] - 1];
			vertex1.normalWS = mesh.normal[mesh.normal_index[i][1] - 1];
			vertex2.normalWS = mesh.normal[mesh.normal_index[i][2] - 1];

			if (!mesh.tex_coord.empty())
			{
				vertex0.uv = mesh.tex_coord[mesh.tex_coord_index[i][0] - 1];
				vertex1.uv = mesh.tex_coord[mesh.tex_coord_index[i][1] - 1];
				vertex2.uv = mesh.tex_coord[mesh.tex_coord_index[i][2] - 1];
			}

			//vertex shader
			compute_vertex_WVC(vertex0);
			compute_vertex_WVC(vertex1);
			compute_vertex_WVC(vertex2);

			//face culling
			//if (face_culling_with_normal(vertex0, vertex1, vertex2))
			//	continue;

			//triangle clip
			if (triangle_clip(vertex0, vertex1, vertex2))
				continue;

			compute_vertex_NS(vertex0);
			compute_vertex_NS(vertex1);
			compute_vertex_NS(vertex2);

			//rasterization
			rasterization(vertex0, vertex1, vertex2);

			triangle_num_drawed++;
		}
	}
}

void pipeline::clean_color(const vec4& color)
{
	forwardBuffer.draw_background(color);
}

void pipeline::clean_depth()
{
	for (int i = 0; i < screenHeight; i++)
		for (int j = 0; j < screenWidth; j++)
			depthBuffer[i * screenWidth + j] = 1.0;

}

void pipeline::swapBuffer()
{
	FrameBuffer tmp = forwardBuffer;
	forwardBuffer = backwardBuffer;
	backwardBuffer = tmp;
}

void pipeline::render()
{

}