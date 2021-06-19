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

void pipeline::vertex_shader(VertexPositionInputs& vertex)
{
	vertex.positionWS = model * vec4(vertex.positionOS, 1.0);
	vertex.positionVS = view * vec4(vertex.positionWS, 1.0);
	vertex.positionCS = projection * vec4(vertex.positionVS, 1.0);

	//clip

	vertex.positionNDC = vertex.positionCS / vertex.positionCS.w;
	vertex.positionSS = viewport(vertex.positionNDC, screenWidth, screenHeight);

	//normal 
}

void pipeline::fragment_shader(VertexPositionInputs& vertex)
{
	//TODO:compute lighting
}

bool pipeline::face_culling(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	vec3 view_dir = normalize(vertex0.positionWS - camera_pos);
	vec3 normal = compute_flat_normal(vertex0, vertex1, vertex2);

	if (dot(view_dir, normal) < 0.0)
		return false;
	else
		return true;
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

void pipeline::flat_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{

	vec3 normal = compute_flat_normal(vertex0, vertex1, vertex2);
	vec4 color = vec4(bling_phong_lighting(light, normal), 1.0);

	//draw the down half triangle
	for (int i = vertex0.positionSS.y; i <= vertex1.positionSS.y; i++)
	{
		if (vertex0.positionSS.y == vertex1.positionSS.y)
		{
			int x0 = vertex0.positionSS.x;
			int x1 = vertex1.positionSS.x;

			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
				forwardBuffer.draw_pixel(j, i, color);
			break;
		}

		double t01 = (double)(i - vertex0.positionSS.y) / (vertex1.positionSS.y - vertex0.positionSS.y);
		double t02 = (double)(i - vertex0.positionSS.y) / (vertex2.positionSS.y - vertex0.positionSS.y);

		int x01 = (int)lerp(vertex0.positionSS.x, vertex1.positionSS.x, t01);
		int x02 = (int)lerp(vertex0.positionSS.x, vertex2.positionSS.x, t02);

		if (x01 > x02) std::swap(x01, x02);

		for (int j = x01; j <= x02; j++)
			forwardBuffer.draw_pixel(j, i, color);
	}

	//draw the up half triangle
	for (int i = vertex1.positionSS.y; i <= vertex2.positionSS.y; i++)
	{
		if (vertex1.positionSS.y == vertex2.positionSS.y)
		{
			int x0 = vertex1.positionSS.x;
			int x1 = vertex2.positionSS.x;

			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
				forwardBuffer.draw_pixel(j, i, color);
			break;
		}

		double t12 = (double)(i - vertex1.positionSS.y) / (vertex2.positionSS.y - vertex1.positionSS.y);
		double t02 = (double)(i - vertex0.positionSS.y) / (vertex2.positionSS.y - vertex0.positionSS.y);

		int x12 = (int)lerp(vertex1.positionSS.x, vertex2.positionSS.x, t12);
		int x02 = (int)lerp(vertex0.positionSS.x, vertex2.positionSS.x, t02);

		if (x12 > x02) std::swap(x12, x02);

		for (int j = x12; j <= x02; j++)
			forwardBuffer.draw_pixel(j, i, color);
	}
}

void pipeline::gouraud_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	//draw the down half triangle
	for (int i = vertex0.positionSS.y; i <= vertex1.positionSS.y; i++)
	{
		if (vertex0.positionSS.y == vertex1.positionSS.y)
		{
			int x0 = vertex0.positionSS.x;
			int x1 = vertex1.positionSS.x;

			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
			{
				vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, i));

				double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

				if (depth_test(j, i, depth))
				{
					depth_write(j, i, depth);
					vec4 color = vec4(bary_coord.x * vertex0.texture + bary_coord.y * vertex1.texture + bary_coord.z * vertex2.texture, 1.0);
					forwardBuffer.draw_pixel(j, i, color);
				}
			}
				
			break;
		}

		double t01 = (double)(i - vertex0.positionSS.y) / (vertex1.positionSS.y - vertex0.positionSS.y);
		double t02 = (double)(i - vertex0.positionSS.y) / (vertex2.positionSS.y - vertex0.positionSS.y);

		int x01 = (int)lerp(vertex0.positionSS.x, vertex1.positionSS.x, t01);
		int x02 = (int)lerp(vertex0.positionSS.x, vertex2.positionSS.x, t02);

		if (x01 > x02) std::swap(x01, x02);

		for (int j = x01; j <= x02; j++)
		{
			vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, i));
			double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

			if (depth_test(j, i, depth))
			{
				depth_write(j, i, depth);
				vec4 color = vec4(bary_coord.x * vertex0.texture + bary_coord.y * vertex1.texture + bary_coord.z * vertex2.texture, 1.0);
				forwardBuffer.draw_pixel(j, i, color);
			}
		}
	}

	//draw the up half triangle
	for (int i = vertex1.positionSS.y; i <= vertex2.positionSS.y; i++)
	{
		if (vertex1.positionSS.y == vertex2.positionSS.y)
		{
			int x0 = vertex1.positionSS.x;
			int x1 = vertex2.positionSS.x;

			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
			{
				vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, i));
				double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

				if (depth_test(j, i, depth))
				{
					depth_write(j, i, depth);
					vec4 color = vec4(bary_coord.x * vertex0.texture + bary_coord.y * vertex1.texture + bary_coord.z * vertex2.texture, 1.0);
					forwardBuffer.draw_pixel(j, i, color);
				}
			}
			break;
		}

		double t12 = (double)(i - vertex1.positionSS.y) / (vertex2.positionSS.y - vertex1.positionSS.y);
		double t02 = (double)(i - vertex0.positionSS.y) / (vertex2.positionSS.y - vertex0.positionSS.y);

		int x12 = (int)lerp(vertex1.positionSS.x, vertex2.positionSS.x, t12);
		int x02 = (int)lerp(vertex0.positionSS.x, vertex2.positionSS.x, t02);

		if (x12 > x02) std::swap(x12, x02);

		for (int j = x12; j <= x02; j++)
		{
			vec3 bary_coord = barycentirc_coord(vertex0.positionSS, vertex1.positionSS, vertex2.positionSS, vec2(j, i));
			double depth = depth_lerp(vertex0.positionNDC.z, vertex1.positionNDC.z, vertex2.positionNDC.z, bary_coord);

			if (depth_test(j, i, depth))
			{
				depth_write(j, i, depth);
				vec4 color = vec4(bary_coord.x * vertex0.texture + bary_coord.y * vertex1.texture + bary_coord.z * vertex2.texture, 1.0);
				forwardBuffer.draw_pixel(j, i, color);
			}
		}
	}
}

