#include "pipeline.h"


void pipeline::setMVP(const mat4x4& model, const mat4x4& view, const mat4x4& projection)
{
	this->model = model;
	this->view = view;
	this->projection = projection;
}

void pipeline::drawArrays(vec3 data[], int data_length, int index[][3], int index_length)
{

	vec3 point0, point1, point2;
	vec4 clip_point0, clip_point1, clip_point2;
	vec3 ndc_point0, ndc_point1, ndc_point2;
	vec2 scr_point0, scr_point1, scr_point2;

	for (int i = 0; i < index_length; i++)
	{
		//vertex shader
		//model space
		point0 = data[index[i][0]];
		point1 = data[index[i][1]];
		point2 = data[index[i][2]];

		//clip space
		clip_point0 = projection * view * model * vec4(point0, 1.0);
		clip_point1 = projection * view * model * vec4(point1, 1.0);
		clip_point2 = projection * view * model * vec4(point2, 1.0);

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

void pipeline::render()
{

	

}