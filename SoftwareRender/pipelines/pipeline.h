#pragma once
#include "FrameBuffer.h"
#include "../gui/Window.h"
#include "../gameobjects/Model.h"
#include "vertexOut.h"
#include "Lighting.h"
#include "../gameobjects/Object.h"

enum shading_mode
{
	FLAT_SHADING,
	GOURAUD_SHADING,
	BLINGPHONG_SHADING
};

class pipeline
{
public:

	int screenWidth, screenHeight;

	mat4x4 model;
	mat4x4 view;
	mat4x4 projection;

	Light light;

	vec3 camera_pos;

	FrameBuffer forwardBuffer;
	FrameBuffer backwardBuffer;

	shading_mode shadingMode;

	double* depthBuffer;

	unsigned char* texture_data;
	int texture_width, texture_height, texture_channle;

	int triangle_num_drawed;

public:
	pipeline();
	pipeline(int width, int height);
	~pipeline();

	void setMVP(const mat4x4& model, const mat4x4& view, const mat4x4& projection);
	void setLight(const Light& light);
	void setCamera(const vec3& cam);
	void setShadingMode(shading_mode mode);

	//vertex & fragment shader
	void compute_vertex_WVC(VertexPositionInputs& vertex);
	void compute_vertex_NS(VertexPositionInputs& vertex);
	void fragment_shader(VertexPositionInputs& vertex);

	//face culling
	bool face_culling(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);

	//triangle clip
	bool triangle_clip(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);

	//depth write & test 
	void depth_persp(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);
	void depth_write(int i, int j, double depth);
	bool depth_test(int, int j, double depth);

	void bind_texture(unsigned char* texture_data, int width, int height, int channel);
	vec4 sample_texture(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2, const vec3& bary_coord);
	

	//shading strategy
	void flat_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);
	void gouraud_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);
	void blingPhong_shading(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);

	void rasterization(const VertexPositionInputs& vertex0, const VertexPositionInputs& vertex1, const VertexPositionInputs& vertex2);

	
	void drawArrays(vec3 data[], int data_length, int index[][3], int index_length, vec3 data_color[]);
	void drawArrays(const Object& obj);


	void clean_color(const vec4& color);
	void clean_depth();

	void swapBuffer();

	void render();
};

