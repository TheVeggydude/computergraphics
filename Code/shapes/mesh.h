#ifndef MESH_H
#define MESH_H

#include "../object.h"
#include "triangle.h"

class Mesh : public Object
{
public:
    explicit Mesh(std::string const &filename, Point pos);

    virtual Hit intersect(const Ray &ray);
    
    std::vector<Triangle> triangles;
};

#endif /* end of include guard: MESH_H */
