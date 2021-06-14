#pragma once
#include "../pipelines/FrameBuffer.h"

typedef struct
{
	int width, height, channels;
	unsigned char* buffer;
}image_t;

image_t* image_create(int width, int height, int channels);
void image_release(image_t* image_t);
void blit_framebuffer_to_image(FrameBuffer* src, image_t* dst);