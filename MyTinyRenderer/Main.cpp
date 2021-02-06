
#include<iostream>
#include<fstream>
#include"Model.h"
#include"platform.h"
#include"Camera.h"

std::ofstream outputfile;

static const char* WINDOW_TITLE = "Viewer";
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

//vertex shader所需的uniform
mat4 ViewProjMatrix;
mat4 viewportMatrix;

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}


void draw_line(int x0, int y0, int x1, int y1, std::shared_ptr<framebuffer_t> frame, vec4 color)
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
void draw_triangle(vec3 *pts, framebuffer_t &frame,vec4 color)
{
   
    vec2i bboxmin(frame.width-1, frame.height - 1);
    vec2i bboxmax(0,0);
    vec2i clamp(frame.width - 1, frame.height - 1);
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
            if (frame.depthbuffer[int(p.x + p.y * frame.width)]< p.z)
            {





                frame.depthbuffer[int(p.x + p.y * frame.width)] = p.z;
                frame.set((int)p.x, (int)p.y, color);
            }
            //这里zbuffer暂定是越大则离相机越近
        }
    }

}
void DrawModelLine(Model& m, std::shared_ptr<framebuffer_t> f, vec4 color)
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
void DrawModelTriangle(Model& m, framebuffer_t& f, vec4 color)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        vec3 screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            vec3 world_coords = m.vert(i, j);
            screen_coords[j] = vec3((world_coords.x + 1.) * f.width / 2, (world_coords.y + 1.) * f.height / 2, world_coords.z);

        }
        draw_triangle(screen_coords, f, color);
    }
}





void draw_triangle(vec3i t0,vec3i t1,vec3i t2,float ity0,float ity1,float ity2,framebuffer_t &f)
{
    if (t0.y == t1.y && t0.y == t2.y) return; // i dont care about degenerate triangles
    if (t0.y > t1.y) { std::swap(t0, t1); std::swap(ity0, ity1); }
    if (t0.y > t2.y) { std::swap(t0, t2); std::swap(ity0, ity2); }
    if (t1.y > t2.y) { std::swap(t1, t2); std::swap(ity1, ity2); }

    //用vec3 double则会导致y值插值不正确？
    int total_height = t2.y - t0.y;
    
    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
        vec3i A = t0+vec3f(t2 - t0) * alpha;
        vec3i B = second_half ? t1+vec3f(t2 - t1) * beta : t0+vec3f(t1 - t0) * beta;
        float ityA = ity0 + (ity2 - ity0) * alpha;
        float ityB = second_half ? ity1 + (ity2 - ity1) * beta : ity0 + (ity1 - ity0) * beta;
        if (A.x > B.x) { std::swap(A, B); std::swap(ityA, ityB); }
        for (int j = A.x; j <= B.x; j++)
        {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (B.x - A.x);
            vec3i    P = vec3f(A) + vec3f(B - A) * phi;
            float ityP = ityA + (ityB - ityA) * phi;
            int idx = P.x + P.y *f.width ;
            if (P.x >= f.width || P.y >= f.height || P.x < 0 || P.y < 0)continue;
            if (f.depthbuffer[idx] < P.z)
            {
                f.depthbuffer[idx] = P.z;
                f.set(P.x, P.y, vec4f(ityP,ityP,ityP,1));
            }
            //z-buffer的值都是插值出来的
        }
    }

}

enum { X, Y, Z, W };
#define  MAX_VERTICES_PER_POLYGON 10
#define W_CLIPPING_PLANE 0.00001
typedef struct polygon
{
    unsigned char hs_numVertices;
    vec4 hs_vetices[MAX_VERTICES_PER_POLYGON];// clip coords
};





void ClipPolygonOnWAxis(polygon* face)//函数参数应该是多边形顶点数组 这里多边形暂定只能为三角形
{
    int i;

    vec4* currentVertices;
    vec4* previousVertices;

    unsigned char in_numVertices = 0;
    vec4 in_vertices[MAX_VERTICES_PER_POLYGON];

    char preiousDot, currentDot;

    float intersectionFactor;
    vec4 intersectionPoint;

    previousVertices = &face->hs_vetices[face->hs_numVertices - 1];









  
   
}

void clipPolygonForAxis(polygon* face, int AXIS)
{

}
void clipTriangle(polygon * face)
{
    ClipPolygonOnWAxis(face);
    clipPolygonForAxis(face, X);//w=x,w=-x


}

//这个drawmodeltriangle可以作为model类的成员函数draw内容
void DrawModelTriangle(Model& m,framebuffer_t & f,camera &c)
{
    for (int i = 0; i < m.nfaces(); i++)
    {
        vec3i screen_coords[3];
        vec3 world_coords[3];
        polygon face;
        face.hs_numVertices = 3;
        for (int j = 0; j < 3; j++)
        {
           //1.取出模型坐标系坐标
           vec3 v= m.vert(i, j);
           //2.乘以model matrix变为world coords,暂时忽略
           world_coords[j] = v;
           //3.把world_coords乘以viewproj变为clip coords
           face.hs_vetices[j] = ViewProjMatrix * embed(world_coords[j]);
           //4.剪切
          
           
           //screen_coords[j] =proj(viewportMatrix*clip_coords[j]); 
      
        }

        //每个三角形都进行一次裁剪
        clipTriangle(&face);
     
        //裁剪完后进行组装三角形
        //然后透视除法 视口变换
        //然后背面剔除 
        //像素化三角形
        



        //背面剔除
        //std::cout << "世界坐标系：" << world_coords[0] << world_coords[1] << world_coords[2] << std::endl;
        vec3 n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();
       // std::cout << "法向量：" << n << std::endl;
        double intensity = n * vec3(0, 0, -1);
       
        if (intensity > 0)
        {
            //std::cout << "intensity:" << std::endl;
            draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2], intensity, intensity, intensity,f);
        }
    }
}

