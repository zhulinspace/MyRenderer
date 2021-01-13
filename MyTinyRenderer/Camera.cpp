#include<assert.h>
#include<stdlib.h>
#include"utils.h"
#include "Camera.h"


static const float NEAR = 0.1f;
static const float FAR = 1000;
static const float FOVY = TO_RADIANS(60);
static const vec3 Up = { 0,1,0 };

camera::camera(vec3 position, vec3 target, float aspect)
{
	assert((position - target).norm() > EPSILON&& aspect > 0);
	this->position = position;
	this->aspect = aspect;
	this->target = target;
}

camera::~camera()
{
}

void camera::SetCamera(vec3 position, vec3 target)
{
	assert((position - target).norm() > EPSILON);
	this->position = position;
	this->target = target;
}

void camera::OrbitUpdate(motion& m)
{
	vec3 from_target = this->position - this->target;
	vec3 from_camera = this->target - this->position;
	vec3 pan = cacluate_pan(from_camera, m);
	vec3 offset = cacluate_offset(from_target, m);
	this->target = this->target + pan;
	this->position = this->target + offset;
}

vec3 camera::cacluate_pan(vec3 from_camera, motion& m)
{
	vec3 forward = from_camera.normalize();
	vec3 left = cross(Up, forward);
	vec3 up = cross(forward, left);

	float distance = from_camera.norm();
	float factor = distance * (float)tan(FOVY / 2) * 2;
	vec3 delta_x = left * (m.pan.x * factor);
	vec3 delta_y = up * (m.pan.y * factor);
	return delta_x + delta_y;
}

vec3 camera::cacluate_offset(vec3 from_target, motion& m)
{

	float radius = from_target.norm();
	float theta = (float)atan2(from_target.x, from_target.z);//azimuth
	float phi = (float)acos(from_target.y / radius);
	float factor = PI * 2;

	radius *= (float)pow(0.95, m.dolly);
	theta -= m.orbit.x * factor;
	phi -= m.orbit.y * factor;
	phi = float_clamp(phi,EPSILON,PI-EPSILON);

	vec3 offset;
	offset.x = radius * (float)sin(phi) * (float)sin(theta);
	offset.y = radius * (float)cos(phi);
	offset.z = radius * (float)sin(phi) * (float)cos(theta);
	return offset;

}

void camera::GetViewMatrix()
{
	ViewMatrix=lookat(position, target, Up);
}

void camera::GetProjMatrix()
{
	ProjMatrix= proj_matrix(FOVY, aspect, NEAR, FAR);
}
