#include"Graphics.h"
#include<limits>
void framebuffer_t::framebuffer_clear_color(vec4 color) {
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
void framebuffer_t::set(int x, int y, vec4 color)
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
   /* vec4 default_color = { 0.0,0.0,0.0, 1 };
    float default_depth = -std::numeric_limits<float>::max();*/
    int num_elems = width * height;
   

    assert(width > 0 && height > 0);

 
    this->width = width;
    this->height = height;
    this->colorbuffer = (vec4*)malloc(sizeof(vec4) * num_elems);
    this->depthbuffer = (float*)malloc(sizeof(float) * num_elems);

  /*  framebuffer_clear_color(default_color);
    framebuffer_clear_depth(default_depth);*/

    
}

void framebuffer_t::reset()
{
    vec4 default_color = { 0.0,0.0,0.0, 1 };
    float default_depth = -std::numeric_limits<float>::max();
    framebuffer_clear_color(default_color);
    framebuffer_clear_depth(default_depth);
}

void framebuffer_t::framebuffer_release() {
    free(this->colorbuffer);
    free(this->depthbuffer);
    
}

