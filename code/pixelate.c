#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include "libs/lr/lr.h"

PUSH_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"
POP_WARNINGS

void usage()
{
	printf("Usage: pixelate [-p size] <input> [output]\n");
}

int main(int argCount, char *args[]) {

	static int pixelSize = 30;
	char* imagePath = NULL;
	char* outputPath = NULL;

	int i;
	for (i = 1; i < argCount; i++)
	{
		if (!strcmp(args[i], "-p")) {
			i++;
			if(i > argCount) {
				usage();
				return 1;
			}
			pixelSize = atoi(args[i]);
		} else if(access(args[i], F_OK) == 0) {
			imagePath = args[i];
			i++;
			outputPath = (argCount >= i + 1) ? args[i] : "output.jpg";
		} else {
			fprintf(stderr, "ERROR: Could not open file: %s\n", args[i]);
			return 1;
		}
	}
	if (imagePath == NULL)
	{
		usage();
		return 1;
	}

	printf("  pixel size: %d\n", pixelSize);
	printf(" output path: %s\n", outputPath);

	int width, height, channels;
	int bytesPerPixel = 4;

	stbi_uc *inputImage = stbi_load(imagePath, &width, &height, &channels, 4);

// TODO: Have an output width and height that will crop the image to be a multiple of pixelSize

	size_t outSize = bytesPerPixel*(width*height);
	void *outContents = malloc(outSize);

	printf(" pixelating...\n");
	for (int y = 0; y < height; y += pixelSize) {
		for (int x = 0; x < width; x += pixelSize) {
			unsigned int red = 0;
			unsigned int green = 0;
			unsigned int blue = 0;
			unsigned int alpha = 0;

			unsigned int *row = (unsigned int *)(inputImage + bytesPerPixel*(width*y + x));
			for (int inY = 0; inY < pixelSize && y + inY < height; inY++) {
				unsigned int *pixel = row;
				for (int inX = 0; inX < pixelSize && x + inX < width; inX++) {
					alpha += ((*pixel & 0xFF000000) >> 24);
					green += ((*pixel & 0x00FF0000) >> 16);
					blue  += ((*pixel & 0x0000FF00) >>  8);
					red   += ((*pixel & 0x000000FF) >> 0);
					pixel += 1;
				}
				row += width;
			}

			// Average out values
			red /= (pixelSize * pixelSize);
			green /= (pixelSize * pixelSize);
			blue /= (pixelSize * pixelSize);
			alpha /= (pixelSize * pixelSize);

			if(channels == 3)
			{
				alpha = 0xff;
			}

			unsigned int value = ((red << 0) | (green << 16) | (blue << 8) | (alpha << 24));

			row = (unsigned int *)(outContents + bytesPerPixel*(width*y + x));
			for(int outY = 0; outY < pixelSize && y + outY < height; outY++) {
				unsigned int *pixel = row;
				for(int outX = 0; outX < pixelSize && x + outX < width; outX++) {
					*pixel = value;
					pixel += 1;
				}
				row += width;
			}

		}
	}

	printf(" writing...\n");
	stbi_write_jpg(outputPath, width, height, bytesPerPixel, outContents, 100);

	printf(" done.\n");

	return 0;
}
