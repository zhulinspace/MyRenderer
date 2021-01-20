#pragma once
#include"maths.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

class framebuffer_t {
public:

    int width, height;
    vec4* colorbuffer;
    float* depthbuffer;
    unsigned char* buffertest;
    int channels;
    /* framebuffer management */
    framebuffer_t(int width, int height,int cls=4);
    void reset();
    void framebuffer_release();
    void framebuffer_clear_color( vec4f color);
    void framebuffer_clear_depth(float depth);
    void set(int x, int y, vec4f color);

};