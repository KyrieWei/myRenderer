#include <assert.h>
#include <string>
#include "Image.h"

image_t* image_create(int width, int height, int channels)
{
	assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);

	int size = sizeof(unsigned char) * width * height * channels;
	image_t* image = (image_t*)malloc(sizeof(image_t));
	image->width = width;
	image->height = height;
	image->channels = channels;

	image->buffer = (unsigned char*)malloc(size);
	memset(image->buffer, 0, size);

	return image;
}

void image_release(image_t* image)
{
	free(image->buffer);
	free(image);
}

void blit_framebuffer_to_image(FrameBuffer* src, image_t* dst)
{
	int width = dst->width;
	int height = dst->height;

	assert(src->width == dst->width && src->height == dst->height);
	assert(dst->channels == 4);

	for (int i = 0; i < width * height; i++)
	{
		dst->buffer[i * 4] = src->image_buffer[i * 4 + 2];
		dst->buffer[i * 4 + 1] = src->image_buffer[i * 4 + 1];
		dst->buffer[i * 4 + 2] = src->image_buffer[i * 4];
		dst->buffer[i * 4 + 3] = src->image_buffer[i * 4 + 3];
	}
}