#include "FrameBuffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../tools/stb_image_write.h"

FrameBuffer::FrameBuffer()
{

}

FrameBuffer::FrameBuffer(const int _width, const int _height, const int _channel) : width(_width), height(_height), channel(_channel)
{
	image_buffer = new unsigned char[width * height * channel];
	//clean buffer;
	draw_background(vec4(0, 0, 0, 1));
}

FrameBuffer::~FrameBuffer()
{
	if(image_buffer != NULL)
		delete[] image_buffer;
	image_buffer = NULL;
}

void FrameBuffer::draw_background(const vec4& color)
{
	if (!check_buffer())
		return;

	for (int i = 0, index = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < channel; k++)
				image_buffer[index++] = static_cast<unsigned char>(color[k] * 255);
		}
	}
}

//point x, y left bottom to right up 
void FrameBuffer::draw_pixel(int x, int y, const vec4& color)
{
	if(!check_buffer())
		return;

	if (x > width || x <= 0 || y > height || y <= 0)
	{
		std::cout << "draw_pixel out of frame_buffer range" << std::endl;
		return;
	}

	int index = ((height - y - 1) * width + x) * channel;

	for (int i = 0; i < channel; i++)
		image_buffer[index + i] = static_cast<unsigned char>(color[i] * 255);
}


void FrameBuffer::draw_line(const vec2& point0, const vec2& point1, const vec4& color)
{
	if (!check_buffer())
		return;

	int x0 = static_cast<int>(point0.x);
	int y0 = static_cast<int>(point0.y);
	int x1 = static_cast<int>(point1.x);
	int y1 = static_cast<int>(point1.y);

	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) // make it left to right
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	double derror2 = std::abs(dy) * 2;
	double error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
			draw_pixel(y, x, color);
		else
			draw_pixel(x, y, color);

		error2 += derror2;
		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void FrameBuffer::draw_filled_triangle(const vec2& point0, const vec2& point1, const vec2& point2, const vec4& color)
{
	//make sure y2 > y1 > y0
	vec2 tmp_0(point0);
	vec2 tmp_1(point1);
	vec2 tmp_2(point2);
	if (tmp_0.y > tmp_1.y) std::swap(tmp_0, tmp_1);
	if (tmp_1.y > tmp_2.y) std::swap(tmp_1, tmp_2);
	if (tmp_0.y > tmp_2.y) std::swap(tmp_0, tmp_2);

	
	
	//draw the down half triangle
	for (int i = tmp_0.y; i <= tmp_1.y; i++)
	{
		if (tmp_0.y == tmp_1.y)
		{
			int x0 = tmp_0.x;
			int x1 = tmp_1.x;
			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
				draw_pixel(j, i, color);
			break;
		}
		double t01 = (double)(i - tmp_0.y) / (tmp_1.y - tmp_0.y);
		double t02 = (double)(i - tmp_0.y) / (tmp_2.y - tmp_0.y);
		int x01 = (int)lerp(tmp_0.x, tmp_1.x, t01);
		int x02 = (int)lerp(tmp_0.x, tmp_2.x, t02);

		if (x01 > x02) std::swap(x01, x02);

		for (int j = x01; j <= x02; j++)
			draw_pixel(j, i, color);
	}

	//draw the up half triangle
	for (int i = tmp_1.y; i <= tmp_2.y; i++)
	{
		if (tmp_1.y == tmp_2.y)
		{
			int x0 = tmp_1.x;
			int x1 = tmp_2.x;
			if (x0 > x1) std::swap(x0, x1);

			for (int j = x0; j <= x1; j++)
				draw_pixel(j, i, color);
			break;
		}
		double t12 = (double)(i - tmp_1.y) / (tmp_2.y - tmp_1.y);
		double t02 = (double)(i - tmp_0.y) / (tmp_2.y - tmp_0.y);

		int x12 = (int)lerp(tmp_1.x, tmp_2.x, t12);
		int x02 = (int)lerp(tmp_0.x, tmp_2.x, t02);

		if (x12 > x02) std::swap(x12, x02);

		for (int j = x12; j <= x02; j++)
			draw_pixel(j, i, color);
	}
}

void FrameBuffer::write_to_image(const char* file_name)
{
	if (!check_buffer())
		return;

	stbi_write_jpg(file_name, width, height, channel, image_buffer, 100);
}

bool FrameBuffer::check_buffer()
{
	if (image_buffer == nullptr)
	{
		std::cout << "image_buffer is not initialized" << std::endl;
		return false;
	}

	return true;
}