void pipeline::blingPhong_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{

}

void pipeline::rasterization(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2)
{
	//draw_triangle
	VertexPositionInputs tmp_0(vertex0);
	VertexPositionInputs tmp_1(vertex1);
	VertexPositionInputs tmp_2(vertex2);

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
	triangle_num_drawed = 0;
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
		vertex_shader(vertex0);
		vertex_shader(vertex1);
		vertex_shader(vertex2);

		//face culling
		if (face_culling(vertex0, vertex1, vertex2))
			continue;


		//fragment shader


		//rasterization
		triangle_num_drawed++;
		rasterization(vertex0, vertex1, vertex2);
	}
}

void pipeline::drawArrays(const Model& obj)
{
	vec3 point0, point1, point2;
	vec4 clip_point0, clip_point1, clip_point2;
	vec3 ndc_point0, ndc_point1, ndc_point2;
	vec2 scr_point0, scr_point1, scr_point2;

	for (int i = 0; i < obj.face_num; i++)
	{
		//vertex shader
		//model space

		point0 = obj.vertex[obj.vertex_index[i][0] - 1];
		point1 = obj.vertex[obj.vertex_index[i][1] - 1];
		point2 = obj.vertex[obj.vertex_index[i][2] - 1];

		//clip space
		clip_point0 = projection * view * model * vec4(point0, 1.0);
		clip_point1 = projection * view * model * vec4(point1, 1.0);
		clip_point2 = projection * view * model * vec4(point2, 1.0);
		
		//triangle clip
		//if(abs(clip_point0.x) > abs(clip_point0.w))

		//ndc space
		ndc_point0 = vec3(clip_point0.x, clip_point0.y, clip_point0.z) / clip_point0.w;
		ndc_point1 = vec3(clip_point1.x, clip_point1.y, clip_point1.z) / clip_point1.w;
		ndc_point2 = vec3(clip_point2.x, clip_point2.y, clip_point2.z) / clip_point2.w;

		//scree space
		scr_point0 = viewport(ndc_point0, screenWidth, screenHeight);
		scr_point1 = viewport(ndc_point1, screenWidth, screenHeight);
		scr_point2 = viewport(ndc_point2, screenWidth, screenHeight);

		//fragment
		vec4 color = vec4(0.2, 0.6, 0.9, 1.0);


		forwardBuffer.draw_filled_triangle(scr_point0, scr_point1, scr_point2, color);
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

void pipeline::render()
{

}