static const vec3 CAMERA_POSITION = { 0,0,20 };
static const vec3 CAMERA_TARGET = { 0, 0, 0 };

typedef struct {
    /* orbit */
    int is_orbiting;
    vec2 orbit_pos;
    vec2 orbit_delta;
    /* pan */
    int is_panning;
    vec2 pan_pos;
    vec2 pan_delta;
    /* zoom */
    float dolly_delta;
    /* light */
    float light_theta;
    float light_phi;
    /* click */
    float press_time;
    float release_time;
    vec2 press_pos;
    vec2 release_pos;
    int single_click;
    int double_click;
    vec2 click_pos;
} record_t;
static const float CLICK_DELAY = 0.25f;
static vec2 get_cursor_pos(window_t* window) {
    float xpos, ypos;
    input_query_cursor(window, &xpos, &ypos);
    return vec2(xpos, ypos);
}
static vec2 get_pos_delta(vec2 old_pos, vec2 new_pos) {
    vec2 delta = new_pos-old_pos;
    return delta* (1.0/WINDOW_HEIGHT);
}
static void button_callback(window_t* window, button_t button, int pressed) {
    record_t* record = (record_t*)window_get_userdata(window);
    vec2 cursor_pos = get_cursor_pos(window);
   
    if (button == BUTTON_L) {
       
        
        float curr_time = platform_get_time();
        if (pressed) {
        
            record->is_orbiting = 1;
            record->orbit_pos = cursor_pos;
            record->press_time = curr_time;
            record->press_pos = cursor_pos;
        }
        else {
            float prev_time = record->release_time;
            vec2 pos_delta = get_pos_delta(record->orbit_pos, cursor_pos);
            record->is_orbiting = 0;
            record->orbit_delta =record->orbit_delta+ pos_delta;
            if (prev_time && curr_time - prev_time < CLICK_DELAY) {
                record->double_click = 1;
                record->release_time = 0;
            }
            else {
                record->release_time = curr_time;
                record->release_pos = cursor_pos;
            }
        }
    }
    else if (button == BUTTON_R) {
        if (pressed) {
            record->is_panning = 1;
            record->pan_pos = cursor_pos;
        }
        else {
            vec2 pos_delta = get_pos_delta(record->pan_pos, cursor_pos);
            record->is_panning = 0;
            record->pan_delta = record->pan_delta+ pos_delta;
        }
    }
}
static void update_camera(window_t* window, camera& camera,
    record_t& record) {
    vec2 cursor_pos = get_cursor_pos(window);
    if (record.is_orbiting) {
        vec2 pos_delta = get_pos_delta(record.orbit_pos, cursor_pos);
        record.orbit_delta = record.orbit_delta+ pos_delta;
        record.orbit_pos = cursor_pos;
    }
    if (record.is_panning) {
        vec2 pos_delta = get_pos_delta(record.pan_pos, cursor_pos);
        record.pan_delta = record.pan_delta+pos_delta;
        record.pan_pos = cursor_pos;
    }
    if (input_key_pressed(window, KEY_SPACE)) {
        camera.SetCamera(CAMERA_POSITION, CAMERA_TARGET);
       
    }
    else {
        motion motion;
        motion.orbit = record.orbit_delta;
        motion.pan = record.pan_delta;
        motion.dolly = record.dolly_delta;
        std::cout << "dolly:" << record.dolly_delta << std::endl;
        camera.OrbitUpdate(motion);
        
    }
}
static void update_click(float curr_time, record_t* record) {
    float last_time = record->release_time;
    if (last_time && curr_time - last_time > CLICK_DELAY) {
        vec2 pos_delta = record->release_pos- record->press_pos;
        if (pos_delta.norm() < 5) {
            record->single_click = 1;
        }
        record->release_time = 0;
    }
    if (record->single_click || record->double_click) {
        float click_x = record->release_pos.x / WINDOW_WIDTH;
        float click_y = record->release_pos.y / WINDOW_HEIGHT;
        record->click_pos = vec2(click_x, 1 - click_y);
    }
}
static void scroll_callback(window_t* window, float offset) {
    record_t* record = (record_t*)window_get_userdata(window);
    record->dolly_delta += offset;
    std::cout << "now the dolly_delta" <<record->dolly_delta<< std::endl;
}
int main()
{
 
    record_t record;
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    camera cam(CAMERA_POSITION,CAMERA_TARGET,aspect);
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.button_callback = button_callback;
    callbacks.scroll_callback = scroll_callback;

	window_t* window = window_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
 
    framebuffer_t framebuffer = framebuffer_t(WINDOW_WIDTH,WINDOW_HEIGHT);
    memset(&record, 0, sizeof(record_t));
    window_set_userdata(window, &record);
    input_set_callbacks(window, callbacks);

    Model m("res//test.obj");
    
    viewportMatrix = viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    mat4 proj = mat4::identity();


	//the origin is the left_bottom
	while (!window_should_close(window))
	{
       
        update_camera(window, cam, record);
        proj[3][2] = -1.f / cam.GetForward();
        ViewProjMatrix = proj*cam.GetViewMatrix();
        framebuffer.reset();
	 
        DrawModelTriangle(m, framebuffer,cam);
        window_draw_buffer(window, framebuffer);

       record.orbit_delta = vec2(0, 0);
       record.pan_delta = vec2(0, 0);
       record.dolly_delta = 0;
       record.single_click = 0;
       record.double_click = 0;

       input_poll_events();
	}
	window_destroy(window);
	framebuffer.framebuffer_release();
	
}