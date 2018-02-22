#ifndef PLANE_H
#define PLANE_H

#include "../object.h"

class Plane : public Object
{
public:
    Plane(Point A, Point B, Point C) : origin(A) {

      normal = (B-A).cross(C-A).normalized();

    }

    virtual Hit intersect(const Ray &ray);

    const Point origin;
    Vector normal;
};

#endif /* end of include guard: PLANE_H */
