#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Imlib2.h>
/* #include "config.h" */

void usage()
{
	printf("Usage: pixelate [-p size] <input> [output]\n");
}

int main(int argc, char *argv[]) {

	static int pixelSize = 30;
	char* imagePath = NULL;
	char* outputPath = NULL;

	int i;
	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-p")) {
			i++;
			if(i > argc) {
				usage();
				return 1;
			}
			pixelSize = atoi(argv[i]);
			printf("pixelsize: %d\n",pixelSize);
		} else if(access(argv[i], F_OK) == 0) {
			imagePath = argv[i];
			i++;
			outputPath = (argc >= i + 1) ? argv[i] : "output.png";
		} else {
			printf("could not open file: %s", argv[i]);
			return 1;
		}
	}
	if (imagePath == NULL)
	{
		usage();
		return 1;
	}

	printf("pixel size: %d", pixelSize);

    // Load the image using Imlib
    Imlib_Image image = imlib_load_image(imagePath);
    imlib_context_set_image(image);

    // Get the screen dimensions
    int width = imlib_image_get_width();
    int height = imlib_image_get_height();

    for (int y = 0; y < height; y += pixelSize) {
        for (int x = 0; x < width; x += pixelSize) {
            int red = 0;
            int green = 0;
            int blue = 0;

            Imlib_Color pixel;
            Imlib_Color* pp;
            pp = &pixel;

            for (int j = 0; j < pixelSize && j < height; j++) {
                for (int i = 0; i < pixelSize && i < width; i++) {
                    imlib_image_query_pixel(x + i, y + j, pp);
                    red += pixel.red;
                    green += pixel.green;
                    blue += pixel.blue;
                }
            }

            red /= (pixelSize * pixelSize);
            green /= (pixelSize * pixelSize);
            blue /= (pixelSize * pixelSize);

            imlib_context_set_color(red, green, blue, pixel.alpha);
            imlib_image_fill_rectangle(x, y, pixelSize, pixelSize);

            red = 0;
            green = 0;
            blue = 0;
        }
    }

    // Save the pixelated image
    imlib_save_image(outputPath);

    // Cleanup
    imlib_free_image();

    return 0;
}
