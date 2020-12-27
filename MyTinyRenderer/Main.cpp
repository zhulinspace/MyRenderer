
#include<iostream>
#include"Model.h"
#include"platform.h"


static const char* WINDOW_TITLE = "Viewer";
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;


void draw_line(int x0, int y0, int x1, int y1, framebuffer_t* frame, vec4_t color)
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
void DrawModel(Model& m,framebuffer_t *f, vec4_t color)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vec3_t v0 = m.vert(i, j);
            vec3_t v1 = m.vert(i, (j + 1) % 3);
            int x0 = (v0.x + 1.) * WINDOW_WIDTH / 2;
            int y0 = (v0.y + 1.) * WINDOW_HEIGHT / 2;
            int x1 = (v1.x + 1.) * WINDOW_WIDTH / 2;
            int y1 = (v1.y + 1.) * WINDOW_HEIGHT / 2;
            draw_line(x0, y0, x1, y1, f, color);

      }

    }
}
int main()
{
	window_t* window;
    framebuffer_t* framebuffer = new framebuffer_t(WINDOW_WIDTH, WINDOW_HEIGHT);
   

	window = window_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
 

    Model m("res//test.obj");


	//the origin is the left_bottom
	while (!window_should_close(window))
	{
		window_draw_buffer(window, framebuffer);
		//draw_line(400, 599, 400, 600, framebuffer, vec4_new(1.0,0.0,0.0,0.0));
        DrawModel(m, framebuffer, vec4_new(1.0, 0.0, 0.0, 0.0));
	}
	window_destroy(window);
	framebuffer->framebuffer_release();
	return 0;
}