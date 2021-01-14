#pragma once
#include"maths.h"


typedef struct
{
	vec2 orbit;
	vec2 pan;
	float dolly;
}motion;
class camera
{
public:
	vec3 position;
	vec3 target;
	float aspect;
	camera(vec3 position,vec3 target,float aspect);
	
	~camera();

	void SetCamera(vec3 position,vec3 target);
	void OrbitUpdate(motion& m);


	static vec3 cacluate_pan(vec3 from_camera,motion &m);
	static vec3 cacluate_offset(vec3 from_target,motion &m);
	vec3 GetPosition() { return position; }
	vec3 GetForward() { return (target - position).normalize(); }
	mat4 GetMVPMatrix();
	

};
