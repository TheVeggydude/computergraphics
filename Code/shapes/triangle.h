#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../object.h"

class Triangle : public Object
{
public:
    Triangle(Point A, Point B, Point C) : A(A), B(B), C(C) { }

    void computeTexCoords(Point center, double* u, double* v); 
    virtual Hit intersect(const Ray &ray);

    const Point A;
    const Point B;
    const Point C;
};

#endif /* end of include guard: TRIANGLE_H */
