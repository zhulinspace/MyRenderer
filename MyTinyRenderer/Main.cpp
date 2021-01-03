
#include<iostream>
#include<fstream>
#include"Model.h"
#include"platform.h"

std::ofstream outputfile;

static const char* WINDOW_TITLE = "Viewer";
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;




inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}


void draw_line(int x0, int y0, int x1, int y1, framebuffer_t* frame, vec4 color)
{
    int index = 0;
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // if the line is steep, we transpose the image 
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // make it left−to−right 
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int init = std::abs(dy) * 2;
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {

            frame->set(y, x, color);// if transposed, de−transpose 
        }
        else {
            frame->set(x, y, color);
        }
        error += init;
        if (error > dx)
        {
            y += (y1 > y0?1 : -1);
            error -= dx * 2;
        }
    }
}
vec3 barycentric(vec3* pnts, vec3 p)
{ // the point p is in same plane as triangle pnts
  //so we can decribe p =(1-u-v)A+uB+vC;
  // same as  0 = u*Vector(AB) + v*Vector(AC) +Vector(PA)

    vec3 u = cross(vec3(pnts[2][0] - pnts[0][0], pnts[1][0] - pnts[0][0], pnts[0][0] - p[0]), vec3(pnts[2][1] - pnts[0][1], pnts[1][1] - pnts[0][1], pnts[0][1] - p[1]));
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u[2]) < 1) return vec3(-1, 1, 1);
    return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}
void draw_triangle(vec3 *pts,framebuffer_t *frame,vec4 color)
{
   
    vec2i bboxmin(frame->width-1, frame->height - 1);
    vec2i bboxmax(0,0);
    vec2i clamp(frame->width - 1, frame->height - 1);
    for (int i = 0; i < 3; i++)
    {//个数
        for (int j = 0; j < 2; j++)
        {//维度
            bboxmin[j] = fmax(0,       fmin( pts[i][j],bboxmin[j]));
            bboxmax[j] = fmin(clamp[j],fmax(pts[i][j], bboxmax[j]));
        }
    }


    vec3 p;
    for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
    {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            vec3 screen = barycentric(pts, p);
            if (screen.x < 0 || screen.y < 0 || screen.z < 0)continue;
         
            p.z = 0;
            for (int i = 0; i < 3; i++)p.z += pts[i][2] * screen[i];
            if (frame->depthbuffer[int(p.x + p.y * frame->width)]< p.z)
            {
                frame->depthbuffer[int(p.x + p.y * frame->width)] = p.z;
                frame->set((int)p.x, (int)p.y, color);
            }
            //这里zbuffer暂定是越大则离相机越近
        }
    }

}
void DrawModelLine(Model& m,framebuffer_t *f, vec4 color)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vec3 v0 = m.vert(i, j);
            vec3 v1 = m.vert(i, (j + 1) % 3);
            int x0 = (v0.x + 1.) * f->width / 2;
            int y0 = (v0.y + 1.) * f->height / 2;
            int x1 = (v1.x + 1.) * f->width / 2;
            int y1 = (v1.y + 1.) * f->height / 2;
            draw_line(x0, y0, x1, y1, f, color);

      }

    }
}
void DrawModelTriangle(Model& m, framebuffer_t* f, vec4 color)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        vec3 screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            vec3 world_coords= m.vert(i, j);
            screen_coords[j] = vec3((world_coords.x + 1.)*f->width/2, (world_coords.y + 1.)*f->height/2, world_coords.z);

        }
        draw_triangle(screen_coords, f, color);
    }
}

void DrawModelTriangle(Model& m, framebuffer_t* f)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        vec3 screen_coords[3];
        vec3 world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            world_coords[j] = m.vert(i, j);
            screen_coords[j] = vec3((world_coords[j].x + 1.) * f->width / 2, (world_coords[j].y + 1.) * f->height / 2,world_coords[j].z);

        }
        //std::cout << "世界坐标系：" << world_coords[0] << world_coords[1] << world_coords[2] << std::endl;
        //vec3 n = cross(world_coords[2] - world_coords[1], world_coords[2] - world_coords[0]);
        vec3 n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);


        n.normalize();
       // std::cout << "法向量：" << n << std::endl;
        double intensity = n * vec3(0, 0, -1);
       
        if (intensity > 0)
        {
            //std::cout << "intensity:" << std::endl;
           // draw_triangle(screen_coords, f, vec4(random_double(), random_double(), random_double(), 1.0));
            draw_triangle(screen_coords, f, vec4(intensity, intensity, intensity, 1.0));
        }
    }
}
int main()
{
   /* outputfile.open("res.txt", std::ios::app);
    if (!outputfile)std::cout << "open failed\n";*/

	window_t* window;
    framebuffer_t* framebuffer = new framebuffer_t(WINDOW_WIDTH, WINDOW_HEIGHT);
   

	window = window_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
 

    Model m("res//test.obj");


	//the origin is the left_bottom
	while (!window_should_close(window))
	{
	  window_draw_buffer(window, framebuffer);
		//draw_line(400, 599, 400, 600, framebuffer, vec4_new(1.0,0.0,0.0,0.0));
       //DrawModelTriangle(m, framebuffer, vec4(random_double(),random_double(), random_double(), 1.0));
       DrawModelTriangle(m, framebuffer);
      // DrawModelTriangle(m, framebuffer, vec4(1.0, 0, 0, 0.0));

	}
	window_destroy(window);
	framebuffer->framebuffer_release();
	


    vec4 t(1, 2, 2, 3);
    std::cout << sizeof(t) << std::endl;
    return 0;
}