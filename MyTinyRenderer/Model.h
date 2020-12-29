#pragma once
#include <vector>
#include <string>
#include"image.h"
#include"Math.h"

class Model {
private:
    std::vector<Vec3f> verts_;     // array of vertices
    std::vector<Vec2f> uv_;        // array of tex coords
    std::vector<Vec3f> norms_;     // array of normal vectors
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
    Vec3f normal(const int iface, const int nthvert) const;  // per triangle corner normal vertex
    Vec3f normal(const Vec2f& uv) const;                      // fetch the normal vector from the normal map texture
    Vec3f vert(const int i) const;
    Vec3f vert(const int iface, const int nthvert) const;
    Vec2f uv(const int iface, const int nthvert) const;
    //TGAColor diffuse(const Vec2f& uv) const;
    double specular(const Vec2f& uv) const;
};