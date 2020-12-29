#pragma once
#include"Math.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

class framebuffer_t {
public:

    int width, height;
    Vec4f* colorbuffer;
    float* depthbuffer;


    /* framebuffer management */
    framebuffer_t(int width, int height);
    void framebuffer_release();
    void framebuffer_clear_color( Vec4f color);
    void framebuffer_clear_depth(float depth);
    void set(int x, int y, Vec4f color);

};