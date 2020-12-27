#pragma once
#include <vector>
#include <string>
#include"image.h"
#include"maths.h"

class Model {
private:
    std::vector<vec3_t> verts_;     // array of vertices
    std::vector<vec2_t> uv_;        // array of tex coords
    std::vector<vec3_t> norms_;     // array of normal vectors
    std::vector<int> facet_vrt_;
    std::vector<int> facet_tex_;  // indices in the above arrays per triangle
    std::vector<int> facet_nrm_;
    image_t diffusemap_;         // diffuse color texture
    image_t normalmap_;          // normal map texture
    image_t specularmap_;        // specular map texture
    void load_texture(const std::string filename, const std::string suffix, image_t& img);
public:
    Model(const std::string filename);
    int nverts() const;
    int nfaces() const;
    vec3_t normal(const int iface, const int nthvert) const;  // per triangle corner normal vertex
    vec3_t normal(const vec2_t& uv) const;                      // fetch the normal vector from the normal map texture
    vec3_t vert(const int i) const;
    vec3_t vert(const int iface, const int nthvert) const;
    vec2_t uv(const int iface, const int nthvert) const;
    //TGAColor diffuse(const vec2_t& uv) const;
    double specular(const vec2_t& uv) const;
};