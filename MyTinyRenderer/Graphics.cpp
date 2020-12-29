#include"Graphics.h"

void framebuffer_t::framebuffer_clear_color(Vec4f color) {
    int num_elems = this->width * this->height;
    int i;
    for (i = 0; i < num_elems; i++) {
        this->colorbuffer[i] = color;
    }
}

void framebuffer_t::framebuffer_clear_depth(float depth) {
    int num_elems = this->width * this->height;
    int i;
    for (i = 0; i < num_elems; i++) {
        this->depthbuffer[i] = depth;
    }
}
void framebuffer_t::set(int x, int y, Vec4f color)
{
    if (x < 0 || y < 0 || x >=this->width ||y>=this->height )
        return;
    else
    {
        int index =y * this->width +x;
       colorbuffer[index] = color;
    }
       
}
framebuffer_t::framebuffer_t(int width, int height) {
    Vec4f default_color = { 0.0,0.0,0.0, 1 };
    float default_depth = 1;
    int num_elems = width * height;
   

    assert(width > 0 && height > 0);

 
    this->width = width;
    this->height = height;
    this->colorbuffer = (Vec4f*)malloc(sizeof(Vec4f) * num_elems);
    this->depthbuffer = (float*)malloc(sizeof(float) * num_elems);

    framebuffer_clear_color(default_color);
    framebuffer_clear_depth(default_depth);

    
}

void framebuffer_t::framebuffer_release() {
    free(this->colorbuffer);
    free(this->depthbuffer);
    
}

