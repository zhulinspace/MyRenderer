#pragma once
#include"maths.h"
#include<iostream>
class IShader
{
	virtual vec4 vertex(const int iface,const int nthvert)=0;
	virtual bool fragment(const vec3 bar, vec4 color) = 0;

};

void triangle(const vec4 clip_verts[3], IShader& shader);