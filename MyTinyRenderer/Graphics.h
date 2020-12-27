#pragma once
#include"maths.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

class framebuffer_t {
public:

    int width, height;
    vec4_t* colorbuffer;
    float* depthbuffer;


    /* framebuffer management */
    framebuffer_t(int width, int height);
    void framebuffer_release();
    void framebuffer_clear_color( vec4_t color);
    void framebuffer_clear_depth(float depth);
    void set(int x, int y, vec4_t color);

};