#include "image.h"
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"maths.h"

image_t* image_create(int width, int height, int channels) {
    int buffer_size = width * height * channels;
    image_t* image;

    assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);

    image = (image_t*)malloc(sizeof(image_t));
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->buffer = (unsigned char*)malloc(buffer_size);
    memset(image->buffer, 0, buffer_size);
    return image;
}

void image_release(image_t* image) {
    free(image->buffer);
    free(image);
}

//image_t* image_load(const char* filename) {
//    const char* extension = private_get_extension(filename);
//    if (strcmp(extension, "tga") == 0) {
//        return private_load_tga_image(filename);
//    }
//    else {
//        assert(0);
//        return NULL;
//    }
//}
//
//void image_save(image_t* image, const char* filename) {
//    const char* extension = private_get_extension(filename);
//    if (strcmp(extension, "tga") == 0) {
//        private_save_tga_image(image, filename);
//    }
//    else {
//        assert(0);
//    }
//}