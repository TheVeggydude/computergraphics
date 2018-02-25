#ifndef QUAD_H
#define QUAD_H

#include "../object.h"
#include "triangle.h"

class Quad : public Object
{
public:
    Quad(Point A, Point B, Point C, Point D);

    virtual Hit intersect(const Ray &ray);

    std::vector<Triangle> triangles;
};

#endif /* end of include guard: QUAD_H */
