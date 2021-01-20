#include"Graphics.h"
#include<limits>
vec4 default_color = { 1.0,0.0,0.0, 1 };
float default_depth = -std::numeric_limits<float>::max();

void framebuffer_t::framebuffer_clear_color(vec4f color) {
    int num_elems = this->width * this->height;
   
    for (int i = 0; i < num_elems; i++) {
        this->colorbuffer[i] = color;
    }

    //for (int i = 0; i < num_elems*this->channels; i+=4)
    //{
    //    this->buffertest[i] = (unsigned char)(color.z * 255);
    //    this->buffertest[i+1]= (unsigned char)(color.y * 255);
    //    this->buffertest[i + 2] = (unsigned char)(color.x * 255);
    //    //这应该是255 为什么不是255
    //}
}


void framebuffer_t::framebuffer_clear_depth(float depth) {
    int num_elems = this->width * this->height;
    int i;
    for (i = 0; i < num_elems; i++) {
        this->depthbuffer[i] = depth;
    }
}
void framebuffer_t::set(int x, int y, vec4f color)
{
    if (x < 0 || y < 0 || x >=this->width ||y>=this->height )
        return;
    else
    {
        int index =y * this->width +x;
       colorbuffer[index] = color;

       
      // int flip = (this->height - 1 - y) * this->width + x;
       /*int start = index *this->channels;
       for (int i = 0; i < 3; i++)
          buffertest[start + i] =(unsigned char)( color[2-i] * 255);*/
     
          
    }
       
}
framebuffer_t::framebuffer_t(int width, int height,int cls) {
   /* vec4 default_color = { 0.0,0.0,0.0, 1 };
    float default_depth = -std::numeric_limits<float>::max();*/
    int num_elems = width * height;
   

    assert(width > 0 && height > 0);

    this->channels = cls;
    this->width = width;
    this->height = height;
    this->colorbuffer = (vec4*)malloc(sizeof(vec4) * num_elems);
    this->depthbuffer = (float*)malloc(sizeof(float) * num_elems);
    this->buffertest =(unsigned char*) malloc(sizeof(unsigned char) * num_elems * cls);
    memset(buffertest, 0, sizeof(unsigned char) * num_elems * cls);
  /*  framebuffer_clear_color(default_color);
    framebuffer_clear_depth(default_depth);*/

    
}

void framebuffer_t::reset()
{
    
    framebuffer_clear_color(default_color);
    framebuffer_clear_depth(default_depth);
}

void framebuffer_t::framebuffer_release() {
    free(this->colorbuffer);
    free(this->depthbuffer);
    free(this->buffertest);
}

