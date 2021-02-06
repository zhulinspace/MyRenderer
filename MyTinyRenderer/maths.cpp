#include"maths.h"
/*view matrix and projection matrix*/


/*
 * eye: the position of the eye point
 * target: the position of the target point
 * up: the direction of the up vector
 *
 * x_axis.x  x_axis.y  x_axis.z  -dot(x_axis,eye)
 * y_axis.x  y_axis.y  y_axis.z  -dot(y_axis,eye)
 * z_axis.x  z_axis.y  z_axis.z  -dot(z_axis,eye)
 *        0         0         0                 1
 *
 * z_axis: normalize(eye-target), the backward vector
 * x_axis: normalize(cross(up,z_axis)), the right vector
 * y_axis: cross(z_axis,x_axis), the up vector
 *
 * see http://www.songho.ca/opengl/gl_camera.html
 */
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
        //res[i][3] = -target[i];
    }
    res[0][3] = -(x * pos);
    res[1][3] = -(y * pos);
    res[2][3] = -(z * pos);
    return res;
}

/*
 * left, right: the coordinates for the left and right clipping planes
 * bottom, top: the coordinates for the bottom and top clipping planes
 * near, far: the distances to the near and far depth clipping planes
 *
 * 2/(r-l)        0         0  -(r+l)/(r-l)
 *       0  2/(t-b)         0  -(t+b)/(t-b)
 *       0        0  -2/(f-n)  -(f+n)/(f-n)
 *       0        0         0             1
 *
 * see http://docs.gl/gl2/glOrtho
 */
mat4 ortho(float left, float right, float bottom, float top, float near, float far)
{
    /*
    这里保证far值大于near，即输入值都保证为正数，大于0，而在矩阵里会进行反号
    */
    float x_range = right - left;
    float y_range = top - bottom;
    float z_range = far - near;
    mat4 m = mat4::identity();
    assert(x_range > 0 && y_range > 0 && z_range > 0);
    m[0][0] = 2 / x_range;
    m[1][1] = 2 / y_range;
    m[2][2] = -2 / z_range;
    m[0][3] = -(left + right) / x_range;
    m[1][3] = -(bottom + top) / y_range;
    m[2][3] = -(near + far) / z_range;
    return m;


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