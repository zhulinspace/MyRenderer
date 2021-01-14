#include"maths.h"
/*view matrix and projection matrix*/

mat4 lookat(vec3 pos, vec3 target, vec3 up)
{
    vec3 z = (pos - target).normalize();
    vec3 x = cross(up, z).normalize();
    vec3 y = cross(z, x).normalize();
    mat4 res = mat4::identity();
  
    for (int i = 0; i < 3; i++)
    {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -target[i];
    }
    return res;
}
mat4 proj_matrix(float fovy, float aspect, float near, float far)
{
    float z_range = far - near;
    mat4 m = mat4::identity();
    assert(fovy > 0 && aspect > 0);
    assert(near > 0 && far > 0 && z_range > 0);

    m[1][1] = 1 / (float)tan(fovy / 2);
    m[0][0] = m[1][1] / aspect;
    m[2][2] = -(near + far) / z_range;
    m[2][3] = (-2 * near * far) / z_range;
    m[3][2] = -1;
    m[3][3] = 0;
    return m;
}

mat4 viewport(int x, int y, int w, int h)
{
    //ns=0.fs=1
    int depth = 255;
    mat4 m = mat4::identity();
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = 1 / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = 1 / 2.f;
    return m;
}