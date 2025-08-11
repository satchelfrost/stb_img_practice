#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <math.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

Color color_from_HSV(float hue, float saturation, float value)
{
    Color color = { 0, 0, 0, 255 };

    // Red channel
    float k = fmodf((5.0f + hue/60.0f), 6);
    float t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.r = (unsigned char)((value - value*saturation*k)*255.0f);

    // Green channel
    k = fmodf((3.0f + hue/60.0f), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.g = (unsigned char)((value - value*saturation*k)*255.0f);

    // Blue channel
    k = fmodf((1.0f + hue/60.0f), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.b = (unsigned char)((value - value*saturation*k)*255.0f);

    return color;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage: %s 0|1|2|3 outfile.png\n", argv[0]);
        return 1;
    }

    const char *file_name = argv[2];
    int width  = 500;
    int height = 400;
    int comp   = 4; // RGBA
    int stride = 0;
    void *data = malloc(width * height * sizeof(int));

    switch (atoi(argv[1])) {
    case 0: {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int a = (i / 50) % 2;
                int b = (j / 50) % 2;
                int color = ((a + b) % 2 == 0) ? 0xff000000 : 0xffffffff;
                ((int*)(data))[i * width + j] = color;
            }
        }
    } break;
    case 1: {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                float nx = (float)j/width;
                float ny = (float)i/height;
                unsigned char r = (unsigned char)(255 * nx);
                unsigned char g = (unsigned char)(255 * ny);
                ((int*)(data))[i * width + j] = (0xff << 24) | (0x00 << 16) | (g << 8) | r;
            }
        }
    } break;
    case 2: {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                float nx = (float)j/width;
                float ny = (float)i/height;
                Color color = color_from_HSV(((nx + ny) * 0.5) * 360.0, 1.0, 1.0);
                unsigned char r = color.r;
                unsigned char g = color.g;
                unsigned char b = color.b;
                ((int*)(data))[i * width + j] = (0xff << 24) | (b << 16) | (g << 8) | r;
            }
        }
    } break;
    case 3: {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                float nx = (float)j/width;
                float ny = (float)i/height;
                nx = sinf(nx * M_PI * width / height - M_PI / 8.0);
                ny = sinf(ny * M_PI);
                Color color = color_from_HSV(((nx + ny) * 0.5) * 360.0, 1.0, 1.0);
                unsigned char r = color.r;
                unsigned char g = color.g;
                unsigned char b = color.b;
                ((int*)(data))[i * width + j] = (0xff << 24) | (b << 16) | (g << 8) | r;
            }
        }
    } break;
    default:
        fprintf(stderr, "no such case %d\n", atoi(argv[1]));
        return 1;
    }

    if (stbi_write_png(file_name, width, height, comp, data, stride)) {
        printf("successfully wrote %s\n", file_name);
    } else {
        fprintf(stderr, "failed to write %s\n", file_name);
        return 1;
    }

    free(data);

    return 0;
}
